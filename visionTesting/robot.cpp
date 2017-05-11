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

void TrainingRobit::DisabledInit()
{

}

void TrainingRobit::UpdateDashboard()
{

}

START_ROBOT_CLASS(TrainingRobit)
