/* Flight logging system firmware for Arduino Nano */

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
float startAltitude, altitude, realAltitude;
int startPressure;

byte temperature;

const byte servoPin = A0;
const byte chipSelect = 7;

char *data;

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
        while (true)
            ;
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
        while (1)
            ;
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
    Serial.println("");

    // Write beggining of logs
    // writeSD("FLIGHT LOGGING STARTED");
}

// Systems
void SendData(const char *data)
{
    // Send data
}

void recieveData()
{
    // Recieve data
    // return data;
}

// --------------

// TODO: dont use String for data :)
void writeSD(char* data)
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

void writeSDln(char* data)
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

    realAltitude = altitude - startAltitude;

    /*
    Serial.print ("a:");
    Serial.print(altitude - startAltitude);
    Serial.print (" - t:");
    Serial.println(temperature);
    */
}

void updateGyroscope(){
    if(gyroscope.getMotionInterruptStatus()) {
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

void loop()
{
    // just updates, no loggin or sending yet
    updateBarometric();
    updateGyroscope();

    delay(100); // updates every 10 times/s
}