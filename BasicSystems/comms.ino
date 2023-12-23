#include <RH_RF95.h>

#define RFM95_CS 10   // SX1278 chip select pin
#define RFM95_RST 9   // SX1278 reset pin
#define RFM95_INT 2   // SX1278 interrupt pin (change it according to your setup)

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  Serial.begin(9600);

  if (!rf95.init()) {
    Serial.println("LoRa initialization failed");
    while (1);
  }

  Serial.println("LoRa initialization successful!");

  // Setup ISM frequency
  rf95.setFrequency(433.0);

  // Set transmit power (range from 5 to 23, default is 13dBm)
  rf95.setTxPower(13);
}

void loop() {
  // Send a message
  const char* messageToSend = "Hello, Arduino!";
  rf95.send((uint8_t*)messageToSend, strlen(messageToSend));
  rf95.waitPacketSent();
  Serial.println("Message sent");

  // Wait for a response
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000)) {
    // Received a message
    if (rf95.recv(buf, &len)) {
      Serial.print("Received message: ");
      Serial.println((char*)buf);
    } else {
      Serial.println("Receive failed");
    }
  } else {
    Serial.println("No response received");
  }

  delay(5000); // Delay before sending the next message
}
