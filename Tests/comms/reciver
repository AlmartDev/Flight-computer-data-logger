#include <SPI.h>
#include <LoRa.h>
 
void setup() {
  Serial.begin(9600);
  while (!Serial);
 
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else
    Serial.println("done");
}
 
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.println("Received packet!");
    while (LoRa.available()) {
      String receivedData = LoRa.readString();
      Serial.println("Received: " + receivedData);
    }
 
    Serial.println("-------------");
  }
}
