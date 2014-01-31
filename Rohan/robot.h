#include "WPILib.h"
#include "math.h"
#include "vision.h"
#include "string.h"

#define LEFT_STICK 1

typedef DriverStationLCD dLcd; //cant type that massive class!

float sf = 0.5;

class Robot: public SimpleRobot {
public:
	Robot();
	void Autonomous();
	void OperatorControl();
	inline void drive(double x, double y);
	void drive();
	void arcadeDrive();
	void print(char* msg, int i);
	void accel(double x, double y);
	bool controlArms(bool arm);
	inline double toMM(double voltage);
	inline double toIN(double voltage);
public:
	DriverStationLCD* lcd;
	RobotDrive rd;
	Joystick stick;
	Gyro angle;
	Vision vis;
	Timer time;
	Relay relay;
	AnalogChannel ac;
	Victor arm1, arm2;
	double speed;
};
