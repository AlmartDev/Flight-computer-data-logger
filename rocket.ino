/* Flight logging system firmware for Arduino Nano */
/* Open Source project from https://github.com/almartdev/flight-computer-data-logger */
/* TODO: Make SD & comms functions simple class to include */

// Including
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <Wire.h>
#include <SPI.h>

#include <Servo.h>

// Custom Libraries
#include "include/comms.h"
#include "include/SDcard.h"

#define LORA_SS 10
#define LORA_RST 9
#define LORA_DIO0 2

// Class instances
Adafruit_BMP085 barometer;
Adafruit_MPU6050 gyroscope;
Servo servo;

Lora lora(433E6, LORA_SS, LORA_RST, LORA_DIO0);
SDcard SD(7);

// Variables
byte startAltitude, altitude, realAltitude;
byte apogee;
byte temperature;

int startPressure;

const byte servoPin = A0;

bool isFalling = false;

// Inits

void initBarometric()
{
    Serial.print("Initializing Barometric sensors  -  ");
    //lora.sendData("Initializing Barometric sensors  -  ");

    if (!barometer.begin())
    {
        Serial.println("ERROR");
        //lora.sendData("ERROR");
        while (1)
            ;
    }
    Serial.println("done.");
    //lora.sendData("done.");
    
    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

void initServo()
{
    Serial.print("Initializing Servo  -  ");
    //lora.sendData("Initializing Servo  -  ");

    servo.attach(servoPin);
    servo.write(0); // CHANGE BEFORE FLIGHT -------------

    Serial.println("done.");
    //lora.sendData("done.");
}

void initGyroscope()
{
    Serial.print("Initializing Gyroscope/Accelerometer  -  ");
    //lora.sendData("Initializing Gyroscope/Accelerometer  -  ");

    // Try to initialize!
    if (!gyroscope.begin())
    {
        Serial.print("ERROR");
        //lora.sendData("ERROR");
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
    //lora.sendData("done.");

    apogee = 2; // So computer doesn't think it is on apogee at start.

    Serial.println("");
    delay(100);
}

// Setup

void setup()
{
    
    Serial.begin(9600);
    while (!Serial);    // wait for serial port to connect. Needed for native USB port only

    Serial.println("Initializing systems...");
    //lora.sendData("Initializing systems...");

    // Lora and SD card are initialized in their own classes

    // initialize systems
    initBarometric();
    initServo();
    initGyroscope();

    Serial.println("Initialization complete.");
    Serial.println("");

    // lora.sendData("Initialization complete.");

    // clear SD card before logging
    SD.clear();
    SD.write("LOGGING STARTED");
    SD.write("All systems initialized  -  Logging started");
    SD.writeln("");
}

// updates
void updateBarometric()
{
    temperature = barometer.readTemperature();
    altitude = barometer.readAltitude(startPressure);

    byte previousAltitude = realAltitude;

    realAltitude = altitude - startAltitude;

    /*
    Serial.print ("a:");
    Serial.print(altitude - startAltitude);
    Serial.print (" - t:");
    Serial.println(temperature);
    */

    // write values to SD card
    SD.write("a: ");
    SD.write(altitude - startAltitude);
    SD.write(" - t°C: ");
    SD.writeln(temperature);

    // Calculate apogee and if falling

    if (realAltitude < previousAltitude)
    {
        apogee = previousAltitude;
        isFalling = true;
    }
    if (realAltitude > apogee)
        apogee = realAltitude;
}

void updateGyroscope()
{
    if (gyroscope.getMotionInterruptStatus())
    {
        /* Get new sensor events with the readings */
        sensors_event_t a, g, temp;
        gyroscope.getEvent(&a, &g, &temp);

        /*
        Serial.print("GyroX:");
        Serial.print(g.gyro.x);
        Serial.print(",");
        Serial.print("GyroY:");
        Serial.print(g.gyro.y);
        Serial.print(",");
        Serial.print("GyroZ:");
        Serial.print(g.gyro.z);
        Serial.println("");
        */

        // write values to SD card
        SD.write("GyroX:");
        SD.write(g.gyro.x);
        SD.write(",");
        SD.write("GyroY:");
        SD.write(g.gyro.y);
        SD.write(",");
        SD.write("GyroZ:");
        SD.write(g.gyro.z);
        SD.writeln("");
    }
}

void deployParachute()
{
    servo.write(180); // CHANGE BEFORE FLIGHT -------------
                      // angle of 0 degrees should also work 

    //SD.write("PARACHUTE DEPLOYED");
    //SendData("PARACHUTE DEPLOYED");
}

void loop()
{
    // updates
    updateBarometric();
    delay(100);     // this stops the two updates from interfering with each other and causing the "multiple setups bug"
    updateGyroscope();

    // check for parachute deployment
    if (realAltitude > 2 && isFalling)  // deploy parachute if falling and above 2m
        deployParachute();
    else if (RecieveData() == "DEPLOY") // deploy parachute if recieve "DEPLOY" from ground
        deployParachute(); 

    // Apogee sending
    bool apogeeSent = false;
    if (isFalling && !apogeeSent)
    {
        SD.writeln("APOGEE REACHED");
        SD.write("APOGEE: ");
        SD.writeln(apogee);

        lora.sendData("APOGEE REACHED");
        //lora.sendData(apogee);

        Serial.print("APOGEE REACHED:  ");
        Serial.println(apogee);

        apogeeSent = true; // only send the apogee once
    }

    delay(500); // updates 2/sec due to comms problems
}
