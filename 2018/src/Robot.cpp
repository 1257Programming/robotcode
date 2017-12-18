#include <iostream>
#include <memory>
#include <string>

#include "WPILib.h"
#include "CANTalon.h"
using namespace std;
using namespace frc;

class Robot : public IterativeRobot
{
private:
	CANTalon FLMotor;
	CANTalon FRMotor;
	CANTalon BLMotor;
	CANTalon BRMotor;
	RobotDrive DriveTrain;
	XboxController DriveController;

public:
	Robot() :
		FLMotor(1),
		FRMotor(2),
		BLMotor(3),
		BRMotor(4),
		DriveTrain(FLMotor, FRMotor, BLMotor, BRMotor),
		DriveController(0)
	{}

	void TeleopPeriodic()
	{
		double speed = DriveController.GetY(GenericHID::JoystickHand::kLeftHand);
		double angle = DriveController.GetX(GenericHID::JoystickHand::kLeftHand);
		DriveTrain.ArcadeDrive(speed, angle, false);
	}
};

START_ROBOT_CLASS(Robot)
