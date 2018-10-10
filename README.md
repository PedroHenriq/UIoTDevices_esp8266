# UIoT Devices_esp8266

This librarys aim is to enable users to create *Clients* and *Services* that follow UIoT's Ontology of sending and managing message flows in Esp8266 devices.

All the protocols included as a means of communication in this library follow the same principles:

 1. Create a *Device*
 2. Create a *Service*
 3. Send *Data*

<!-- The documentation of the Ontology followed by this library can be found at @@link-ontologia -->


The supported protocols by Esp8266 are displayed in the following table  

<table>
<tr>

    <th>Protocol</th>
    <th>Example</th>
  </tr>
  <tr>

  </tr>
  <tr>
    <td>HTTP</td>
      <td><a href="https://github.com/uiot/ESPDevices/tree/master/examples/httpExampleEsp8266">httpExampleEsp8266</a></td>
  </tr>
  </table>



# HTTP

  The HTTP library depends on the following libraries:

  <!-- * PubSubClient.h -->
  * FS.h
  * ESP8266HTTPClient.h
  * WiFiManager.h
  * ESP8266WiFi.h

  So, they must be installed in the users environment.

## Creating a Device

  A *Device* is an object of the UHttp class that represents the hardware whose means of communication protocol is the HTTP.

  To create a device using UHttp library you must create an instance of it, initialize it and, finally, set a server to send data to, as demonstrated below


  ```c++
  UHttp device;
  (...)
  device.init();
  device.set_server(SERVER_STRING);
  ```

  On the moment device.init is called, it creates a WifiManager instance. If the Esp doens't connect to a network automatticaly, it's possible to configure a wifi network through WifiManagers board. This is done by connecting to a wifi network whose SSID corresponds to the MAC address of the device.     


## Creating a Service

  A *Service* in an object of the Service class that represents a task witch a *Device* has.

  To create a service using UHttp library you must create an instance of it and set a value from the [*UHttp.create_service*](#create_service) method as demonstrated below

  ```c++
  Service service1;
  (...)
  device.create_service(number, name, unit, numeric, parameter);
  ```

## Sending Data

  To send data to the specified server, you must use the [*UHttp.send_data*](#send_data) method as demonstrated below

  ```c++
    device.send_data(service1, ptr , tam, 0);
  ```


## Methods

### set_service


```c++
void UHttp::set_server(const char *server)
```


| Parameter | Type | Description | Obligatory |
|-|-|-|-|
| server | char* | array of characters that contains the servers domain or IP and port, e.g. XXX.XXX.XXX.XXX:PORT | :heavy_check_mark:



### create_service

```c++
int BaseProtocol::create_service(int number, const char *name, String unit, bool numeric, String parameter)
```

| Parameter | Type | Description | Obligatory |
|-|-|-|-|
| number | int | Number to be attributed to the Service object that is beeing created | :heavy_check_mark:
| name | char* | Array of characters that mounts the name of the Service object that is beein created| :heavy_check_mark:
| unit | String | String that contais the unit of the value that is produced by the Service object that is beeing createed | :heavy_check_mark:
| numeric | bool | Value that indicates if the data produced by the service is of a numeric type or not | :heavy_check_mark:
| parameter | String | Value that indicates the parameters of the Service object that is beeing created | :heavy_check_mark:


### send_data

```c++
    bool BaseProtocol::send_data(Service service, float *data, int array_size, int sensitive=0)
```

| Parameter | Type | Description | Obligatory |
|-|-|-|-|
| service | Service | Service object with the *services* characteristics | :heavy_check_mark:
| data | float* | Array of float values corresponding to the data produced from the service| :heavy_check_mark:
| array_size | int | number of values in the data array | :heavy_check_mark:
| sensitive | int | value that indicates if the data is sensitive or not, that is, if the data must bypass the gateways buffer (default value is 0 - not sensitive)| :x:


```c++
    bool BaseProtocol::send_data(Service service, char *char_data, int sensitive=0)
```

| Parameter | Type | Description | Obligatory |
|-|-|-|-|
| service | Service | Service object with the *services* characteristics | :heavy_check_mark:
| char_data | float* | Array of characters with the value corresponding to the data produced from the service| :heavy_check_mark:
| sensitive | int | value that indicates if the data is sensitive or not, that is, if the data must bypass the gateways buffer (default value is 0 - not sensitive)| :x:

<!-- TODO# Serial -->
