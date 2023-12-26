// Code made explicitly for Arduino Nano in the Rocket's flight computer

#include <RH_RF95.h>

#include <Servo.h>

#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <SD.h>

int startAltitude, altitude;
int startPressure;
int temperature;

bool m_shutDown = false;

const int chipSelect = 10;

RH_RF95 driver;
Servo servo1, servo2;
Adafruit_BMP085 barometer;

// Inits -----------------------------

void initComms() {
    Serial.println("Initializing LoRa comms ----------------");
    if (!driver.init()) {
        Serial.println("ERROR");
        // shutDown();
    }
    else
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
    else 
        sendData("done.");
    // Serial.println("done.");

    // Read presure and calculate altitude at start
    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

void initSDBoard() {
    //Serial.print("Initializing SD card...");
    sendData("Initializing SD card");
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
        //Serial.println("ERROR");
        sendData("ERROR");
        // don't do anything more:
        while (1);
    }
    else 
        sendData("done.");
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
    altitude = barometer.readAltitude(startPressure) - startAltitude;     // this will give the change in the altitude

    // Serial.println(altitude);

    sendData(temperature);
    sendData(altitude);
}

void updateSDBoard(char data) {  /* REVIEW THIS */
    String dataString = data;

    // read three sensors and append to the string:
    for (int analogPin = 0; analogPin < 3; analogPin++) {
      int sensor = analogRead(analogPin);
      dataString += String(sensor);
      if (analogPin < 2) {
        dataString += ",";
      }
    }

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.flightComputer", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
    }
    // if the file isn't open, pop up an error:
    else {
      //Serial.println("error opening datalog.txt");
      sendData("Error writing to SD card");
    }
}

// Updates -----------------------------

void setup() {
  Serial.begin(9600); // only used for debugging

  initComms();          // Communication
  initServos();         // Servos
  initBarometric();     // Barometric sensor/temperature
  initSDBoard();        // SD card module
}

void loop() {
    // Important Systems
    if (recieveData("off")) { shutDown(); } // When shut down signal is recieved or comms init fail, system updates are shut down
    if (m_shutDown) { break }

    // Updates
    updateBarometric();
    updateSDBoard();

    // Examples
    // sendData(temperature);
    //
    // if (recieveData() == "parachute") {
    //     deployParachute();
    // }
    //

    delay(500);
}

void shutDown() {
    m_shutDown = true;
}