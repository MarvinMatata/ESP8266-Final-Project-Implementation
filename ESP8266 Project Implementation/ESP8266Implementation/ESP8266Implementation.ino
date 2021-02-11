//Author Details.
//Name: Marvin Matata Mwendwa
//Admission no: 096990
//Supervisor: Daniel Machanje

//Configure the board manager and the port allocated to the device
//Board: NodeMCU
//Port: COM7 (Depends on your computer)#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

WidgetMap myMap(V0);

char auth[] = "U8btnZgjTHNgrS7ANO2pYK-Gm-H4qzgV"; // Authorization token
//Example of WiFi configurations.

//Strathmore Wifi configureation
const char *ssid =  "strathmore";    
const char *pass =  "5trathm0re"; 

//const char *ssid =  "DAM ESTATE 70";
//const char *pass =  "Mengere@123";

//Home configuration
//onst char *ssid =  "JTL Faiba";
//const char *pass =  "Matata@345";

SimpleTimer timer;

String myString; // complete message from Arduino, which consists of snesors data
char rdata; // received characters
String gpsdata = "";

int myindex;
float mylat;
float mylon;
String myvalue;
double fahad;  

int firstVal, secondVal, thirdVal; // sensors
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);

}



void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, sensorvalue1);

}

void loop()
{
  if (Serial.available() == 0 )
  {
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
  }

  if (Serial.available() > 0 )
  {
    rdata = Serial.read();
    myString = myString + rdata;
    // Serial.print(rdata);
    if ( rdata == '\n')
    {
      gpsdata = myString;
      String k = getValue(myString, ',', 0); // index
      String m = getValue(myString, ',', 1); // lati
      String n = getValue(myString, ',', 2); // longi
      String o = getValue(myString, ',', 3); // Value

      myindex = k.toInt();
      mylat = m.toFloat();
      mylon = n.toFloat();
      myvalue = o;

      Serial.println(myindex);
      Serial.println(mylat,5); 
      Serial.println(mylon,5); 
 
      Serial.println(myvalue);

      myString = "";
      // end new code
    }
  }

}

void sensorvalue1()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  // Blynk.virtualWrite(V2, gpsdata);
  myMap.location( myindex, mylat, mylon, myvalue);

}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
