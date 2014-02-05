#include "WPILib.h"
#define dAbs(val) (val < 0 ? -val : val)
class Team1257Robot : public SimpleRobot
{
public:
	Joystick Stick1;
	Joystick Stick2;
	Victor LeftDrive;
	Victor RightDrive;
	RobotDrive Drive;
	Victor LeftArm;
	Victor RightArm;
	Victor ArmShoulder;
	Gyro Angle;
	//AnalogChannel Usonic;
	DriverStationLCD* Lcd;
	double leftspeed;
	double rightspeed;
	double speed;
	double curve;
	double leftarmspeed;
	double rightarmspeed;
	double shoulderspeed;
	
	Team1257Robot(): Stick1(1), Stick2(2), LeftDrive(1), RightDrive(2), Drive(LeftDrive, RightDrive),
			LeftArm(3), RightArm(4), ArmShoulder(5), Angle(1)
	{
		Drive.SetExpiration(.1);
		Lcd = DriverStationLCD::GetInstance();
		leftspeed = 0;
		rightspeed = 0;
		leftarmspeed = 0;
		rightarmspeed = 0;
		shoulderspeed = 0;
	}
	void OperatorControl();
	void Autonomous();
	void Test();
	double accel(Joystick& stick, int axis, double& current, double limit);
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
