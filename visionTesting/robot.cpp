#include "Robot.h"

TrainingRobit::TrainingRobit() :
FrontLeftDrive(1),
BackLeftDrive(2),
FrontRightDrive(3),
BackRightDrive(4),
DriveTrain(FrontLeftDrive, BackLeftDrive, FrontRightDrive, BackRightDrive),
Gyro(SPI::kOnboardCS0)
{
	numberofMotors = 4;
}

void TrainingRobit::RobotInit()
{

}

void TrainingRobit::AutonomousInit()
{
	Gyro.Reset();

}
void TrainingRobit::AutonomousPeriodic()
{
	SmartDashboard::PutNumber("Gyro", Gyro.GetAngle());
	int angle = round(Gyro.GetAngle());
	if(angle > .5 && angle % 360 < 180)
	{
		DriveTrain.ArcadeDrive(0, .1, false);
	}
	else if(angle < -.5 && angle % 360 < 180)
	{
		DriveTrain.ArcadeDrive(0, -.1, false);
	}
	else
	{
		DriveTrain.ArcadeDrive(.5, 0, false);
	}
}

void TrainingRobit::TeleopInit()
{

}

void TrainingRobit::TeleopPeriodic()
{

}

void TrainingRobit::DisabledInit()
{

}

void TrainingRobit::UpdateDashboard()
{

}

START_ROBOT_CLASS(TrainingRobit)
