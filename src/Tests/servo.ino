#include <servo.h>

Servo servo;

byte servoPin = 9;

void setup() { 
    // print servo is initialized
    servo.attach(servoPin);
    servo.write(0);
}