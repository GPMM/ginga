
#include <iostream>

#include "PubSubClient.h"
#include "AsyncLoop.h"

namespace PubSubClient {

std::shared_ptr<AsyncLoop::AsyncLoop> async_loop = std::make_shared<AsyncLoop::AsyncLoop> ();

PubSubClient::PubSubClient () : PubSubPlug::PubSubPlug (),
                                connected (false),
                                evt_loop (async_loop) {}

PubSubClient::~PubSubClient () {}

void PubSubClient::connect (std::string host, int port)
{
  this->host       = host;
  this->port       = port;
  this->sock->data = this;
  this->conn->data = this;
  
  this->evt_loop->async_conn (this);

  /* 
   * block until it's connected to avoid race conditions
   */
  std::unique_lock<std::mutex> lock (this->conn_mutex);
  conn_cv.wait (lock, [this] () { return this->connected == true; });
  lock.unlock ();
}

void PubSubClient::publish (std::string topic, std::string message)
{
  this->handle_publish (topic, message);
}

void PubSubClient::subscribe (std::vector<std::string> topics)
{
  this->handle_subscribe (topics);
}

void PubSubClient::disconnect ()
{
  this->handle_disconnect ();
  this->evt_loop->async_close (this);

  /* 
   * block until it's disconnected to avoid race conditions
   */
  std::unique_lock<std::mutex> lock (this->conn_mutex);
  conn_cv.wait (lock, [this] () { return this->connected == false; });
  lock.unlock ();
}

void PubSubClient::send (char *buf, size_t size)
{
  this->evt_loop->async_send (this, buf, size);
}

void PubSubClient::async_handle_connect ()
{
  /* 
   * notify conn cv to unblock
   */
  std::unique_lock<std::mutex> lock (this->conn_mutex);
  this->connected = true;
  lock.unlock ();
  this->conn_cv.notify_one ();

  this->handle_connect ();
}

void PubSubClient::async_handle_disconnect ()
{
  /* 
   * notify conn cv to unblock
   */
  std::unique_lock<std::mutex> lock (this->conn_mutex);
  this->connected = false;
  lock.unlock ();
  this->conn_cv.notify_one ();
}

void PubSubClient::handle_error (std::string msg, int line)
{
  std::cerr << "ERROR " << line << ": " << msg << std::endl;
  /* TODO handle error TODO */
}

}

