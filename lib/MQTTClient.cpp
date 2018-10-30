
#include <cassert>

#include "MQTTClient.h"

namespace MQTTClient
{

static int encode_remaining_len (char *remaining_len, size_t len) 
{
  int  nbytes;
  char encoded_byte;

  nbytes = 0;

  do
  {
    encoded_byte  = len % 0x80;
    len          /= 0x80;

    if (len > 0) encoded_byte |= 0x80;

    remaining_len[nbytes++] = encoded_byte;
  }
  while (len > 0);

  return nbytes;
}

static int decode_remaining_len (char **remaining_len)
{
  int encoded_byte;
  int multiplier, value;

  multiplier = 1;
  value      = 0;

  do
  {
    encoded_byte  = **remaining_len;
    value        += (encoded_byte & 0x7f) * multiplier;
    multiplier   *= 0x80;

    if (multiplier > 0x200000) return -1;

    (*remaining_len)++;
  }
  while ((encoded_byte & 0x80) != 0);

  return value;
}

MQTTClient::MQTTClient (std::string client_id) : PubSubClient::PubSubClient ()
{
  this->client_id = client_id;
}

void MQTTClient::async_handle_connect ()
{
  this->handle_connect ();
}

void MQTTClient::handle_recv (char *buf, size_t size)
{
  char    *ptr;
  int      remaining_len;
  uint8_t  header, header_type;

  ptr = buf;

  memcpy (&header, (ptr++), sizeof (uint8_t));

  header_type   = header & 0xF0;
  remaining_len = decode_remaining_len (&ptr);

  if (remaining_len == -1) this->handle_error ("Failed to decode remaining len.", __LINE__);

  switch (header_type)
  {
    case CONNACK:
      this->recv_connack (ptr, remaining_len);
      break;
    case PUB:
      this->recv_pub (ptr, remaining_len);
      break;
    case SUBACK:
      this->recv_suback (ptr, remaining_len);
      break;    
    default:
      this->handle_error ("Unknown header type.", __LINE__);
  }
}

void MQTTClient::handle_connect ()
{
  this->send_conn ();
}

void MQTTClient::handle_publish (std::string topic, std::string message)
{
  this->send_pub (topic, message);
}

void MQTTClient::handle_subscribe (std::vector<std::string> topics)
{
  this->send_sub (topics);
}

void MQTTClient::handle_disconnect ()
{
  this->send_disconnect ();
}

void MQTTClient::send_conn ()
{
  conn_pkt pkt;
  uint8_t  header;
  size_t   payload_len, rlen_size, pkt_len, len;
  char     remaining_len[4], *payload, *buf, *ptr;

  header      = CONN;
  pkt_len     = 0;
  payload_len = 0;

  memset (&pkt, 0, sizeof (conn_pkt));
  memcpy (&pkt.proto_name, PROTO, 6);

  pkt.proto_level    = 0x4;
  pkt.conn_flag     |= CLEAN_SESS;
  pkt.keep_alive[0]  = MSB(200); 
  pkt.keep_alive[1]  = LSB(200); 
  pkt_len           += sizeof (conn_pkt);

  /*  TODO 
   *
   *  username, password, will_retain, will_qos, will_flag,
   *  clean_session
   *
   */

  if (!this->client_id.empty ()) payload_len += (size_t) (2 + this->client_id.size ());

  payload  = (char *) calloc (payload_len, sizeof (char));

  assert (payload);

  pkt_len += payload_len;
  ptr      = NULL;
  ptr      = payload;

  if (!this->client_id.empty ())
  {
    len      = (size_t) this->client_id.size ();
    *(ptr++) = MSB(len); 
    *(ptr++) = LSB(len); 

    memcpy (ptr, this->client_id.c_str (), len);

    ptr += len;
  }

  memset (remaining_len, 0, sizeof (remaining_len));

  rlen_size  = encode_remaining_len (remaining_len, pkt_len);
  pkt_len   += (1 + rlen_size);
  buf        = (char *) calloc (pkt_len, sizeof (char));

  assert (buf);
  memset (buf, 0, sizeof (char) * pkt_len);

  ptr = NULL;
  ptr = buf;

  /* write header on buf */
  memcpy ((ptr++), &header, sizeof (uint8_t));

  /* write remaining length on buf */
  memcpy (ptr, remaining_len, rlen_size);

  ptr += rlen_size;

  /* write conn pkt on buf */
  memcpy (ptr, &pkt, sizeof (conn_pkt));

  ptr += sizeof (conn_pkt);

  /* write payload on buf */
  memcpy (ptr, payload, payload_len);

  this->send (buf, pkt_len);

  free (payload);
  free (buf);
}

void MQTTClient::recv_connack (char *buf, size_t size)
{
  char        *ptr;
  connack_pkt  pkt;

  ptr = buf;

  memcpy (&pkt, ptr, size);

  /* 
   * TODO handle error TODO 
   *
   * FIXME
   * this behavior should be optional because it
   * blocks the thread until connack pkt is received
   * FIXME
   *
   * notify conn cv to unblock
   */
  std::unique_lock<std::mutex> lock (this->conn_mutex);
  this->connected = true;
  lock.unlock ();
  this->conn_cv.notify_one ();
}

void MQTTClient::send_pub (std::string topic, std::string message)
{
  pub_pkt pkt;
  uint8_t header;
  char    remaining_len[4], *payload, *buf, *ptr;
  size_t  topic_len, payload_len, rlen_size, pkt_len;

  header  = PUB;
  pkt_len = 0;

  memset (&pkt, 0, sizeof (pub_pkt));

  topic_len         = topic.size ();
  pkt.topic_len[0]  = MSB(topic_len);
  pkt.topic_len[1]  = LSB(topic_len);
  pkt.topic         = (char *) topic.c_str ();
  pkt_len          += (2 + topic_len);
  payload_len       = message.size ();
  payload           = (char *) calloc (payload_len, sizeof (char));

  assert (payload);
  memcpy (payload, message.c_str (), payload_len);

  pkt_len += (2 + payload_len);

  memset (remaining_len, 0, sizeof (remaining_len));

  rlen_size  = encode_remaining_len (remaining_len, pkt_len);
  pkt_len   += (1 + rlen_size); // remaing length size + fixed header
  buf        = (char *) calloc (pkt_len, sizeof (char));

  assert (buf);

  ptr = NULL;
  ptr = buf;

  /* write header on buf */
  memcpy ((ptr++), &header, sizeof (uint8_t));

  /* write remaining length on buf */
  memcpy (ptr, remaining_len, rlen_size);

  ptr += rlen_size;

  /* write variable header */
  memcpy (ptr, pkt.topic_len, 2); 

  ptr += 2;

  memcpy (ptr, pkt.topic, topic_len);

  ptr += topic_len;
  ptr += 2; // pkt id bytes

  /* write variable header */
  memcpy (ptr, payload, payload_len);

  this->send (buf, pkt_len);

  free (payload);
  free (buf);
}

void MQTTClient::recv_pub (char *buf, size_t len)
{
  pub_pkt   pkt;
  size_t    payload_len;
  uint16_t  topic_len; //, pkt_id; FIXME
  char     *payload, *ptr;

  memset (&pkt, 0, sizeof (pub_pkt));

  payload = NULL;
  ptr     = NULL;
  ptr     = buf;

  memcpy (pkt.topic_len, ptr, 2); 

  topic_len = pkt.topic_len[0] << 8 | pkt.topic_len[1];

  ptr += 2;
  pkt.topic = (char *) calloc (topic_len + 1, sizeof (char));

  assert (pkt.topic);
  memcpy (pkt.topic, ptr, topic_len); 

  ptr += topic_len;
  pkt.topic[topic_len] = '\0';

  /*
  FIXME mosquitto_sub ignores pkt id field (maybe mqtt version issue?)
  memcpy (pkt.pkt_id, ptr, 2); 

  ptr += 2;
  pkt_id = pkt.pkt_id[0] << 8 | pkt.pkt_id[1];

  payload_len = len - (2 + topic_len + 2);
  FIXME
  */

  payload_len = len - (2 + topic_len);

  payload = (char *) calloc (payload_len + 1, sizeof (char));

  assert (payload);
  memcpy (payload, ptr, payload_len);

  payload[payload_len] = '\0';

  printf ("topic_len: %d, topic: %s, payload %s\n", topic_len, pkt.topic, payload);
}

void MQTTClient::send_sub (std::vector<std::string> topics)
{
  sub_pkt         pkt;
  uint8_t         header;
  sub_pkt_payload pkt_payload;
  size_t          topic_len, payload_len, rlen_size, pkt_len;
  char            remaining_len[4], *buf, *ptr;

  std::string topic;
  std::vector<std::string>::iterator it;

  header  = SUB | 0x2;
  pkt_len = sizeof (sub_pkt);

  memset (&pkt, 0, sizeof (sub_pkt));

  payload_len = 0;

  for (it = topics.begin (); it != topics.end (); it++)
  {
    topic        = *it;
    topic_len    = topic.size ();
    payload_len += 2 + topic_len + 1; // size + topic len + qos
  }

  pkt_len += payload_len;

  memset (remaining_len, 0, sizeof (remaining_len));

  rlen_size  = encode_remaining_len (remaining_len, pkt_len);
  pkt_len   += (1 + rlen_size); // remaing length size + fixed header
  buf        = (char *) calloc (pkt_len, sizeof (char));

  assert (buf);

  ptr = NULL;
  ptr = buf;

  /* write header on buf */
  memcpy ((ptr++), &header, sizeof (uint8_t));

  /* write remaining length on buf */
  memcpy (ptr, remaining_len, rlen_size);

  ptr += rlen_size;

  /* write variable header */
  memcpy (ptr, pkt.pkt_id, sizeof (pkt.pkt_id)); 

  ptr += sizeof (pkt.pkt_id);

  for (it = topics.begin (); it != topics.end (); it++)
  {
    topic     = *it;
    topic_len = topic.size ();

    memset (&pkt_payload, 0, sizeof (sub_pkt_payload));

    pkt_payload.topic_len[0] = MSB(topic_len);
    pkt_payload.topic_len[1] = LSB(topic_len);
    pkt_payload.topic        = (char *) topic.c_str ();

    memcpy (ptr, pkt_payload.topic_len, sizeof (pkt_payload.topic_len)); 

    ptr += 2;

    memcpy (ptr, pkt_payload.topic, topic_len); 

    ptr += topic_len;

    memcpy ((ptr++), &pkt_payload.qos, sizeof (pkt_payload.qos));
  }

  this->send (buf, pkt_len);

  free (buf);
}

void MQTTClient::recv_suback (char *buf, size_t size)
{
  suback_pkt pkt;
  uint16_t   pkt_id;

  memset (&pkt, 0, sizeof (suback_pkt));

  pkt_id = pkt.pkt_id[0] << 8 | pkt.pkt_id[1];

  switch (pkt.ret_code)
  {
    case 0x00:
      /* TODO handle qos0 TODO */
      break;
    case 0x01:
      /* TODO handle qos1 TODO */
      break;
    case 0x02:
      /* TODO handle qos2 TODO */
      break;
    case 0x80:
      /* TODO handle error TODO */
      break;
  }
}

void MQTTClient::send_disconnect ()
{
  size_t   pkt_len;
  char    *ptr, *buf;
  uint8_t  header, remaining_len;

  header        = DISCONNECT;
  remaining_len = 0;
  pkt_len       = 2;
  buf           = (char *) calloc (pkt_len, sizeof (char));

  assert (buf);

  ptr = NULL;
  ptr = buf;

  /* write header */
  memcpy ((ptr++), &header, sizeof (uint8_t));

  /* write remaining len */
  memcpy (ptr, &remaining_len, sizeof (uint8_t));

  this->send (buf, pkt_len);

  free (buf);
}

}
