#include <UHttp_esp8266.h>

UHttp_esp8266 cliente;
int service1;
int tam = 10;

void setup(){
  Serial.begin(115200);
//  UHttp_esp8266 cliente("http://192.168.0.105:5000");
//  Service service1 = cliente.create_service(1, "getTemp", "*C", true, "Temperatura");
//  cliente.send_data(service1, (char*)"56", 0);
  while (!Serial);
  delay(1000);

  Serial.println("ue");
  cliente.init();
  Serial.println("ue2");
  cliente.set_server("http://172.16.9.145:8000");
  service1 = cliente.create_service(1, "getTemp", "*C", true, "Temperatura");
}

void loop(){
  delay(5000);
  float* ptr = gerador_array(tam);
  cliente.send_data(service1, ptr , tam, 0);
  Serial.println("loop");
}


float* gerador_array(int tamanho){
  float *values;
  values = (float*)malloc(tamanho*sizeof(float));
  for(int i = 0; i < tamanho; i++){
    values[i] = random(2,100) + random(0,999)/1000;
  }
  Serial.print("Random Values:");
  for(int i = 0; i < tamanho; i++){
    Serial.print(values[i]);
    Serial.print(" - ");
  }
  return values;
}
