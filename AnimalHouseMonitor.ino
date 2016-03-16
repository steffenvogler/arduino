//#include <WiFi.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Phant.h>
#include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

//char ssid[] = "DZNE-Mac";     //  your network SSID (name)
//char pass[] = "deftones";    // your network password
//int status = WL_IDLE_STATUS;     // the Wifi radio's status
//WiFiClient client;
byte mac[] = {0x40, 0x6C, 0x8f, 0x4b, 0xab, 0x3b};
EthernetClient client;
unsigned long i=1;
IPAddress ip;
IPAddress subnet;
IPAddress gateway;
//WiFiClient client;

//byte mac[6];

IPAddress server(192,168,179,68);
char host[] = "192.168.179.68";
char url[] = "/arduino_push_data2.php";
char key[] = "test";
char c;

float t = 0;
float h = 0;
int li = 0;
//int temp = 0;   // Analog-Wert
const int sensorPin = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to network...");
  if(Ethernet.begin(mac) == 0) {
    Serial.println("Failed");
    while(true);
  }
  delay(1000);
  Serial.print("This IP adress: ");
  IPAddress myIPAddress = Ethernet.localIP();
  Serial.print(myIPAddress);
  /*status = WiFi.begin(ssid, pass);
  delay(5000); 
 Serial.println("connecting to Wifi...");
 if ( status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    // don't do anything else:
    while(true);
  }
  else {
    Serial.println("Connected to wifi");
    Wifi_Data();
  }
  */
}
 void loop(){
    Serial.print(" Iteration ");
 Serial.println(i);
//delay(2000); //every 2 seconds
delay(1800000);//after 30 minute
//delay(1000);
  i=i+1;
  Sensor_lesen();
  //Daten_sendenWiFi();
  Daten_senden();
  
  
  if (client.available())
{
char c = client.read();
Serial.print(c);
}
delay(5000); //Time till next Data push
client.stop();
client.flush();
}

/*void Wifi_Data()
{
  ip = WiFi.localIP();
     subnet = WiFi.subnetMask();
     gateway = WiFi.gatewayIP();
    Serial.print("NETMASK: ");Serial.print(subnet);Serial.println();
    Serial.print("IP Address: "); Serial.print(ip); Serial.println(); 
    Serial.print("Gateway: ");Serial.print(gateway);Serial.println();
    WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}*/

void Sensor_lesen()
{
 li = analogRead(sensorPin); 
 h = dht.readHumidity();
 t = dht.readTemperature();
 
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(li)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  //float hi = dht.computeHeatIndex(f, h);

  Serial.print("Light-Intensity (a.u.): "); 
  Serial.print(li);
  Serial.print(" Humidity (%): "); 
  Serial.print(h);
  Serial.print(" Temperature (C): "); 
  Serial.println(t);
}
  
void Daten_sendenWiFi()
{
if (client.connect(server, 80))   // Verbindung zum Server aufbauen
{
Serial.print("Verbunden…sende Daten…");
client.print("GET ");
client.print(url);
client.print("?TEMP=");
client.print(i);
client.print("&key=");
client.print(key);
//client.println(" HTTP/1.1");
//client.print("Host: ");
//client.println(host);
client.println();
Serial.println("fertig!");
}
else
{
Serial.println(" could not connect to server");
}
}

void Daten_senden()
{
if (client.connect(server, 80)>0){
Serial.print("Verbunden…sende Daten…");
client.print("GET ");
client.print(url);
client.print("?TEMP=");
client.print(t);
client.print("&key=");
client.print(key);
client.print("&HUMID=");
client.print(h);
client.print("&LI=");
client.print(li);
//client.println(" HTTP/1.1");
//client.print("Host: ");
//client.println(host);
client.println();
Serial.println("fertig!");
}
else
{
Serial.println(" could not connect to server");
}
}
  
