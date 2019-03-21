
#include <mqtt/async_client.h>

#include "Protocol.h"

enum MQTTOption
{
  QOS,
  CLEAN_SESSION
};

enum MQTTState
{
  IDLE,
  CONNECTED,
  CONNECTING,
  DISCONNECTED,
  CONNECTION_LOST,
  CONNECTION_FAILED
};

enum MQTTActionType
{
  CONNECT,
  PUBLISH,
  SUBSCRIBE
};

class MQTTAction
{
public:
  MQTTAction(MQTTActionType type) : type(type) {}

  MQTTActionType type;
};

class MQTTSubAction : public MQTTAction
{
public:
  MQTTSubAction(std::string topic) : MQTTAction(SUBSCRIBE), topic(topic) {}

  std::string topic;
};

class MQTTPubAction : public MQTTAction
{
public:
  MQTTPubAction(std::string topic, std::string message) : MQTTAction(PUBLISH),
                                                          topic(topic),
                                                          message(message) {}

  std::string topic;
  std::string message;
};

class MQTTConnAction : public MQTTAction
{
public:
  MQTTConnAction(std::function<void ()> callback) : MQTTAction(CONNECT),
                                                    callback(callback) {}

  std::function<void ()> callback;
};

class MQTTCallback;
class MQTTSubActionListener;

class MQTT : public Protocol {
public:
  MQTT(std::string host, int port = 1883);
  ~MQTT();

  bool isConnOriented;

  void setState(MQTTState);

  void setOption(MQTTOption opt, int value);
  void setOption(MQTTOption opt, bool value);

  void addTopic(std::string);

  void connect() override;
  void connect(std::function<void ()> callback) override;
  void disconnect() override;
  void subscribe(std::string topic) override;
  void publish(std::string topic, std::string msg) override;

private:
  static int _id;

  int _qos;
  int _port;
  MQTTState _state;
  std::string _host;
  std::string _clientId;
  mqtt::connect_options _connOpts;
  std::vector<std::string> _topics;
  std::unique_ptr<MQTTCallback> _callback;
  std::unique_ptr<mqtt::async_client> _client;
  std::queue<std::unique_ptr<MQTTAction>> _actions;
  std::unique_ptr<MQTTSubActionListener> _subActionListener;

  void triggerQueuedActions();
  void stateTransition(MQTTState state);
};

class MQTTSubActionListener : public virtual mqtt::iaction_listener
{
public:
  MQTTSubActionListener(MQTT *client) : _client(client) {}

  void on_failure(const mqtt::token& tok) override
  {
    std::cout << "failure" << std::endl;
  }

  void on_success(const mqtt::token& tok) override
  {
    auto topics = tok.get_topics();

    for(int i = 0; i < topics->size(); i++)
    {
      _client->addTopic((*topics)[i]);
    }
  }

private:
  MQTT *_client;
};

class MQTTCallback : public mqtt::callback,
                     public mqtt::iaction_listener
{
public:
  MQTTCallback(MQTT *client) : _client(client) {}

  /* mqtt::callback */
  void connected(const std::string &cause) override
  {
    _client->setState(CONNECTED);
  }

  void connection_lost(const std::string &cause) override
  {
    _client->setState(CONNECTION_LOST);
  }

  void delivery_complete(mqtt::delivery_token_ptr tok) override
  {
    std::cout << "delivery tok " << tok->get_message_id() << std::endl;
  }

  void message_arrived(mqtt::const_message_ptr msg) override
  {
    std::cout << "topic: " << msg->get_topic() << " payload: " << msg->to_string() << std::endl;
  }

  /* mqtt::iaction_listener */
  void on_failure(const mqtt::token& tok) override
  {
    std::cout << "failure" << std::endl;
  }

  void on_success(const mqtt::token& tok) override
  {
    std::cout << "success" << std::endl;
  }

private:
  MQTT *_client;
};

