#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
String lati=String("0.000000°");
String longi=String("0.000000");

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

int counter = 0;
void setup() 
{
  Serial.begin(9600);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while(1);
  }
  ss.begin(GPSBaud);
  
}
void loop() 
{ 
  if(Serial.available()>0){  
  while (ss.available() > 0)
    if (gps.encode(ss.read())){
      displayInfo();
      exit(0);
    }
  if (millis() > 10000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  }
}

void displayInfo() {
  while(1){
  Serial.print(F("Location: ")); 
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
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
  } else {
    Serial.print(F("INVALID"));
  }
  Serial.println();

 String latitude=String(gps.location.lat(), 6);
 String longitude=String(gps.location.lng(), 6);
  //int sVal=analogRead(A1);
  //float tempc = (sVal/1024.0)*500.0;
  
  if ((latitude.equals(lati)) && (longitude.equals(longi)))
  {
    Serial.println("Coordinates not availabe");
  }
  else {
  String msg = "TN 20 ES 1940; Latitude is " + latitude + " Longitude is" + longitude;
  
  //Serial.print("Sending message ");
  Serial.println(msg);
  
  LoRa.beginPacket();
  LoRa.print(msg);
  LoRa.endPacket();
  exit(0);
  }
  
  delay(5000);
  }
}
