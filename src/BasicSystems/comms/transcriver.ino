#include <LoRa.h>
 
#define LORA_SS 10
#define LORA_RST 9
#define LORA_DIO0 2
 
void setup() {
  Serial.begin(9600);
  while (!Serial);
 
  // Initialize LoRa module
  if (!LoRa.begin(433E6)) { // Change the frequency to match your LoRa module's frequency
    Serial.println("LoRa initialization failed. Check your wiring!");
    while (1);
  }
 
  Serial.println("LoRa Transceiver Initialized");
}
 
void loop() {
  String message = "Hello, LoRa!";
 
  // Send message
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
 
  Serial.println("Message sent: " + message);
 
  // Wait for some time before sending the next message
  delay(5000);
 
  // Check for incoming messages
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.println("Received a message:");
 
    // Read the message
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
 
    Serial.println();
  }
}
