#ifndef BaseProtocol_esp8266_H
#define BaseProtocol_esp8266_H


#include <PubSubClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <FS.h>

class Service {
  public:
    Service (int number, const char *name, String unit, int numeric, String parameter){
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
    int numeric;
    String parameter = "";
    bool registered = false;
  };


class BaseProtocol_esp8266 {
    public:
        bool send_data();
        void device_identificator();
        bool register_all(int, char*, int);
        bool register_service_data(int, char*, int);
        virtual bool register_device() = 0;
        virtual bool register_service(int)= 0;
        virtual bool register_data(int, char*, int)= 0;
        bool send_data(int , float*, int, int);
        bool send_data(int , char*, int);
        char* float_to_char(float*, int);
        int create_service(int , const char *, String, bool, String);
        char nibble_to_char(int);
        char *make_client_data(char*);
        char *make_service_data(Service, char*);
        char *make_raw_data(Service, char*, int, char*);
        byte get_value_from_char(char);
        char *append_json(char*, const char*, const char*, int);

    private:
        bool DEVICE_REGISTERED = false;


    protected:
        byte mac_byte[6] = {0x9A, 'I', 'O', 'T', 0, 0};
        byte chipset_byte[6];
        String name = "Arduino";
        String serial = "CA124";
        String processor = "ATMega";
        String channel = "Ethernet";
        String chipset = "";
        String mac = "";
        Service service[5];
        int service_index = 0;
};

#endif
