#include <Servo.h>

Servo servo;

void setup() {
    Serial.begin(9600);

    Serial.println("Servos initialized")
    Serial.println("done.")

    servo.attach(A0);
    servo.write(90);
}

void deployParachute() {
    // the parachute open when the angle of the servo moves out of the 90 degrees
    servo.write(0); // or 180 degrees (would take slightly more memory)

    // make the gyroscope stop reading and sending information
    // send information that the parachute has been deployed and write it on the sd card so data isnt misunderstood
}