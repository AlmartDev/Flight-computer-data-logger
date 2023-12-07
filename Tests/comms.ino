// code based for the Arduino Nano/Uno and an SX1278, using frequency 915.0

#include <RH_RF95.h>

RH_RF95 driver;

void initComms() {
    Serial.println("Initializing LoRa comms");
    if (!driver.init()) {
        Serial.println("ERROR");
    }
    else
        Serial.println("done.");

    driver.setFrequency(915.0);
}

void sendData(const char data) {
    driver.send((uint8_t *)data, strlen(data));
    driver.waitPacketSent();
}

void recieveData() {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (driver.waitAvailableTimeout(1000)) {
        if (driver.recv(buf, &buflen)) {
            buf[buflen] = '\0';
            return (char *)buf;
        }
    } 
}

void setup() {
  Serial.begin(9600);
  initComms();
}

void loop() {
  Serial.println(recieveData());
  Serial.println(" ");

  sendData("Hello, comms test in ./test/comms.ino")

  delay(500);
}