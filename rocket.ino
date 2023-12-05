// Code made explicitly for Arduino Nano in the Rocket's flight computer

#include <RH_RF95.h>

RH_RF95 driver;

void initComms() {
    Serial.println("Initializing LoRa comms");
    if (!driver.init()) {
        Serial.println("ERROR");
        // ShutDown();
    }
    Serial.println("done.");
    driver.setFrequency(915.0);

    sendData("LoRa Comms inited");
}

void sendData(const char data = "No data.") {
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
  // Examples

  // sendData(temperature);

  //    if (recieveData() == "parachute") {
  //        deployParachute();
  //    }

  delay(500);
}