
#include "PubSubClient.h"

#define LSB(x) (uint8_t) (x & 0x00ff)
#define MSB(x) (uint8_t) ((x & 0xff00) >> 8)

namespace MQTTClient
{

const char PROTO[6] = { MSB(0x4), LSB(0x4), 'M', 'Q', 'T', 'T' };

enum flag
{
  CONN       = 0x10,
  CONNACK    = 0x20,
  PUB        = 0x30,
  PUBACK     = 0x40,
  PUBREC     = 0x50,
  PUBREL     = 0x60,
  PUBCOMP    = 0x70,
  SUB        = 0x80,
  SUBACK     = 0x90,
  UNSUB      = 0xA0,
  UNSUBACK   = 0xB0,
  PINGREQ    = 0xC0,
  PINGRESP   = 0xD0,
  DISCONNECT = 0xE0
};

enum conn_flags
{
  USERNAME    = 0x80,
  PASSWORD    = 0x40,
  WILL_RETAIN = 0x20,
  WILL_QOS2   = 0x10,
  WILL_QOS1   = 0x8,
  WILL_QOS0   = 0x0,
  CLEAN_SESS  = 0x2
};

enum retcodes
{
  CONN_ACC           = 0x0,
  UNACC_PROTO_VER    = 0x1,
  ID_REJECTED        = 0x2,
  SERVER_UNAVAILABLE = 0x3,
  BAD_CREDS          = 0x4,
  NOT_AUTH           = 0x5
};

enum pub_flags
{
  DUP    = 0x8,
  QOS_0  = 0x0,
  QOS_1  = 0x2,
  QOS_2  = 0x4,
  RETAIN = 0x1
};

struct fixed_header_s
{
  uint8_t flag;
  uint8_t remaining_len[4];
};

typedef fixed_header_s fixed_header;

struct conn_pkt_s
{
  uint8_t proto_name[6];
  uint8_t proto_level;
  uint8_t conn_flag;
  uint8_t keep_alive[2]; /* { MSB(X), LSB(X) } */
  /* payload */
};

struct connack_pkt_s
{
  uint8_t connack_flag;
  uint8_t ret_code;
};

struct pub_pkt_s
{
  uint8_t  topic_len[2]; /* { MSB(X), LSB(X) } */
  char    *topic;
  uint8_t  pkt_id[2];    /* { MSB(X), LSB(X) } */
  /* payload */
};

struct pubresp_pkt_s
{
  uint8_t pkt_id[2]; /* { MSB(X), LSB(X) } */
};

struct sub_pkt_s
{
  uint8_t pkt_id[2];      /* { MSB(X), LSB(X) } */
  /* payload */
};

struct sub_pkt_payload_s
{
  uint8_t  topic_len[2];
  char    *topic;
  uint8_t  qos;
};

struct suback_pkt_s
{
  uint8_t pkt_id[2]; /* { MSB(X), LSB(X) } */
  uint8_t ret_code;
};

struct unsub_pkt_s
{
  uint8_t pkt_id[2];      /* { MSB(X), LSB(X) } */
  struct unsub_payload_struct {
    uint8_t  topic_len[2]; /* { MSB(X), LSB(X) } */
    char    *topic_filter;
  } *payload;
};

struct unsuback_pkt_s
{
  uint8_t pkt_id[2];
};

typedef conn_pkt_s        conn_pkt;
typedef connack_pkt_s     connack_pkt;
typedef pub_pkt_s         pub_pkt;
typedef pubresp_pkt_s     puback_pkt;
typedef pubresp_pkt_s     pubrec_pkt;
typedef pubresp_pkt_s     pubrel_pkt;
typedef pubresp_pkt_s     pubcomp_pkt;
typedef sub_pkt_s         sub_pkt;
typedef sub_pkt_payload_s sub_pkt_payload;
typedef suback_pkt_s      suback_pkt;
typedef unsub_pkt_s       unsub_pkt;
typedef unsuback_pkt_s    unsuback_pkt;
typedef fixed_header      ping_pkt;
typedef fixed_header      disconnect_pkt;

class MQTTClient : public PubSubClient::PubSubClient
{

public:

  MQTTClient (std::string client_id);

  void async_handle_connect ();

private:

  std::string client_id;

  /* TODO
  std::string username;
  std::string password;
  bool        will_retain;
  bool        will_qos; 
  bool        will_flag;
  bool        clean_session;
  */

  void handle_recv (char *buf, size_t size);
  void handle_connect ();
  void handle_publish (std::string topic, std::string message);
  void handle_subscribe (std::vector<std::string> topic);
  void handle_disconnect ();

  void send_conn ();
  void recv_connack (char *buf, size_t size);

  void send_pub (std::string topic, std::string message);
  void recv_pub (char *buf, size_t size);

  void send_sub (std::vector<std::string> topic);
  void recv_suback (char *buf, size_t size);

  void send_disconnect ();

/* TODO
  int recv_pubrec_pkt ();
  int send_pubrel_pkt ();
  int recv_pubcomp_pkt ();
  
  int recv_suback_pkt ();

  int send_unsub_pkt ();
  int recv_unsuback_pkt ();

  int send_pingreq_pkt ();
  int recv_pingresp_pkt ();

*/
  
};

}
