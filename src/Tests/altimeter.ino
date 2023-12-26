#include <Adafruit_BMP085.h>
#include <Wire.h>

Adafruit_BMP085 barometer;

float startAltitude, altitude;
int startPressure;

int temperature;

void initBarometric() {
    Serial.print("Initializing Barometric sensors  -  ");

    if (!barometer.begin()) {
        Serial.println("ERROR");
        while (1) {}
    }
    else
        Serial.println("done.");

    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

void setup () {
    Serial.begin(9600);
    initBarometric();
}

void loop() {
    temperature = barometer.readTemperature();

    altitude = barometer.readAltitude(startPressure);  

    Serial.print ("a:");
    Serial.print(altitude - startAltitude);
    Serial.print (" - t:");
    Serial.println(temperature);

    delay(1000);
}