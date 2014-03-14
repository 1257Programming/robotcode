#include "WPILib.h"

typedef DriverStationLCD dLcd;

class Team1257Robot : public SimpleRobot
{
public:
	Joystick Stick1; //Driver
	Joystick Stick2; //Arms
	Victor LeftDrive1;
	Victor LeftDrive2;
	Victor RightDrive1;
	Victor RightDrive2;
	RobotDrive Drive;
	Victor LeftArm;
	Victor RightArm;
	Victor ArmShoulder1;
	Victor ArmShoulder2;
	DriverStationLCD* Lcd; //Message Box
	double leftspeed;
	double rightspeed;
	double speed;
	double curve;
	double leftarmspeed;
	double rightarmspeed;
	double shoulderspeed;
	
	Team1257Robot(): Stick1(1), Stick2(2), LeftDrive1(1), LeftDrive2(2), RightDrive1(7), RightDrive2(8), 
			Drive(LeftDrive1, LeftDrive2, RightDrive1, RightDrive2), 
			LeftArm(3), RightArm(4), ArmShoulder1(5), ArmShoulder2(6)
	{
		Drive.SetExpiration(.1);
		Lcd = DriverStationLCD::GetInstance();
		leftspeed = 0;
		rightspeed = 0;
		leftarmspeed = 0;
		rightarmspeed = 0;
		shoulderspeed = 0;
		speed = 0;
		curve = 0;
	}
	void OperatorControl();
	void Autonomous();
	void Test();	double accel(Joystick& stick, int axis, double& current, double sf);
	void drive();
	void arms();
	double dabs(double value)
	{
		if(value >= 0)
			return value;
		else
			return -value;
	}
	
};
