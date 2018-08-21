//***************************************************************************************
// IoT Prototype to Acquire Information about Temperature, Humidity, Luminosity and Current
//
// Author:
//         Cassio Fabius Cambraia Ribeiro  - cassiofabius@gmail.com
//
//
// Date: June 14, 2017
//
// Arduino Mega 2560 + Ethernet Shield + ACS712 + DHT11 + Display Nokia 5510 + LDR
// ThingSpeak as IoT web plataform and ThinkView as Android app
// Sending all info via MQTT to UIoT Gateway and, then, to RAISE
//***************************************************************************************

// Libraries for DHT11, ethernet shield and display:

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <UMqtt_esp8266.h>
//#include "base_protocol.h"


// Ethernet parameters:
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xFA, 0x06 };
EthernetClient ethernet_client;

// MQTT parameters:
IPAddress server(192, 168, 0, 108);


void setup() {

  // Communication between user and Arduino via Serial:
  Serial.begin(9600);


  // Start the Ethernet connection:
//  Serial.println("Getting IP Address");
//  Ethernet.begin(mac);
//  delay (1000);

  // Print the local IP address, if connected:
  //print_ip_address();

  //Data to be send
//  String payload = payload_creator();
//  Serial.println("Payload:");
//  Serial.println(payload);
//  //const char *jsonCharTemp = payload.c_str();
//  Serial.print("Size: ");
//  Serial.println(payload.length() + 1);
//  char *jsonChar = new char[payload.length() + 1];
//  strcpy(jsonChar, payload.c_str());
//  Serial.print("JSONCHAR: ");
//  Serial.println(jsonChar);

  //Class-made Client
  UMqtt_esp8266 class_client(ethernet_client, server);
  Service service1 = cliente.create_service(1, "getTemp", "°C", true, "Temperatura");
  class_client.send_data(service1, (char*)"56");


//  Serial.println("Payload: ");
//  Serial.println(jsonChar);

  //Raw Client Manufacture
//  PubSubClient mqtt_client;
//  mqtt_client.setClient(ethernet_client);
//  mqtt_client.setServer(server, 1883);
//
//  connect_to_broker(mqtt_client, "RawPubSub");
//
//  Serial.println("Publishing Manufactured PubSubClient from Client RawPubSub");
//  mqtt_client.publish("teste/teste1/", jsonChar);
//  mqtt_client.disconnect();





  //PubSub Complete Client
//  PubSubClient cliente(server, 1883, ethernet_client);
//
//  connect_to_broker(cliente, "PubSubComplete");
//
//  cliente.publish("teste/teste1/", jsonChar);
//  cliente.disconnect();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("loop");
  delay(10000);

}


void print_ip_address (void) {
  Serial.print ("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print (Ethernet.localIP ()[thisByte], DEC);
    Serial.print (".");
  }

  Serial.println ();
}

//loop that assures connection to broker
//TODO = Better solution
void connect_to_broker(PubSubClient c, const char* client_name){
  while(!c.connected()){
  Serial.println("Connecting...");
  c.connect(client_name);

  if (c.connected()){
    Serial.println("Connected");
  }
  else{
    Serial.println("Not Connected");
  }
  delay(1500);
  }
}

String payload_creator(){
  StaticJsonBuffer<200> jsonBuffer;
  //DynamicJsonBuffer  jsonBuffer(200);
  JsonObject& root = jsonBuffer.createObject();
  JsonObject& root2 = jsonBuffer.createObject();
  root["oi"] = "olar";
  root2["teste"] = "Ü";
  root["tudo-e-vc"] = root2;

  String jsonChar;
  root.prettyPrintTo(jsonChar);

  return jsonChar;
}
