#include <Servo.h>
#include <cmath>
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

void SetDriveMotors(double left, double right) {
  servoLeft.write((left + 1.0)*90);
  servoRight.write((right + 1.0)*90);
}
void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs)
{
// local variables to hold the computed PWM values for the motors
  float leftMotorOutput;
  float rightMotorOutput;

  if (squaredInputs)
  {
	// square the inputs (while preserving the sign) to increase fine control
	// while permitting full power
	if (moveValue >= 0.0)
	{
		moveValue = (moveValue * moveValue);
	}
	else
	{
		moveValue = -(moveValue * moveValue);
	}

	if (rotateValue >= 0.0)
	{
		rotateValue = (rotateValue * rotateValue);
	}
	else
	{
		rotateValue = -(rotateValue * rotateValue);
	}
  }

  if (moveValue > 0.0)
  {
	  if (rotateValue > 0.0)
	  {
		  leftMotorOutput = moveValue - rotateValue;
		  rightMotorOutput = std::max(moveValue, rotateValue);
	  }
	  else
	  {
		leftMotorOutput = std::max(moveValue, -rotateValue);
		rightMotorOutput = moveValue + rotateValue;
	  }
  }
  else
  {
	  if (rotateValue > 0.0)
	  {
		  leftMotorOutput = -std::max(-moveValue, rotateValue);
		  rightMotorOutput = moveValue + rotateValue;
	  }
	  else
	  {
		  leftMotorOutput = moveValue - rotateValue;
		  rightMotorOutput = -std::max(-moveValue, -rotateValue);
	  }
  }

  SetDriveMotors(leftMotorOutput, -rightMotorOutput);
}
void loop() {
  servoValH = analogRead(joyLR);
  servoValH = map(servoValH, 0, 1023, 0, 180); 
  servoValV = analogRead(joyUD);
  servoValV = map(servoValV, 0, 1023, 0, 180)
  ArcadeDrive(servoValV, servoValH, true);
}
