#include <RH_RF95.h>
 
#define RFM95_CS 10   // SX1278 chip select pin
#define RFM95_RST 9   // SX1278 reset pin
#define RFM95_INT 2   // SX1278 interrupt pin (change it according to your setup)
 
#include <Adafruit_BMP085.h>
#include <Wire.h>
 
 
 
float startAltitude, altitude;
float startPressure;
 
Adafruit_BMP085 barometer;
 
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
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
 
void setup() {
  Serial.begin(9600);
 
  if (!rf95.init()) {
    Serial.println("LoRa initialization failed");
    while (1);
  }
 
  initBarometric();
 
 
  Serial.println("LoRa initialization successful!");
 
  // Setup ISM frequency
  rf95.setFrequency(433.0);
 
  // Set transmit power (range from 5 to 23, default is 13dBm)
  rf95.setTxPower(13);
}
 
void loop() {
 
  altitude = barometer.readAltitude(startPressure);  
 
  Serial.println(altitude - startAltitude);
  //Serial.println (" - t:");
  //Serial.print(temperature);
 
  float realAltitude = altitude - startAltitude;
 
  // Send a message
  char str[32];
  dtostrf(realAltitude, 8, 2, str);
 
  const char* messageToSend = str;
  rf95.send((uint8_t*)messageToSend, strlen(messageToSend));
  rf95.waitPacketSent();
  Serial.println("Message sent");
  Serial.println(messageToSend);
 
 
  delay(100); // Delay before sending the next message
}
