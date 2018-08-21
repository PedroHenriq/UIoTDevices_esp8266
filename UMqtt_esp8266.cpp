#include "UMqtt_esp8266.h"

UMqtt_esp8266::UMqtt_esp8266(Client& client, IPAddress server) {
  this->device_identificator();

 Serial.println("Getting IP Address");
 Ethernet.begin(this->mac_byte);
 //byte m[] = { 0x9A, 'I', 'O', 'T', 0xFB, 0x06 };
 //Ethernet.begin(m);
 Serial.print ("My IP address: ");
 for (byte thisByte = 0; thisByte < 4; thisByte++) {
   // print the value of each byte of the IP address:
   Serial.print (Ethernet.localIP ()[thisByte], DEC);
   Serial.print (".");
 }

 Serial.println ();

  delay (1000);

	this->mqtt_client.setClient(client);
  this->mqtt_client.setServer(server, this->mqtt_port);
}

bool UMqtt_esp8266::register_device(){
    char *data;
    data = this->make_client_data();
    // data = "oicara{'tudo': 1}";
    this->publish("Register/Device", data);
 // root.printTo((char*)c, root.measureLength() + 1);
 // Serial.println(c);
 //  this->json_to_char(root);
 //  Serial.println(this->json_to_char(root));
 // return this->publish("Register/Device", this->json_to_char(root));
  return false;
}


bool UMqtt_esp8266::register_service(Service service){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["tags"] = service.tags;
  root["number"] = service.number;
  root["chipset"] = this->chipset;
  root["mac"] = this->mac;
  root["name"] = service.name;
  root["unit"] = service.unit;
  root["numeric"] = service.numeric;

  return this->publish("Register/Service", json_to_char(root));

  return true;
}
bool UMqtt_esp8266::register_data(char  *data){
	data = this->add_mac_chipset(data);
	return this->publish("Data/", data);
}

bool UMqtt_esp8266::publish(const char *topic, char *data) {
     // char *c = new char[data.length() + 1];
     // char c[15];
     // strcpy(c, data.c_str());
     // data.toCharArray(c, data.length() + 1);
    Serial.print("Sending Data: ");
    Serial.println(data);
    this->mqtt_client.connect("ClassClient");

    Serial.println(this->mqtt_client.publish(topic, data));
    this->mqtt_client.disconnect();
    return true;
    // while(!this->mqtt_client.connected()){
    //   Serial.println("Connecting...");
    //   this->mqtt_client.connect("ArduinoClient");
    //
    //   if (this->mqtt_client.connected()){
    //     Serial.println("Connected");
		// 	}
    //   else{
		// 		Serial.println("Not Connected");
		// 		return false;
		// 	}
  	// }
}

char* UMqtt_esp8266::add_mac_chipset(char *data){
  return data;
}

char* UMqtt_esp8266::json_to_char(JsonObject& root){
  String payload;
  root.prettyPrintTo(payload);
  Serial.println("Payload:");
//  Serial.println(payload);

  char *jsonChar = new char[payload.length() + 1];
  strcpy(jsonChar, payload.c_str());
  Serial.println("JSONCHAR:");
//  Serial.println(jsonChar);

  return jsonChar;
}
