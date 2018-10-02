#include "BaseProtocol_esp8266.h"

int BaseProtocol_esp8266::create_service(int number, const char *name, String unit, bool numeric, String parameter){
  this->service[number] = Service(number, name, unit, numeric, parameter);
  return this->service[number].number;
  // return Service(number, name, unit, numeric, parameter);
}

bool BaseProtocol_esp8266::send_data(int service, float *data, int array_size, int sensitive=0) {
  char * char_data = float_to_char(data, array_size);
	this->DEVICE_REGISTERED = (!this->DEVICE_REGISTERED)? this->register_all(service, char_data, sensitive) : this->register_service_data(service, char_data, sensitive);
  free(char_data);
  return this->DEVICE_REGISTERED;
}

bool BaseProtocol_esp8266::send_data(int service, char *char_data, int sensitive=0) {
	this->DEVICE_REGISTERED = (!this->DEVICE_REGISTERED)? this->register_all(service, char_data, sensitive) : this->register_service_data(service, char_data, sensitive);
  return this->DEVICE_REGISTERED;
}

void BaseProtocol_esp8266::device_identificator(){
 Serial.println("Indenticator");
 int address = 0;
 int bytes[8];
 bool result = SPIFFS.begin();
 Serial.println("SPIFFS opened: " + result);

  File f = SPIFFS.open("/UIOT.txt", "r");

 if (!f){
    Serial.println("nao existe arquivo");
    Serial.println("Criando arquivo");
    File f = SPIFFS.open("/UIOT.txt", "w");

    int iterator = 0xFF;
    for (int i = 0; i < 8; i++){
      int a = millis();
      int b = rand() % 123 + 1;
      int unique = a * b;
      bytes[i] = unique & iterator;
    }


      this->mac_byte[4] = bytes[0];
      this->mac_byte[5] = bytes[1];

      for (int i = 0; i < 6; i++){
        this->chipset_byte[i] = bytes[i+2];
      }

      for(int i = 0; i < 6; i++){
        byte b = this->chipset_byte[i];
        this->chipset += this->get_value_from_char((b & 0xF0)>>4);
        this->chipset += this->get_value_from_char(b & 0xF);
        this->chipset += ":";
      }
      this->chipset.remove(this->chipset.length()-1,1);

      for(int i = 0; i < 6; i++){
        byte b = this->mac_byte[i];
        this->mac += this->get_value_from_char((b & 0xF0)>>4);
        this->mac += this->get_value_from_char(b & 0xF);
        this->mac += ":";
      }
      this->mac.remove(this->mac.length()-1,1);

      f.println(this->mac);
      f.println(this->chipset);

      while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('\n');
      Serial.println(this->mac);
      Serial.println(this->chipset);
    }


      // EEPROM.write(i + 2,bytes[i]);
      //unique = unique >> 8;
    // }
  } else{


    Serial.println("ELSE");
    this->mac = f.readStringUntil('\r');
    f.readStringUntil('\n');
    this->chipset = f.readStringUntil('\r');
    this->mac_byte[4] = this->get_value_from_char(this->mac.charAt(12))*0x10;
    this->mac_byte[4] += this->get_value_from_char(this->mac.charAt(13));

    this->mac_byte[5] = this->get_value_from_char(this->mac.charAt(15))*0x10;
    this->mac_byte[5] += this->get_value_from_char(this->mac.charAt(16));

    Serial.println(this->mac_byte[4]);
    Serial.println(this->mac_byte[5]);


  }
  f.close();






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


bool BaseProtocol_esp8266::register_all(int service, char *data, int sensitive){
	return this->register_device() && this->register_service_data(service, data, sensitive);
}

bool BaseProtocol_esp8266::register_service_data(int service,  char* data, int sensitive){
  Serial.println(this->service[service].registered);
  if (this->service[service].registered){
    if(this->register_data(service, data, sensitive)){
      return true;
    }
    else{
      this->service[service].registered = false;
      return false;
    }
  }
  else{
    Serial.println("Else");
    if(this->register_service(service) && this->register_data(service, data, sensitive)){
      Serial.println("AKI");
      Serial.println(this->service[0].registered);
      this->service[service].registered = true;
      Serial.println(this->service[service].registered);
      return true;
    }
    else{
      Serial.println("OIE");
      this->service[service].registered = false;
      return false;
    }
  }
}


char *BaseProtocol_esp8266::make_client_data(char* json){
  // Serial.println("Registering Device");

  // Serial.println(strlen(json));
  json = (char*)malloc(2*sizeof(char));
  json[0] = '{';
  json[1] = '\0';
  json = this->append_json(json, "name", this->name.c_str(),0);
  json = this->append_json(json, "chipset", this->chipset.c_str(),0);
  json = this->append_json(json, "mac", this->mac.c_str(),0);
  json = this->append_json(json, "serial", this->serial.c_str(),0);
  json = this->append_json(json, "processor", this->processor.c_str(),0);
  json = this->append_json(json, "channel", this->channel.c_str(),0);
  json[strlen(json)-1] = '}';
  // Serial.println(json);
  return json;
}


char *BaseProtocol_esp8266::make_service_data(Service service, char* json){
  // Serial.println("Registering Service");

  // Serial.println(strlen(json));
  json = (char*)malloc(2*sizeof(char));
  json[0] = '{';
  json[1] = '\0';
  json  = this->append_json(json, "name", service.name,0);
  // Serial.println(json);
  // delay(1000);
  json = this->append_json(json, "chipset", this->chipset.c_str(),0);
  // Serial.println(json);
  // delay(1000);
  json = this->append_json(json, "mac", this->mac.c_str(),0);
  // Serial.println(json);
  // delay(1000);
  json = this->append_json(json, "parameter", service.parameter.c_str(),0);
  // Serial.println(json);
  // delay(1000);
  json = this->append_json(json, "number", String(service.number).c_str(), 1);
  // Serial.println(json);
  // delay(1000);
  json = this->append_json(json, "unit", service.unit.c_str(),0);
  // Serial.println(json);
  // delay(1000);
  json = this->append_json(json, "numeric", String(service.numeric).c_str(),1);
  json[strlen(json)-1] = '}';
  // Serial.println(json);
  // delay(1000);
  return json;
}

char *BaseProtocol_esp8266::make_raw_data(Service s, char *data, int sensitive, char* json){
  // Serial.println("Raw Data");
  // Serial.println(strlen(json));
  json = (char*)malloc(2*sizeof(char));
  json[0] = '{';
  json[1] = '\0';
  json = this->append_json(json, "chipset", this->chipset.c_str(), 0);
  json = this->append_json(json, "mac", this->mac.c_str(), 0);
  json = this->append_json(json, "sensitive", String(sensitive).c_str(),1);
  json = this->append_json(json, "serviceNumber", String(s.number).c_str(),1);
  json = this->append_json(json, "values", data, s.numeric);
  json[strlen(json)-1] = '}';
  // Serial.println(json);
  return json;
}


char* BaseProtocol_esp8266::append_json(char *json, const char* key, const char* value, int numeric){
  // Serial.println(strlen(json));
  if(key == "values" && !numeric){
    json = (char*) realloc (json, (strlen(key) + strlen(value) + 9 + strlen(json)) * sizeof(char)); // 9 because "" and : and ,
    strcat(json, "\"");
    strcat(json, key);
    strcat(json, "\":[\"");
    strcat(json, value);
    strcat(json, "\"],");
    json[strlen(json)] = '\0';
  }else if(key == "values" && numeric){
    json = (char*) realloc (json, (strlen(key) + strlen(value) + 7 + strlen(json)) * sizeof(char)); // 5 because "" and : and ,
    strcat(json, "\"");
    strcat(json, key);
    strcat(json, "\":[");
    strcat(json, value);
    strcat(json, "],");
    json[strlen(json)] = '\0';
  }else if(key != "values" && !numeric){
    json = (char*) realloc (json, (strlen(key) + strlen(value) + 7 + strlen(json)) * sizeof(char)); // 5 because "" and : and ,
    strcat(json, "\"");
    strcat(json, key);
    strcat(json, "\":\"");
    strcat(json, value);
    strcat(json, "\",");
    json[strlen(json)] = '\0';
  }else if(key != "values" && numeric){
    json = (char*) realloc (json, (strlen(key) + strlen(value) + 5 + strlen(json)) * sizeof(char)); // 5 because "" and : and ,
    strcat(json, "\"");
    strcat(json, key);
    strcat(json, "\":");
    strcat(json, value);
    strcat(json, ",");
    json[strlen(json)] = '\0';
  }
  return json;
}

char* BaseProtocol_esp8266::float_to_char(float* float_array, int array_size){
  char *values;
  values = (char*)malloc(1*sizeof(char));
  // values[0] = '[';
  values[0] = '\0';
  String b;
  int contador = 1;
  for(int i = 0; i < array_size; i++){
    // Serial.println(dtostrf(float_array[i], 10, 3, b));
    b = String(float_array[i]);
    contador += b.length() + 1;
    values = (char*) realloc (values, (contador ) * sizeof(char));
    strcat(values, b.c_str());
    strcat(values, ",");
    // Serial.print("values: ");
    // Serial.println(values);
  }
  values[contador-2] = '\0';

  // strcat(values, "]");

  return values;
}

char BaseProtocol_esp8266::nibble_to_char(int nibble){
  return nibble + 48 + 7*(nibble > 9); //ascii based
}


byte BaseProtocol_esp8266::get_value_from_char(char hexa){
    return hexa - 48  - 7*(hexa>='A');

}
bool BaseProtocol_esp8266::register_service(int service){
  return false;
}
bool BaseProtocol_esp8266::register_device(){
  return false;
}
bool BaseProtocol_esp8266::register_data(int svc, char* dt, int stv){
  return false;
}
