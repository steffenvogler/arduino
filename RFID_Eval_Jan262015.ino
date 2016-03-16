/*
  RFID Eval 13.56MHz Shield example sketch v10
  
  Aaron Weiss, aaron at sparkfun dot com
  OSHW license: http://freedomdefined.org/OSHW
  
  works with 13.56MHz MiFare 1k tags

  Based on hardware v13:
  D7 -> RFID RX
  D8 -> RFID TX
  D9 -> XBee TX
  D10 -> XBee RX
  
  Note: RFID Reset attached to D13 (aka status LED)
  
  Note: be sure include the NewSoftSerial lib, http://arduiniana.org/libraries/newsoftserial/
  
  Usage: Sketch prints 'Start' and waits for a tag. When a tag is in range, the shield reads the tag,
  blinks the 'Found' LED and prints the serial number of the tag to the serial port
  and the XBee port. 

*/
#include <NewSoftSerial.h>

NewSoftSerial rfid(7, 8);
NewSoftSerial xbee(10, 9);

//Prototypes
void check_for_notag(void);
void halt(void);
void parse(void);
void print_serial(void);
void read_serial(void);
void seek(void);
void set_flag(void);

//Global var
int flag = 0;
int Str1[11];

//INIT
void setup()  
{
  Serial.begin(9600);
  Serial.println("Start");
  
  // set the data rate for the NewSoftSerial ports
  xbee.begin(9600);
  rfid.begin(19200);
  delay(10);
  halt();
}

//MAIN
void loop()                 
{
  read_serial();
}

void check_for_notag()
{
  seek();
  delay(10);
  parse();
  set_flag();
  
  if(flag = 1){
    seek();
    delay(10);
    parse();
  }
}

void halt()
{
 //Halt tag
  rfid.print(255, BYTE);
  rfid.print(0, BYTE);
  rfid.print(1, BYTE);
  rfid.print(147, BYTE);
  rfid.print(148, BYTE);
}

void parse()
{
  while(rfid.available()){
    if(rfid.read() == 255){
      for(int i=1;i<11;i++){
        Str1[i]= rfid.read();
      }
    }
  }
}

void print_serial()
{
  if(flag == 1){
    //print to serial port
    Serial.print(Str1[8], HEX);
    Serial.print(Str1[7], HEX);
    Serial.print(Str1[6], HEX);
    Serial.print(Str1[5], HEX);
    Serial.println();
    //print to XBee module
    xbee.print(Str1[8], HEX);
    xbee.print(Str1[7], HEX);
    xbee.print(Str1[6], HEX);
    xbee.print(Str1[5], HEX);
    xbee.println();
    delay(100);
    //check_for_notag();
  }
}

void read_serial()
{
  seek();
  delay(10);
  parse();
  set_flag();
  print_serial();
  delay(100);
}

void seek()
{
  //search for RFID tag
  rfid.print(255, BYTE);
  rfid.print(0, BYTE);
  rfid.print(1, BYTE);
  rfid.print(130, BYTE);
  rfid.print(131, BYTE); 
  delay(10);
}

void set_flag()
{
  if(Str1[2] == 6){
    flag++;
  }
  if(Str1[2] == 2){
    flag = 0;
  }
}

