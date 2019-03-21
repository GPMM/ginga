
#include <iostream>

#include "MQTT.h"

int MQTT::_id = 0;

MQTT::MQTT(std::string host, int port) : Protocol(),
                                         _qos(0),
                                         _port(port),
                                         _host(host),
                                         _state(IDLE),
                                         isConnOriented(true),
                                         _clientId("client" + std::to_string(_id++)),
                                         _callback(std::make_unique<MQTTCallback>(this)),
                                         _client(std::make_unique<mqtt::async_client>(_host, _clientId)),
                                         _subActionListener(std::make_unique<MQTTSubActionListener>(this))
{
  _client->set_callback (*_callback);
}

MQTT::~MQTT() {}

void MQTT::setState(MQTTState state)
{
  this->stateTransition(state);
  _state = state;
}

void MQTT::setOption(MQTTOption opt, int value)
{
  switch (opt)
  {
    case QOS:
      if (value >= 0 && value <= 3)
        _qos = value;
      break;
  }
}

void MQTT::setOption(MQTTOption opt, bool value)
{
  switch (opt)
  {
    case CLEAN_SESSION:
      _connOpts.set_clean_session(value);
      break;
  }
}

void MQTT::addTopic(std::string topic)
{
  _topics.push_back(topic);
}

void MQTT::connect(std::function<void ()> callback)
{
  if (_state == CONNECTED || _state == CONNECTING)
    return;

  std::unique_ptr<MQTTConnAction> action = std::make_unique<MQTTConnAction>(callback);
  _actions.push(std::move(action));

  this->connect();
}

void MQTT::connect()
{
  if (_state == CONNECTED || _state == CONNECTING)
    return;

  this->setState(CONNECTING);
  _client->connect(_connOpts);
}

void MQTT::disconnect()
{
  _client->disconnect();
}

void MQTT::publish(std::string topic, std::string message)
{
  if (_state == CONNECTING)
  {
    std::unique_ptr<MQTTPubAction> action = std::make_unique<MQTTPubAction>(topic, message);
    _actions.push(std::move(action));
    return;
  }

  mqtt::message_ptr pubMessage = mqtt::make_message(topic, message.c_str());
  pubMessage->set_qos(_qos);
  _client->publish(pubMessage);
}

void MQTT::subscribe(std::string topic)
{
  if (_state == CONNECTING)
  {
    std::unique_ptr<MQTTSubAction> action = std::make_unique<MQTTSubAction>(topic);
    _actions.push(std::move(action));
    return;
  }

  _client->subscribe(topic, _qos, nullptr, *_subActionListener);
}

void MQTT::triggerQueuedActions()
{
  MQTTConnAction *conn;
  MQTTPubAction *publish;
  MQTTSubAction *subscribe;

  while (!_actions.empty())
  {
    std::unique_ptr<MQTTAction> action = std::move(_actions.front());

    switch (action->type)
    {
      case CONNECT:
        conn = static_cast<MQTTConnAction *> (action.get());
        conn->callback ();
        break;
      case PUBLISH:
        publish = static_cast<MQTTPubAction *> (action.get());
        this->publish(publish->topic, publish->message);
        break;
      case SUBSCRIBE:
        subscribe = static_cast<MQTTSubAction *> (action.get());
        this->subscribe(subscribe->topic);
        break;
    }

    _actions.pop();
  }
}

void MQTT::stateTransition(MQTTState state)
{
  switch (_state)
  {
    case CONNECTING:
      switch (state)
      {
        case CONNECTED:
          _state = state;
          this->triggerQueuedActions();
          break;
      }

      break;
  }
}

