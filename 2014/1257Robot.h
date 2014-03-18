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
	//THEY Victor LeftArm;
	//BWOKE Victor RightArm;
	Victor ArmShoulder1;
	Victor ArmShoulder2;
	DriverStationLCD* Lcd; //Message Box
	DigitalInput LimitSwitch;
	double leftspeed;
	double rightspeed;
	double speed;
	double curve;
	double leftarmspeed;
	double rightarmspeed;
	double shoulderspeed;
	
	Team1257Robot(): Stick1(1), Stick2(2), LeftDrive1(3), LeftDrive2(4), RightDrive1(9), RightDrive2(10), 
			Drive(LeftDrive1, LeftDrive2, RightDrive1, RightDrive2), 
			ArmShoulder1(7), ArmShoulder2(8), LimitSwitch(2)
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
	void Test();	
	double accel(Joystick& stick, int axis, double& current, double sf);
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
