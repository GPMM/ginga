#include <string>

#ifdef __cplusplus
extern "C" {
#endif

#include <uv.h>

#ifdef __cplusplus
}
#endif

#ifndef __PUB_SUB_PLUG__
#define __PUB_SUB_PLUG__

namespace PubSubPlug
{

class PubSubPlug
{

public:

  std::string   get_host () { return this->host; };
  int           get_port () { return this->port; };
  uv_tcp_t     *get_sock () { return this->sock; };
  uv_connect_t *get_conn () { return this->conn; };

  PubSubPlug () : sock (new uv_tcp_t),
                  conn (new uv_connect_t) {}

  ~PubSubPlug () { delete this->sock; delete this->conn; }

  virtual void async_handle_connect () = 0;
  virtual void async_handle_disconnect () = 0;

  void async_handle_recv (char *buf, size_t len) { this->handle_recv (buf, len); };

protected:

  std::string host;
  int         port;

  uv_tcp_t     *sock;
  uv_connect_t *conn; 

  virtual void handle_connect () = 0;
  virtual void handle_recv (char *buf, size_t len) = 0;

};

}

#endif

