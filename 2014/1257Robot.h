#include "WPILib.h"

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
	//Ultrasonic Usonic;
	DriverStationLCD* Lcd;
	double speed1;
	double speed2;
	
	Team1257Robot(): Stick1(1), Stick2(2), LeftDrive(1), RightDrive(2), Drive(LeftDrive, RightDrive),
			LeftArm(3), RightArm(4), ArmShoulder(5), Angle(1)
	{
		Drive.SetExpiration(.1);
		Lcd = DriverStationLCD::GetInstance();
		speed1 = 0;
		speed2 = 0;
	}
	void OperatorControl();
	void Autonomous();
	void Test();
	void drive();
};
