#include "robot.h"
#include "math.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */

Robot::Robot() :
	rd(1, 2), stick(LEFT_STICK), angle(1), relay(1) {
	rd.SetExpiration(0.1);
	lcd = dLcd::GetInstance();
	speed = 0;
}

#define PRINT(msg){lcd->Printf(dLcd::kUser_Line1, 1, msg);}//simple print
void Robot::Autonomous() {
	PRINT("Starting Autonomous");
	//relay.Set(relay.kForward);
	bool got = false;
	while (IsAutonomous() && IsEnabled()) {
		if (!got) {
			vis.aim();
			got = true;
		} else
			continue;
	}
	//relay.Set(relay.kOff);
}

void Robot::OperatorControl() {
	relay.Set(relay.kOff);
	PRINT("Started Operator Control");
	while (IsOperatorControl() && IsEnabled()) {
		if (stick.GetRawButton(5) && stick.GetRawButton(6))
			arcadeDrive();
		else if (stick.GetZ())
			drive();
	}
}

inline void Robot::drive(double x, double y) {
	rd.SetLeftRightMotorOutputs(x, y);
}

void Robot::drive()//tank drive
{
	double x = stick.GetY();
	double y = stick.GetRawAxis(5);
	accel(x, y);
}

void Robot::arcadeDrive() {
	double x = stick.GetX();
	double y = stick.GetRawAxis(5);
	rd.ArcadeDrive(y, x, false);
}

void Robot::accel(double x, double y) {
	if (x == speed && y == speed)
		drive(speed, speed);
	if (x == y) {
		double target = x;
		double n = 0;
		if (target < 0) {
			while (n > target) {
				drive(n, n);
				n -= 0.05;
			}
			speed = n;
		} else {
			while (n < target) {
				drive(n, n);
				n += 0.05;
			}
		}
	} else
		drive(x * sf, y * sf);
}

void Robot::print(char* msg, int i) {
	if (i > 7)
		i = 6;
	if (i < 1)
		i = 1;

	switch (i) {
	case 1:
		lcd->Printf(dLcd::kUser_Line1, 1, msg);
		break;
	case 2:
		lcd->Printf(dLcd::kUser_Line2, 1, msg);
		break;
	case 3:
		lcd->Printf(dLcd::kUser_Line3, 1, msg);
		break;
	case 4:
		lcd->Printf(dLcd::kUser_Line4, 1, msg);
		break;
	case 5:
		lcd->Printf(dLcd::kUser_Line5, 1, msg);
		break;
	case 6:
		lcd->Printf(dLcd::kUser_Line6, 1, msg);
		break;
	}
	lcd->UpdateLCD();
}

START_ROBOT_CLASS(Robot)
;
