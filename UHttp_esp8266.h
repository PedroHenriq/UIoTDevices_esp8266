#ifndef UHttp_esp8266_H
#define UHttp_esp8266_H

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "BaseProtocol_esp8266.h"
#include <ESP8266WiFi.h>

class UHttp_esp8266 : public BaseProtocol_esp8266{
    public:
        UHttp_esp8266(String);
        bool register_device();
        bool register_service(Service);
        bool register_data(Service, char*, int);

        // char* add_mac_chipset(char *data);
        // char *json_to_char(JsonObject&);
        // bool publish(const char[], char*);

    private:
        // PubSubClient mqtt_client;
        String server;
        HTTPClient http;
        int mqtt_port = 1883;


};





#endif
