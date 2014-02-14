#include "WPILib.h"

class Team1257Robot : public SimpleRobot
{
public:
	Joystick Stick1; //Driver
	Joystick Stick2; //Arms
	Victor LeftDrive;
	Victor RightDrive;
	RobotDrive Drive;
	Victor LeftArm;
	Victor RightArm;
	Victor ArmShoulder;
	DriverStationLCD* Lcd; //Message Box
	double leftspeed;
	double rightspeed;
	double speed;
	double curve;
	double leftarmspeed;
	double rightarmspeed;
	double shoulderspeed;
	bool realbot; // Testbot or Realbot
	
	Team1257Robot(): Stick1(1), Stick2(2), LeftDrive(1), RightDrive(2), Drive(LeftDrive, RightDrive),
			LeftArm(4), RightArm(5), ArmShoulder(3)
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
		realbot = true; // Assume real robot
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

