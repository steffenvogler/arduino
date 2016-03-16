//#include <WiFi.h>
#include <Ethernet.h>
#include <SPI.h>

#include <Wire.h> //I2C needed for sensors
#include "MPL3115A2.h" //Pressure sensor
#include "HTU21D.h" //Humidity sensor

MPL3115A2 myPressure; //Create an instance of the pressure sensor
HTU21D myHumidity; //Create an instance of the humidity sensor

//////////// COMMUNICATION//////////

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

const byte REFERENCE_3V3 = A3;
const byte LIGHT = A1;
const byte STAT1 = 7;
const byte STAT2 = 8;

float t = 0;
float h = 0;
float li = 0;
float p=0;

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
  pinMode(STAT1, OUTPUT); //Status LED Blue
  pinMode(STAT2, OUTPUT); //Status LED Green
  pinMode(LIGHT, INPUT);
  pinMode(REFERENCE_3V3, INPUT);
  
  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags
  
  //Configure the humidity sensor
  myHumidity.begin();
  
  Serial.println("Weather Shield online!");
  
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
 digitalWrite(STAT1, LOW); //Turn off stat LED
 delay(2000); //every 2 seconds
//delay(1800000);//after 30 minute
//delay(1000);
  i=i+1;
  digitalWrite(STAT1, HIGH); //Blink stat LED
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
 li = get_light_level();
 h = myHumidity.readHumidity();
 t = myPressure.readTemp();
 p = myPressure.readPressure();
 
  
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
  Serial.print(" Pressure (Pa): "); 
  Serial.println(p);
}

float get_light_level()

{
	float operatingVoltage = analogRead(REFERENCE_3V3);
	float lightSensor = analogRead(LIGHT);
	operatingVoltage = 3.3 / operatingVoltage; //The reference voltage is 3.3V
	lightSensor = operatingVoltage * lightSensor;
	return(lightSensor);
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
  
