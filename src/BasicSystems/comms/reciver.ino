#include <LoRa.h>
 
#define LORA_SS 10
#define LORA_RST 9
#define LORA_DIO0 2
 
void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.print("Initializing LoRa receiver  -");

  if (!LoRa.begin(433E6)) { 
    Serial.println("ERROR");
    while (1);
  }
 
  Serial.println("done.");
}
 
void loop() {
  int packetSize = LoRa.parsePacket();
 
  if (packetSize) {
    Serial.print("->  ");
 
    // Read the message
    while (LoRa.available()) {
      Serial.println((char)LoRa.read());
    }
 
    Serial.println();
  }
}
