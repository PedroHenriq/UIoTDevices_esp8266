#include "BaseProtocol_esp8266.h"

bool BaseProtocol_esp8266::send_data(Service service,char *data, int sensitive=0) {
  Serial.println("Send Data");
	this->DEVICE_REGISTERED = (!this->DEVICE_REGISTERED)? this->register_all(service, data, sensitive) : this->register_data(service, data, sensitive);
  return this->DEVICE_REGISTERED;
}

void BaseProtocol_esp8266::device_identificator(){
  int address = 0;
  int bytes[8];
  // Serial.println(EEPROM.read(0));
  // Serial.println(EEPROM.read(1));
 bool result = SPIFFS.begin();
 Serial.println("SPIFFS opened: " + result);

 File f = SPIFFS.open("/UIOT.txt", "r");

  if (!f){
    Serial.println("nao existe arquivo");
    Serial.println("Criando arquivo");
    File f = SPIFFS.open("/UIOT.txt", "w");
    // // EEPROM.write(0, 74);
    // EEPROM.write(1, 114);

    int iterator = 0xFF;
    for (int i = 0; i < 8; i++){
      int a = millis();
      int b = rand() % 123 + 1;
      Serial.print("millis=");
      Serial.println(a);
      Serial.print("rand=");
      Serial.println(b);
      int unique = a * b;
      Serial.print("Unique=");
      Serial.println(unique);
      bytes[i] = unique & iterator;
      Serial.print("bytes[");
      Serial.print(i);
      Serial.print("]=");
      Serial.println(bytes[i]);


      this->mac_byte[4] = bytes[0];
      this->mac_byte[5] = bytes[1];

      for (int i = 0; i < 6; i++){
        this->chipset_byte[i] = bytes[i+2];
      }

      for(int i = 0; i < 6; i++){
        byte b = this->chipset_byte[i];
        this->chipset += this->nibble_to_char((b & 0xF0)>>4);
        this->chipset += this->nibble_to_char(b & 0xF);
        this->chipset += ":";
      }
      this->chipset.remove(this->chipset.length()-1,1);

      for(int i = 0; i < 6; i++){
        byte b = this->mac_byte[i];
        this->mac += this->nibble_to_char((b & 0xF0)>>4);
        this->mac += this->nibble_to_char(b & 0xF);
        this->mac += ":";
      }
      this->mac.remove(this->mac.length()-1,1);

      f.println(this->mac);
      f.println(this->chipset);

      while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('\n');
      Serial.println(line);
    }


      // EEPROM.write(i + 2,bytes[i]);
      //unique = unique >> 8;
    }
  } else{



    this->mac = f.readStringUntil('\n');
    this->chipset = f.readStringUntil('\n');

    this->mac_byte[4] = this->get_value_from_char(this->mac.charAt(12))*0x10;
    this->mac_byte[4] += this->get_value_from_char(this->mac.charAt(13));

    this->mac_byte[5] = this->get_value_from_char(this->mac.charAt(15))*0x10;
    this->mac_byte[5] += this->get_value_from_char(this->mac.charAt(16));

    Serial.println(this->mac_byte[4]);
    Serial.println(this->mac_byte[5]);
    f.close();


  }






  // String mac = f.readStringUntil('\n');
  // Serial.println(mac);



    Serial.print("MAC: ");
    Serial.println(this->mac);

    Serial.print("CHIPSET: ");
    Serial.println(this->chipset);



  // for (int i = 0; i< 6; i++){
  //   Serial.print(this->mac[i]);
  //   Serial.print(", ");
  // }

  // Serial.print("CHIPSET: ");
  // Serial.println(this->chipset);
  // for (int i = 0; i< 6; i++){
  //   Serial.print(this->chipset[i]);
  //   Serial.print(", ");
  // }

  // Serial.println("");

}

Service BaseProtocol_esp8266::create_service(int number, const char *name, String unit, bool numeric, String parameter){
  return Service(number, name, unit, numeric, parameter);
}

bool BaseProtocol_esp8266::register_all(Service service, char *data, int sensitive){
  Serial.println("Register all");
  return this->register_device() && this->register_service(service) && this->register_data(service, data, sensitive);
  // this->register_device();
  // this->register_service(service);
	// return false;
	//return this->register_device() && this->register_service(service) && this->register_data(data);
}

char *BaseProtocol_esp8266::make_client_data(){
  Serial.println("Registering Device");
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  // Serial.print("Name: ");
  // Serial.println(this->name);
  // Serial.print("Chipset: ");
  // Serial.println(this->chipset);
  // Serial.print("Mac: ");
  // Serial.println(this->mac);
  // Serial.print("serial: ");
  // Serial.println(this->serial);
  // Serial.print("Processor: ");
  // Serial.println(this->processor);
  // Serial.print("Channel: ");
  // Serial.println(this->channel);

  String p;
  root["name"] = this->name.c_str();
  root["chipset"] = this->chipset;
  root["mac"] = this->mac;
  root["serial"] = this->serial.c_str();
  root["processor"] = this->processor;
  root["channel"] = this->channel;
  char *c = new char[root.measureLength() + 1];
  root.printTo((char*)c, root.measureLength() + 1);
  Serial.println("auiiiiii");
  Serial.println(c);
  Serial.println("qqqqqqq");
  return (c);
}


char *BaseProtocol_esp8266::make_service_data(Service service){
  Serial.println("Registering Service");
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  // root["number"] = service.number;


  root["chipset"] = this->chipset;
  root["mac"] = this->mac;
  root["name"] = service.name;
  root["parameter"] = service.parameter;
  root["unit"] = service.unit;
  root["numeric"] = service.numeric;

  char *c = new char[root.measureLength() + 1];
  root.printTo((char*)c, root.measureLength() + 1);
  return(c);
}

char *BaseProtocol_esp8266::make_raw_data(Service s, char *data, int sensitive){
  Serial.println("Raw Data");
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["chipset"] = this->chipset;
  root["mac"] = this->mac;
  root["sensitive"] = sensitive;
  root["serviceNumber"] = s.number;
  root["values"] = data;

  char *c = new char[root.measureLength() + 1];
  root.printTo((char*)c, root.measureLength() + 1);
  return(c);
}

char BaseProtocol_esp8266::nibble_to_char(int nibble){
  return nibble + 48 + 7*(nibble > 9);
}

bool BaseProtocol_esp8266::register_service(Service service){
  return false;
}
bool BaseProtocol_esp8266::register_device(){
  return false;
}
bool BaseProtocol_esp8266::register_data(Service s, char* value, int sensitive){
  return false;
}

byte BaseProtocol_esp8266::get_value_from_char(char hexa){
    return hexa - 48  - 7*(hexa>='A');

}
