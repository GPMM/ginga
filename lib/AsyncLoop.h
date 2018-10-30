
#include <thread>
#include <queue>

#ifdef __cplusplus
extern "C" {
#endif

#include <uv.h>

#ifdef __cplusplus
}
#endif

#include "PubSubPlug.h"

#ifndef __ASYNC_LOOP__
#define __ASYNC_LOOP__

namespace AsyncLoop
{

enum action
{
  CONNECT,
  SEND,
  CLOSE,
  CLOSE_ASYNC
};

struct async_data_s
{
  int                     action;
  PubSubPlug::PubSubPlug *client;
  uv_buf_t                buf;
};

typedef async_data_s async_data;

class AsyncLoop
{

public:

  AsyncLoop ();
  ~AsyncLoop ();

  void async_conn (PubSubPlug::PubSubPlug *client);
  void async_send (PubSubPlug::PubSubPlug *client, char *buf, size_t len);
  void async_close (PubSubPlug::PubSubPlug *client);

private:

  volatile int thread_running;

  uv_loop_t  evt_loop;
  uv_async_t async_handle;

  std::condition_variable loop_cv;
  std::mutex              loop_mutex;
  std::thread             loop_thread;
  std::mutex              queue_mutex;
  std::queue<async_data>  async_queue;

  void async_loop ();
  void close_async_handle ();
  void handle_async ();

};

}

#endif
