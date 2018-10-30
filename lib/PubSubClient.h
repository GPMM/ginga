
#ifdef __cplusplus
extern "C" {
#endif

#include <uv.h>

#ifdef __cplusplus
}
#endif

#include "PubSubPlug.h"
#include "AsyncLoop.h"

#ifndef __PUB_SUB_CLIENT__
#define __PUB_SUB_CLIENT__

namespace PubSubClient
{

class PubSubClient : public PubSubPlug::PubSubPlug
{

public:

  PubSubClient ();

  ~PubSubClient ();

  void connect (std::string host, int port);
  void publish (std::string topic, std::string msg);
  void subscribe (std::vector<std::string> topics);
  void disconnect ();

  virtual void async_handle_connect ();
  void async_handle_disconnect ();

protected:

  bool connected;

  std::condition_variable conn_cv;
  std::mutex              conn_mutex;

  void send (char *buf, size_t size);
  void handle_error (std::string msg, int line);

  virtual void handle_recv (char *buf, size_t size) = 0;
  virtual void handle_connect () = 0;
  virtual void handle_publish (std::string topic, std::string message) = 0;
  virtual void handle_subscribe (std::vector<std::string> topics) = 0;
  virtual void handle_disconnect () = 0;

private:

  std::shared_ptr<AsyncLoop::AsyncLoop> evt_loop;

};

}

#endif

