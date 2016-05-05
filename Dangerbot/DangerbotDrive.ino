#include <Servo.h>

const int servoL = 3;
const int servoR = 10;
const int joyLR = 3;
const int joyUD = 4;

//to read value from pin, stores analog values from joystick
int servoValH;
int servoValV;

Servo servoLeft;
Servo servoRight;

void setup() {
  servoLeft.attach(servoL);
  servoRight.attach(servoR);
  
  Serial.begin(9600);
}

void setLeftRightMotorOutputs(double left, double right) {
  servoLeft.write((left + 1.0)*90);
  servoRight.write((right + 1.0)*90);
}

void loop() {
  servoValH = analogRead(joyLR);
  servoValH = map(servoValH, 0, 1023, 0, 180); 
  servoValV = analogRead(joyUD);
  servoValV = map(servoValV, 0, 1023, 0, 180)
   
}

void forward() {
  servoLeft.write(180);
  servoRight.write(180);
}

void reverse() {
  servoLeft.write(0);
  servoRight.write(0);
}

void turnRight() {
  servoLeft.write(180);
  servoRight.write(0);
}
void turnLeft() {
  servoLeft.write(0);
  servoRight.write(180);
}
