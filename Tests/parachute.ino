#include <Servo.h>

Servo servo;

void setup() {
    servo.attach(A0);
    servo.write(180);
}