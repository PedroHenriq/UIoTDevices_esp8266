#include "UHttp_esp8266.h"

UHttp_esp8266::UHttp_esp8266(){
 //  this->device_identificator();
 //
 //  this->server = server;
 //  WiFi.begin("UIOT_FOR_TESTS", "1234Cinco");
 //  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 //    Serial.println("Connecting to Wifi");
 //    delay(2500);
 // }
 // Serial.println("Connection Successfull");
}

UHttp_esp8266::UHttp_esp8266(String server){
  this->device_identificator();

  this->server = server;
  WiFi.begin("UIOT", "A12818UIOT");
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    Serial.println("Connecting to Wifi");
    delay(2500);
 }
 Serial.println("Connection Successfull");
}

void UHttp_esp8266::init(){
  delay(1000);
  this->device_identificator();
  WiFiManager wifimanager;
  wifimanager.autoConnect(this->mac.c_str());
 //  WiFi.begin("UIOT", "A12818UIOT");
 //  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 //    Serial.println("Connecting to Wifi");
 //    delay(2500);
 // }
 Serial.println("Connection Successfull");
}


void UHttp_esp8266::set_server(String server){
  Serial.println("set_server");
  this->server = server;
}

bool UHttp_esp8266::register_device(){
  char *data = NULL;
  data = this->make_client_data(data);
  this->http.begin(this->server + "/client");
  this->http.addHeader("Content-Type", "application/json");
  int code = this->http.POST(data);
  String payload = this->http.getString();
  Serial.println(data);
  Serial.println(code);
  Serial.println(payload);
  free(data);
  this->http.end();
  return (code==200);
}
bool UHttp_esp8266::register_service(int s){
  char *data = NULL;
  // data = this->make_service_data(s, data);
  data = this->make_service_data(this->service[s], data);

  this->http.begin(this->server + "/service");
  this->http.addHeader("Content-Type", "application/json");
  int code = this->http.POST(data);
  String payload = this->http.getString();
  Serial.println(code);
  Serial.println(payload);
  free(data);
  this->http.end();
  return (code==200);
}
bool UHttp_esp8266::register_data(int s, char* value, int sensitive){
  char *data = NULL;
  // data = this->make_raw_data(s, value, sensitive, data);
  data = this->make_raw_data(this->service[s], value, sensitive, data);
  this->http.begin(this->server + "/data");
  this->http.addHeader("Content-Type", "application/json");
  int code = this->http.POST(data);
  String payload = this->http.getString();
  Serial.println(code);
  Serial.println(payload);
  free(data);
  this->http.end();
  return (code==200);
}
