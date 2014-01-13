#include <WPILib.h>
#include <Wait.h>
#include <cstdarg>
#define LEFT 1
#define RIGHT 2
#define LARM 3
#define RARM 4
#define LBEND 5
#define RBEND 6
#define TANK_DRIVE 0
#define ARCADE_DRIVE 1
#define ZTWIST_DRIVE 2
#define XY_DRIVE 3
#define NULL (void*)0
class Team1257Robot : public SimpleRobot {
	RobotDrive team1257Robot;
	Joystick LeftStick;
	Joystick RightStick;
	Servo armServo1;
	Servo armServo2;
	Servo armBend1;
	Servo armBend2;
	DriverStationLCD * team1257LCD;
	bool leftStickIsEnabled;
	bool rightStickIsEnabled;
	int whichDrive;
public:
	Team1257Robot();
	void Autonomous();
	void OperatorControl();
	void TankDrive();
	void ArcadeDrive();
	void ZTwistDrive();
	void XYDrive();
	void Drive(float, float);
	int GetWhichDrive();
	void SetWhichDrive(int);
	void CheckSetDriveSticks();
	void SetArmsX(float);
	void SetArmsY(float);
	void printf(char*, ...);
	RobotDrive * GetDrive();
	Joystick * GetRightStick();
	Joystick * GetLeftStick();
	Servo * GetServoInstanceFromNumber(int);
	DriverStationLCD * GetLCD();
};
