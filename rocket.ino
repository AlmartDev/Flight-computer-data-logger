// Code made explicitly for Arduino Nano in the Rocket's flight computer

#include <RH_RF95.h>

#include <Servo.h>

#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int startAltitude, altitude;
int startPressure;
int temperature;

RH_RF95 driver;
Servo servo1, servo2;
Adafruit_BMP085 barometer;

// Inits -----------------------------

void initComms() {
    Serial.println("Initializing LoRa comms ----------------");
    if (!driver.init()) {
        Serial.println("ERROR");
        // ShutDown();
    }
    Serial.println("done.");
    driver.setFrequency(915.0);

    sendData("LoRa Comms inited ----------------");
}

void initServos() {
    sendData("Initializing Servos");
    // Serial.println("Initializing Servos");

    servo1.attach(A0);
    servo2.attach(A1);

    sendData("done.");
    // Serial.println("done.");
}

void initBarometric() {
    sendData("Initializing Barometric sensors");
    // Serial.println("Initializing Barometric sensors");

    if (!barometer.begin()) {
        sendData("ERROR");
        // Serial.println("ERROR");
        while (1) {}
    }

    sendData("done.");
    // Serial.println("done.");

    // Read presure and calculate altitude at start
    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

// Inits -----------------------------

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

// Updates -----------------------------

void updateBarometric() {
    temperature = barometer.readTemperature();

    // altitude calculation
    //                     Actual altitude (sea level)      Start Altitude (sea level)
    float altitude = barometer.readAltitude(startPressure) - startAltitude;     // this will give the change in the altitude

    // Serial.println(altitude);

    sendData(temperature);
    sendData(altitude);
}

// Updates -----------------------------


void setup() {
  Serial.begin(9600); // only used for debugging

  initComms();          // Communication
  initServos();         // Servos
  initBarometric();     // Barometric sensor/temperature
}

void loop() {
    // Updates
    updateBarometric();

    // Examples
    // sendData(temperature);

    //    if (recieveData() == "parachute") {
    //        deployParachute();
    //    }

    delay(500);
}