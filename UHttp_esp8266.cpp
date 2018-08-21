#include "UHttp_esp8266.h"

UHttp_esp8266::UHttp_esp8266(String server){
  this->device_identificator();

  this->server = server;
  WiFi.begin("UIOT_FOR_TESTS", "1234Cinco");
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    Serial.println("Connecting to Wifi");
    delay(2500);
 }
 Serial.println("Connection Successfull");
}


bool UHttp_esp8266::register_device(){
  char *data;
  data = this->make_client_data();
  this->http.begin(this->server + "/client");
  this->http.addHeader("Content-Type", "application/json");
  Serial.println(this->http.POST(data));
  // data = "oicara{'tudo': 1}";
  // this->publish("Register/Device", data);
  this->http.end();
  return false;
}
bool UHttp_esp8266::register_service(Service s){
  char *service;
  service = this->make_service_data(s);

  this->http.begin(this->server + "/service");
  this->http.addHeader("Content-Type", "application/json");
  Serial.println(service);
  Serial.println(this->http.POST(service));

  this->http.end();
  return false;
}
bool UHttp_esp8266::register_data(Service s, char* value, int sensitive){
  char *data;
  data = this->make_raw_data(s, value, sensitive);

  this->http.begin(this->server + "/data");
  this->http.addHeader("Content-Type", "application/json");
  Serial.println(data);
  Serial.println(this->http.POST(data));

  return false;
}