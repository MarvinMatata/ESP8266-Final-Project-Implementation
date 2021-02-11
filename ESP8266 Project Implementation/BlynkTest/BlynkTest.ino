//Author Details.
//Name: Marvin Matata Mwendwa
//Admission no: 096990
//Supervisor: Daniel Machanje

//Configure the board manager and the port allocated to the device
//Board: NodeMCU
//Port: COM7 (Depends on your computer)

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS 
//TX - Tranfer
//RX - Reciever

static const int RXPin = 4, TXPin = 5;   
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800
const char server[] = "blynk-cloud.com";


TinyGPSPlus gps; // The TinyGPS++ object
WidgetMap myMap(V0);  // V0 for virtual pin of Map Widget

SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

BlynkTimer timer;

float speed;       //Variable  to store the speed
float satellites;      //Variable to store no. of satellites response
String direction;  //Variable to store orientation or direction of GPS

//Example of WiFi configurations.

//Strathmore Wifi configureation
const char *ssid =  "strathmore";    
const char *pass =  "5trathm0re"; 

//const char *ssid =  "DAM ESTATE 70";
//const char *pass =  "Mengere@123";

//Home configuration
//onst char *ssid =  "JTL Faiba";
//const char *pass =  "Matata@345";

char auth[] = "U8btnZgjTHNgrS7ANO2pYK-Gm-H4qzgV"; //

//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
  

void setup()
{
  Serial.begin(9600);
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V6, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
  else{
    Serial.println(F("Working okay."));
      Blynk.virtualWrite(V6, "GPS working");
  }
}

void loop()
{
    while (ss.available() > 0) 
    {
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read())){
        displayInfo();
     }
  }
  Blynk.run();
  timer.run();
}

void displayInfo()
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));
    
     myMap.location(move_index, latitude, longitude, "GPS_Location");
     
     speed = gps.speed.kmph();               //get speed
     Blynk.virtualWrite(V3, speed);
     
     satellites = gps.satellites.value();    //get number of satellites
     Blynk.virtualWrite(V4, satellites);
    
     direction = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
     Blynk.virtualWrite(V5, direction);                   
  }
  
 Serial.println();
}

//fleet location tracking system
