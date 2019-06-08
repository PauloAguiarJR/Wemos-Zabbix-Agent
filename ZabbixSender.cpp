

#include <string>
#include <ArduinoJson.h>
#include "ZabbixSender.h"

ZabbixSender::ZabbixSender() {
}

ZabbixSender::ZabbixSender(const ZabbixSender& orig) {
}

ZabbixSender::~ZabbixSender() {
}

String ZabbixSender::createPayload(const char *hostname, float temp, float humid) {
    DynamicJsonBuffer jsonBuffer(ZJSONBUFFER_SIZE);
    JsonObject& root = jsonBuffer.createObject();
    root["request"] = "sender data";
    
    JsonArray& data = root.createNestedArray("data");
    
    JsonObject& data_0 = data.createNestedObject();
    data_0["host"] = hostname;
    data_0["key"] = "temp";
    data_0["value"] = temp;
    
    JsonObject& data_1 = data.createNestedObject();
    data_1["host"] = hostname;
    data_1["key"] = "humid";
    data_1["value"] = humid;
    
    String json;
    root.printTo(json); 
    return json;
}

/**
 * Create message to send to zabbix based on payload
 */
String ZabbixSender::createMessage(String jsonPayload) {
    String header = "ZBXD\x01";
    // we will use only 2 bytes, will never exceed length = 65565
    String placeholderLen = "12345678";  
    String msg = header + placeholderLen + jsonPayload;
    uint16_t len = jsonPayload.length();
         
    // patch the string
    char zNull = '\x00';
    unsigned char lenLsb = (unsigned char) (len & 0xFF);
    unsigned char lenMsb = (unsigned char) ((len >> 8)& 0xFF);  
  
    msg.setCharAt(5, lenLsb);
    msg.setCharAt(6, lenMsb);
    // len padding
    msg.setCharAt(7, zNull);
    msg.setCharAt(8, zNull);
    msg.setCharAt(9, zNull);
    msg.setCharAt(10, zNull);
    msg.setCharAt(11, zNull);
    msg.setCharAt(12, zNull);      
     
    return msg;
}
