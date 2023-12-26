#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <Wire.h>
#include <SPI.h>

#include <Servo.h>

#include <SD.h>

#include <LoRa.h>

// Class instances
Adafruit_BMP085 barometer;
Adafruit_MPU6050 gyroscope;
Servo servo;

// Variables
float startAltitude, altitude;
int startPressure;

byte temperature;

const byte servoPin = A0;
const int chipSelect = 7;

// Inits

void initBarometric()
{
    Serial.print("Initializing Barometric sensors  -  ");

    if (!barometer.begin())
    {
        Serial.println("ERROR");
        while (1)
        {
        }
    }
    else
        Serial.println("done.");

    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

void initCommms()
{
    Serial.print("Initializing LoRa comms  -  ");

    if (!LoRa.begin(915E6))
    { // initialize ratio at 915 MHz
        Serial.println("ERROR");
        while (true);
    }
    Serial.println("done.");
    
}

void initServo()
{
    Serial.print("Initializing Servo  -  ");

    servo.attach(servoPin);
    servo.write(0); // CHANGE BEFORE FLIGHT -------------

    Serial.println("done.");
}

void initSD()
{
    Serial.print("Initializing SD card  -  ");

    if (!SD.begin(chipSelect))
    {
        Serial.println("ERROR");
        while (1);
    }
    Serial.println("done.");
}

void initGyroscope()
{
    Serial.print("Initializing Gyroscope/Accelerometer  -  ");

    // Try to initialize!
    if (!gyroscope.begin())
    {
        Serial.print("ERROR");
        while (1)
        {
            delay(10);
        }
    }
    // setupt motion detection
    gyroscope.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    gyroscope.setMotionDetectionThreshold(1);
    gyroscope.setMotionDetectionDuration(20);
    gyroscope.setInterruptPinLatch(true); // Keep it latched.  Will turn off when reinitialized.
    gyroscope.setInterruptPinPolarity(true);
    gyroscope.setMotionInterrupt(true);

    Serial.print("done.");

    Serial.println("");
    delay(100);
}

//

void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing systems...");


    // initialize systems
    initCommms();
    initBarometric();
    initServo();
    initSD();
    initGyroscope();

    Serial.println("Initialization complete.");
}

void loop()
{
}