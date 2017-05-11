#include "Robot.h"

void TrainingRobit::AutonomousInit()
{
	Gyro.Reset();
	DriveTrain.ArcadeDrive(1, 0, false);
	Wait(5);
	DriveTrain.ArcadeDrive(0, 0, false);

}
void TrainingRobit::AutonomousPeriodic()
{
	SmartDashboard::PutNumber("Gyro", Gyro.GetAngle());
int angle = Gyro.GetAngle();
	if(abs(angle%360) > 180)
	{
		DriveTrain.ArcadeDrive(0, (360 - angle%360), false);
	}
	else if(abs(angle%360) < 180)
	{
		DriveTrain.ArcadeDrive(0, -(angle%360), false);
	}
}
