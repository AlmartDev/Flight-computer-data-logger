#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change the parameters if your LCD is different


float startAltitude, altitude;
float startPressure;

Adafruit_BMP085 barometer;

void initBarometric() {
    Serial.println("Initializing Barometric sensors");

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

    lcd.begin();  // Initialize the LCD
}

void loop() {
    //temperature = barometer.readTemperature();

    altitude = barometer.readAltitude(startPressure);  

    Serial.println(altitude - startAltitude);
    //Serial.println (" - t:");
    //Serial.print(temperature);

    lcd.setCursor(0, 0);  // Set the cursor to the top-left corner
    lcd.print(altitude - startAltitude); 

    delay(500);
    lcd.setCursor(0, 0);  // Set the cursor to the top-left corner

    lcd.print("                      "); 
}