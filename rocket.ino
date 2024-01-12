/* Flight logging system firmware for Arduino Nano */

// Including
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
byte startAltitude, altitude, realAltitude;
byte apogee;
byte temperature;

int startPressure;

const byte servoPin = A0;
const byte chipSelect = 7;

bool isFalling = false;

// Inits

void initBarometric()
{
    Serial.print("Initializing Barometric sensors  -  ");
    //SendData("Initializing Barometric sensors  -  ");

    if (!barometer.begin())
    {
        Serial.println("ERROR");
        //SendData("ERROR");
        while (1)
            ;
    }
    Serial.println("done.");
    //SendData("done.");
    
    startPressure = barometer.readPressure();
    startAltitude = barometer.readAltitude(startPressure);
}

void initComms()
{
    Serial.print("Initializing LoRa comms  -  ");
    //SendData("Initializing LoRa comms  -  ");

    if (!LoRa.begin(915E6))  // initialize ratio at 915 MHz
    { 
        Serial.println("ERROR");
        //SendData("ERROR");
        while (true)
            ;
    }
    Serial.println("done.");
    //SendData("done.");
}

void initServo()
{
    Serial.print("Initializing Servo  -  ");
    //SendData("Initializing Servo  -  ");

    servo.attach(servoPin);
    servo.write(0); // CHANGE BEFORE FLIGHT -------------

    Serial.println("done.");
    //SendData("done.");
}

void initSD()
{
    Serial.print("Initializing SD card  -  ");
    //SendData("Initializing SD card  -  ");

    if (!SD.begin(chipSelect))
    {
        Serial.println("ERROR");
        //SendData("ERROR");
        while (1)
            ;
    }
    Serial.println("done.");
    //SendData("done.");
}

void initGyroscope()
{
    Serial.print("Initializing Gyroscope/Accelerometer  -  ");
    //SendData("Initializing Gyroscope/Accelerometer  -  ");

    // Try to initialize!
    if (!gyroscope.begin())
    {
        Serial.print("ERROR");
        //SendData("ERROR");
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
    //SendData("done.");

    apogee = 2; // So computer doesn't think it is on apogee at start.

    Serial.println("");
    delay(100);
}

// Setup

void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing systems...");
    //SendData("Initializing systems...");

    // initialize systems
    initComms();
    initBarometric();
    initServo();
    initSD();
    initGyroscope();

    Serial.println("Initialization complete.");
    Serial.println("");

    // SendData("Initialization complete.");

    // clear SD card before logging
    clearSD();
    writeSD("LOGGING STARTED");
}

// --------------

// Remote comms

template <typename remoteData>
void SendData(remoteData data)  // sends data to ground station, all data will be send in a new line
{
    LoRa.beginPacket();
    LoRa.print(String(data));
    LoRa.endPacket();
}

String RecieveData()
{
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        while (LoRa.available()) {
            return LoRa.readString();
        }
    }
}

// --------------

void clearSD()
{
    File dataFile = SD.open("DATALOG.txt", FILE_WRITE  | O_TRUNC);

    if (dataFile)
    {
        dataFile.println("");
        dataFile.close();
    }
    else
    {
        Serial.println("ERROR opening DATALOG.txt");
    }
}

template <typename writeData>
void writeSD(writeData data)
{
    File dataFile = SD.open("DATALOG.txt", FILE_WRITE);

    if (dataFile)
    {
        dataFile.print(data);
        dataFile.close();
    }
    else
    {
        Serial.println("ERROR opening DATALOG.txt");
    }
}

template <typename writeDataLn>
void writeSDln(writeDataLn data)
{
    File dataFile = SD.open("DATALOG.txt", FILE_WRITE);

    if (dataFile)
    {
        dataFile.println(data);
        dataFile.close();
    }
    else
    {
        Serial.println("ERROR opening DATALOG.txt");
    }
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
    }
}

void deployParachute()
{
    servo.write(180); // CHANGE BEFORE FLIGHT -------------
                      // angle of 0 degrees should also work 

    //writeSDln("PARACHUTE DEPLOYED");
    //SendData("PARACHUTE DEPLOYED");
}

void loop()
{
    // updates
    updateBarometric();
    delay(100);
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
        writeSDln("APOGEE REACHED");
        writeSD("APOGEE: ");
        writeSDln(apogee);

        SendData("APOGEE REACHED");
        SendData(apogee);

        Serial.print("APOGEE REACHED:  ");
        Serial.println(apogee);

        apogeeSent = true; // only send the apogee once
    }

    delay(100); // updates every 10 times/s
}
