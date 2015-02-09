#include "1257Robot.h"

Team1257Robot::Team1257Robot():
	Left(0), Right(1), Center(2), Lift(3), Stick1(0), Stick2(1), dSolenoid(4, 5), pixy(GET_CALL_PTR(Team1257Robot, blockProcess), this)
{
	Lw = LiveWindow::GetInstance();
}

void Team1257Robot::TeleopInit()
{

}

void Team1257Robot::TeleopPeriodic()
{
	double sf = 0.5;
	double strafesf = 1;
	if (Stick1.GetRawButton(5) && !Stick1.GetRawButton(6)) // Back button; just one, not both
	{
		Center.Set(-(float)accel(Stick1, 0, strafe, strafesf));
		if(!Stick1.GetRawAxis(4) || dAbs(Stick1.GetRawAxis(4)) < .2)
		{
			Left.Set(-(float)accel(Stick1, 1, forwardback, sf));
			Right.Set((float)accel(Stick1, 1, forwardback, sf));
		}
		else
		{
			Left.Set((float)accel(Stick1, 4, turn, sf));
			Right.Set((float)accel(Stick1, 4, turn, sf));
		}
	}
	else if (!Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
	{
		Center.Set(-(float)accel(Stick1, 4, strafe, strafesf));
		if(!Stick1.GetRawAxis(0) || dAbs(Stick1.GetRawAxis(0)) < .2)
		{
			Left.Set(-(float)accel(Stick1, 5, forwardback, sf));
			Right.Set((float)accel(Stick1, 5, forwardback, sf));
		}
		else
		{
			Left.Set((float)accel(Stick1, 0, turn, sf));
			Right.Set((float)accel(Stick1, 0, turn, sf));
		}
	}
	else
	{
		Left.Set(0);
		Right.Set(0);
		Center.Set(0);
	}
	if (Stick2.GetRawButton(1))
	{
		dSolenoid.Set(DoubleSolenoid::kForward);
	}
	else if(Stick2.GetRawButton(2))
	{
		dSolenoid.Set(DoubleSolenoid::kReverse);
	}

	Lift.Set(Stick2.GetRawAxis(3) - Stick2.GetRawAxis(2));

	SmartDashboard::PutString("DB/String 0", "Channel 12:");
	std::stringstream ss;
	std::string reading;
	ss<<Pdp.GetCurrent(12);
	ss>>reading;
	SmartDashboard::PutString("DB/String 1", reading);

	SmartDashboard::PutString("DB/String 2", "Channel 13:");
	std::stringstream ss2;
	std::string reading2;
	ss2<<Pdp.GetCurrent(13);
	ss2>>reading2;
	SmartDashboard::PutString("DB/String 3", reading2);

	SmartDashboard::PutString("DB/String 4", "Channel 14:");
	std::stringstream ss3;
	std::string reading3;
	ss3<<Pdp.GetCurrent(14);
	ss3>>reading3;
	SmartDashboard::PutString("DB/String 5", reading3);

	SmartDashboard::PutString("DB/String 6", "Channel 15:");
	std::stringstream ss4;
	std::string reading4;
	ss4<<Pdp.GetCurrent(15);
	ss4>>reading4;
	SmartDashboard::PutString("DB/String 7", reading3);


}
void Team1257Robot::AutonomousInit()
{

}

void Team1257Robot::AutonomousPeriodic()
{

}

void Team1257Robot::TestInit()
{

}

void Team1257Robot::TestPeriodic()
{
	if(Stick1.GetRawButton(1))
		Left.Set(1);
	else
		Left.Set(0);
	if(Stick1.GetRawButton(2))
		Right.Set(1);
	else
		Right.Set(0);
	if(Stick1.GetRawButton(3))
		Center.Set(1);
	else
		Center.Set(0);
}

double Team1257Robot::accel(Joystick& Stick, int axis, double& current, double sf)
{
	double raw = Stick.GetRawAxis(axis);
	if(raw > current && raw > 0) // If speeding up, increment it there, instead of sudden jerk
		current += .05;
	if(raw < current && raw < 0) // Same as above, taking into account negative joyStick1 values
		current -= .05;
	if(dAbs(raw) < dAbs(current)) // If the target speed is lesser, reduce to that instantly, like for stopping
		current = raw;
	if(dAbs(raw) < .2) //Taking into account SLIGHTLY off-centered axes
		current = 0;
	return (current * sf);
}

inline double Team1257Robot::dAbs(double x)
{
	if(x >= 0)
		return x;
	else
		return -x;
}

START_ROBOT_CLASS(Team1257Robot);
