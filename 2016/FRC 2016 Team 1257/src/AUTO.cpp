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

	double autospeed = .85;
	double autoturn = 0;
	while(true)
	{
		if(gyro.GetAngle() < -5)
		{
			autoturn -= .001;
		}

		else if(gyro.GetAngle() > 5)
		{
			autoturn += .001;
		}

		ArcadeDrive(-autospeed, autoturn, false);


		if(IsDisabled())
		{
			break;
		}

		if(timeout.Get() > 2.5)
		{
			break;
		}

	//	if(encDriveLeft.GetDistance() > (9 * 12) || encDriveRight.GetDistance() > (9 * 12))
	//	{
	//		break;
	//	}

	}
	SetDriveMotors(0, 0);
	std::cout<<"Going Forward"<<std::endl;
}

void Robot::backcross()
{
	Timer timeout;
	timeout.Start();

	double autospeed = .55;
	double autoturn = 0;

	while(true)
	{
		dashstatus();
		SmartDashboard::PutNumber("AUTO_TIMEOUT", timeout.Get());

		if(gyro.GetAngle() < -5)
		{
			autoturn -= .001;
		}

		else if(gyro.GetAngle() > 5)
		{
			autoturn += .001;
		}

		ArcadeDrive(-autospeed, autoturn, false);

		if(IsDisabled())
		{
			break;
		}

		if(timeout.Get() > .75)
		{
			break;
		}

		//if(encDriveLeft.GetDistance() < -(2.5 * 12) || encDriveRight.GetDistance() < -(2.5 * 12))
		//{
//
//		}
	}
	SetDriveMotors(0, 0);
	timeout.Stop();
	timeout.Reset();

	intakeAdjust(false);
	wedgeAdjust(false);

	timeout.Start();

	while(true)
	{
		dashstatus();
		SmartDashboard::PutNumber("AUTO_TIMEOUT", timeout.Get());

		if(gyro.GetAngle() < -5)
		{
			autoturn -= .001;
		}

		else if(gyro.GetAngle() > 5)
		{
			autoturn += .001;
		}

		ArcadeDrive(-autospeed, autoturn, false);

		if(IsDisabled())
		{
			break;
		}

		if(timeout.Get() > 2)
		{
			break;
		}

		//if(encDriveLeft.GetDistance() < -(7 * 12) || encDriveRight.GetDistance() < -(7 * 12))
		//{
		//	break;
		//}
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
