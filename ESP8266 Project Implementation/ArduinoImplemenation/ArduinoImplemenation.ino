//Author Details.
//Name: Marvin Matata Mwendwa
//Admission no: 096990
//Supervisor: Daniel Machanje

//Configure the board manager and the port allocated to the device
//Board: Arduino Uno
//Port: COM4 (Depends on your computer)

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <stdlib.h>
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// some strings 
String myindex = "1"; // this number is incremented as we increase the number of GPS modules. 
// let's say if you are using 2 GPS modules, then we will use the same programming but will change the index number to 1, for third nodemcu module we change it to 2 and so on. 
String mylong = ""; // for storing the longittude value
String mylati = ""; // for storing the latitude value
String myvalue = "abc"; 

char buff[10];
 
                                                                                                                                                                                                                       
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin); // for gps

SoftwareSerial nodemcu(7, 8); // for gsm module


int led = 13; // tilt sensor output indication

String textForSMS;
 int limits = 4; // limit switch
 
 double longitude; 
 double latitude;
void setup()
{
  nodemcu.begin(9600);
  ss.begin(GPSBaud);
  Serial.begin(9600);
  Serial.println(" logging time completed!");
  randomSeed(analogRead(0));
  pinMode(limits, INPUT);
  digitalWrite(limits, HIGH);

  
    Serial.println(F("ESP8266Implementation.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); 
  Serial.println(TinyGPSPlus::libraryVersion());

  Serial.println();
}
 

 
void loop()
{

 
    // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
 // for the button 
//*****************************************************************
//****************************************************************
  if(digitalRead(limits) == HIGH)
  {
    displayInfo();
latitude = gps.location.lat(), 6 ;
longitude = gps.location.lng(), 6 ;
// for latitude
  mylati = dtostrf(latitude, 3, 6, buff);
  mylong = dtostrf(longitude, 3, 6, buff);
  
 textForSMS = textForSMS + myindex + "," + mylati + "," + mylong + "," + myvalue +","; 
// textForSMS = textForSMS + myindex + "," + "34.014623" + "," + "72.164979" + "," + myvalue +",";
  Serial.println(textForSMS);
  nodemcu.println(textForSMS); 
  textForSMS = ""; 
  delay(1000);
  }
  else
  digitalWrite(limits, HIGH);
   digitalWrite(led, LOW);
}


void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print(" ");
    Serial.print(F("Speed:"));
    Serial.print(gps.speed.kmph());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
