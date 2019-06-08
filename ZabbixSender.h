

#ifndef ZABBIXSENDER_H
#define ZABBIXSENDER_H

#include <stdio.h>
#include <string>

#ifndef ZJSONBUFFER_SIZE
#define ZJSONBUFFER_SIZE 1100
#endif

class ZabbixSender {
public:
  ZabbixSender();
  ZabbixSender(const ZabbixSender& orig);
  virtual ~ZabbixSender();
  String createPayload(const char *hostname, float temp, float humid);
  String createMessage(String jsonPayload);
private:
};

#endif /* ZABBIXSENDER_H */
