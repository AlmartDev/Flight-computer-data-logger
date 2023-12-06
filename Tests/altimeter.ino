#include <Adafruit_BMP085.h>
#include <Wire.h>

int startAltitude, altitude;
int startPressure;
int temperature;

Adafruit_BMP085 barometer;

void initBarometric() {
    Serial.println("Initializing Barometric sensors");

    if (!barometer.begin()) {
        Serial.println("ERROR");
        while (1) {}
    }

    Serial.println("done.");

    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

void setup () {
    Serial.begin(9600);
    initBarometric();
}

void update() {
    temperature = barometer.readTemperature();

    altitude = barometer.readAltitude(startPressure) - startAltitude;   

    Serial.println(altitude);
    Serial.println(temperature);
}