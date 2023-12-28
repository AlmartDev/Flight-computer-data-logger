#include <Servo.h>

Servo servo;

int servoPin = 9;

void setup() { 
    // print servo is initialized
    servo.attach(servoPin);
    servo.write(0);
}