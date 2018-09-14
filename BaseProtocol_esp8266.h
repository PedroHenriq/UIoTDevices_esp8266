#ifndef BaseProtocol_esp8266_H
#define BaseProtocol_esp8266_H


#include <PubSubClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <FS.h>

class Service {
  public:
    Service (int number, const char *name, String unit, bool numeric, String parameter){
      this->number = number;
      this->name = name;
      this->unit = unit;
      this->numeric = numeric;
      this->parameter = parameter;
    }
      Service(){}
    int number;
    const char * name;
    String *tags;
    String unit = "";
    bool numeric;
    String parameter = "";
  };


class BaseProtocol_esp8266 {
    public:
        bool send_data();
        bool register_all(Service, char*, int);
        virtual bool register_device() = 0;
        virtual bool register_service(Service)= 0;
        virtual bool register_data(Service, char*, int)= 0;
        bool send_data(Service , float*, int, int);
        void device_identificator();
        Service create_service(int , const char *, String, bool, String);
        char nibble_to_char(int);
        char* float_to_char(float*, int);
        char *make_client_data(char*);
        char *make_service_data(Service, char*);
        char *make_raw_data(Service, char*, int, char*);
        byte get_value_from_char(char);
        char *append_json(char*, const char*, const char*);

    private:
        bool DEVICE_REGISTERED = false;

    protected:
        byte mac_byte[6] = {0x9A, 'I', 'O', 'T', 0, 0};
        byte chipset_byte[6];
        String name = "Arduino Uno";
        String serial = "C210";
        String processor = "Arduino Processor";
        String channel = "Ethernet";
        String chipset = "";
        String mac = "";
};

#endif
