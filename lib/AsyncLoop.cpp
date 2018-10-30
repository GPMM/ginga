
#include <cassert>

#include "PubSubPlug.h"
#include "AsyncLoop.h"

namespace AsyncLoop
{

void AsyncLoop::async_loop ()
{
  uv_loop_init (&this->evt_loop);

  this->async_handle.data = this;

  if ((uv_async_init (&this->evt_loop, &this->async_handle, [] (uv_async_t *handle) {
          auto self = (AsyncLoop *) handle->data;
          self->handle_async ();
        })) != 0)
  {
    /* TODO handle error TODO */
    return;
  }

  /* notify loop cv to unblock */
  std::unique_lock<std::mutex> lock (this->loop_mutex);
  this->thread_running = 1;
  lock.unlock ();
  this->loop_cv.notify_one ();

  while (this->thread_running) uv_run (&this->evt_loop, UV_RUN_DEFAULT);
}

AsyncLoop::AsyncLoop () : thread_running (0),
                          loop_thread (std::thread (&AsyncLoop::async_loop, this))
{
  /* block until loop start to avoid race conditions */
  std::unique_lock<std::mutex> lock (this->loop_mutex);
  loop_cv.wait (lock, [this] () { return this->thread_running == 1; });
  lock.unlock ();
}

AsyncLoop::~AsyncLoop ()
{
  if (this->thread_running)
  {
    thread_running = 0;

    this->close_async_handle ();

    if (uv_loop_close (&this->evt_loop) == UV_EBUSY)
    {
      /* TODO handle busy TODO */
    }

    this->loop_thread.join ();
  }
}

void AsyncLoop::handle_async ()
{
  std::unique_lock<std::mutex> lock (this->queue_mutex);

  while (!this->async_queue.empty ())
  {
    async_data async = this->async_queue.front ();
    auto client = async.client;

    switch (async.action)
    {
      case CONNECT:
        struct sockaddr_in addr;

        uv_tcp_init (&this->evt_loop, client->get_sock ());
        uv_ip4_addr (client->get_host ().c_str (), client->get_port (), &addr);

        uv_tcp_connect (client->get_conn (), client->get_sock (), 
             (const struct sockaddr *) &addr, [] (uv_connect_t *conn, int status) {
              auto client = (PubSubPlug::PubSubPlug *) conn->data;
              if (!status) client->async_handle_connect ();
              /* TODO handle error TODO */
            });

        uv_read_start ((uv_stream_t *) client->get_sock (),
            [] (uv_handle_t *handle, long unsigned size, uv_buf_t *buf) {
              buf->base = (char *) malloc (size);
              buf->len  = size;
            }, [] (uv_stream_t *stream, long nread, const uv_buf_t *buf) {
              auto client = (PubSubPlug::PubSubPlug *) stream->data;
              if (nread > 0) client->async_handle_recv (buf->base, buf->len);
              /* TODO handle error TODO */
              free (buf->base);
            });

        break;
      case SEND:
        uv_write ((new uv_write_t), (uv_stream_t *) client->get_sock (), &async.buf, 1,
            [] (uv_write_t *request, int status) {
            delete request;
            /* TODO handle error TODO */
          }); 

        free (async.buf.base);

        break;
      case CLOSE:
        uv_read_stop ((uv_stream_t *) client->get_sock ());

        uv_close ((uv_handle_t *) client->get_sock (), [] (uv_handle_t *handle) {
            auto client = (PubSubPlug::PubSubPlug *) handle->data;
            client->async_handle_disconnect ();
          });

      case CLOSE_ASYNC:
        uv_close ((uv_handle_t *) &this->async_handle, [] (uv_handle_t *handle) {
            /* TODO handle close TODO */
          });

        break;
    }

    this->async_queue.pop ();
  }

  lock.unlock ();
}

void AsyncLoop::async_conn (PubSubPlug::PubSubPlug *client)
{
  async_data async = {
    .action = CONNECT,
    .client = client,
    .buf    = { .base = NULL, .len = 0 }
  };

  std::unique_lock<std::mutex> lock (this->queue_mutex);

  async_queue.push (async);
  uv_async_send (&this->async_handle);

  lock.unlock ();
}

void AsyncLoop::async_send (PubSubPlug::PubSubPlug *client, char *buf, size_t len)
{
  async_data async = {
    .action = SEND,
    .client = client,
    .buf    = { .base = NULL, .len = 0 }
  };

  async.buf.base = (char *) malloc (len);

  assert (async.buf.base);
  memcpy (async.buf.base, buf, len);

  async.buf.len = len;

  std::unique_lock<std::mutex> lock (this->queue_mutex);

  async_queue.push (async);
  uv_async_send (&this->async_handle);

  lock.unlock ();
}

void AsyncLoop::async_close (PubSubPlug::PubSubPlug *client)
{
  async_data async = {
    .action = CLOSE,
    .client = client,
    .buf    = { .base = NULL, .len = 0 }
  };

  std::unique_lock<std::mutex> queue_lock (this->queue_mutex);
  async_queue.push (async);
  uv_async_send (&this->async_handle);
  queue_lock.unlock ();
}

void AsyncLoop::close_async_handle ()
{
  async_data async = {
    .action = CLOSE_ASYNC,
    .client = NULL,
    .buf    = { .base = NULL, .len = 0 }
  };

  std::unique_lock<std::mutex> lock (this->queue_mutex);

  async_queue.push (async);
  uv_async_send (&this->async_handle);

  lock.unlock ();
}

}

