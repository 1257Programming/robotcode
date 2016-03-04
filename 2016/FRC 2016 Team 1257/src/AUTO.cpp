#include "Robot.h"

void Robot::reach()
{

}

void Robot::scorepath()
{

}

void Robot::forcross()
{
	Timer timeout;
	timeout.Start();
	while(encDriveLeft.GetDistance() < (13 * 12) || encDriveRight.GetDistance() < (13 * 12))
	{
		if(gyro.GetAngle() < -5 && !pidGyro.IsEnabled())
		{
			SetDriveMotors(.85, -.80);
		}
		else if(gyro.GetAngle() > 5 && !pidGyro.IsEnabled())
		{
			SetDriveMotors(.80, -.85);
		}
	/*	else if(dabs(gyro.GetAngle()) > 20)
		{
			pidGyro.Enable();
		} */
		else
		{
			SetDriveMotors(.85, -.85);
			pidGyro.Disable();
		}

		if(IsDisabled())
		{
			break;
		}

		if(timeout.Get() > 7)
		{
			break;
		}
	}
	SetDriveMotors(0, 0);
	std::cout<<"Going Forward"<<std::endl;
}

void Robot::backcross()
{
	Timer timeout;
	timeout.Start();

	while(encDriveLeft.GetDistance() > -(5 * 12) || encDriveRight.GetDistance() > -(5 * 12))
	{
		if(gyro.GetAngle() < -5)
		{
			SetDriveMotors(-.85, .80);
		}
		else if(gyro.GetAngle() > 5)
		{
			SetDriveMotors(-.80, .85);
		}
		else
		{
			SetDriveMotors(-.85, .85);
		}

		if(IsDisabled())
		{
			break;
		}

		if(timeout.Get() > 7)
		{
			break;
		}
	}
	SetDriveMotors(0, 0);
	timeout.Stop();
	timeout.Reset();

	intakeAdjust(false);
	wedgeAdjust(false);

	timeout.Start();

	while(encDriveLeft.GetDistance() > -(10 * 12) || encDriveRight.GetDistance() > -(10 * 12))
	{
		if(gyro.GetAngle() < -5)
		{
			SetDriveMotors(-.55, .50);
		}
		else if(gyro.GetAngle() > 5)
		{
			SetDriveMotors(-.50, .55);
		}
		else
		{
			SetDriveMotors(-.55, .55);
		}

		if(IsDisabled())
		{
			break;
		}

		if(timeout.Get() > 7)
		{
			break;
		}
	}
	SetDriveMotors(0, 0);
	std::cout<<"Going Backward"<<std::endl;

}

void Robot::portcullis()
{

}

void Robot::cheval()
{

}
