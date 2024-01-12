#include <LoRa.h>
 
#include <Adafruit_BMP085.h>
#include <Wire.h>
 
 
 
float startAltitude, altitude;
float startPressure;
 
Adafruit_BMP085 barometer;
  
void initBarometric() {
    Serial.println("Initializing Barometric sensors");
 
    if (!barometer.begin()) {
        Serial.println("ERROR");
        while (1) {}
    }
    else
        Serial.println("done.");
 
    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

void initCommms()
{
    Serial.print("Initializing LoRa comms  -  ");
    //SendData("Initializing LoRa comms  -  ");

    if (!LoRa.begin(915E6))  // initialize ratio at 915 MHz
    { 
        Serial.println("ERROR");
        //SendData("ERROR");
        while (true)
            ;
    }
    Serial.println("done.");
    //SendData("done.");
}
 
void setup() {
  Serial.begin(9600);
 
  initCommms();
  initBarometric();
}

template <typename remoteData>
void SendData(remoteData data)  // sends data to ground station, all data will be send in a new line
{
    LoRa.beginPacket();
    LoRa.print(String(data));
    LoRa.endPacket();
}
 
void loop() {
 
  altitude = barometer.readAltitude(startPressure);  
 
  Serial.println(altitude - startAltitude);
  //Serial.println (" - t:");
  //Serial.print(temperature);
 
  float realAltitude = altitude - startAltitude;
  
  SendData("Testttt!!!!");
  
  delay(1000); // Delay before sending the next message
}
