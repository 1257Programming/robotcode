#include "1257Robot.h"

using namespace std;

template<typename T>
std::string ToString(T t)
{
	std::stringstream ss;
	ss << t;
	string res = "";
	ss >> res;
	return res;
}


Team1257Robot::Team1257Robot():
	Left(0), Right(1), Center(2), Lift(3), Stick1(0), Stick2(1), dSolenoid(4, 5), Ultra(0),
	bottomlimit(0), toplimit(1)
{
	Lw = LiveWindow::GetInstance();
}

Team1257Robot::~Team1257Robot()
{

}

void Team1257Robot::TeleopInit()
{
	CameraServer::GetInstance()->StartAutomaticCapture("cam1");
}

void Team1257Robot::TeleopPeriodic()
{
	double sf = .8;
	double strafesf = 1;

	if (Stick1.GetRawButton(5) && !Stick1.GetRawButton(6)) // Back button; just one, not both
	{
		Center.Set(-(float)accel(Stick1, 0, strafe, strafesf, 0.2));
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
		Center.Set(-(float)accel(Stick1, 4, strafe, strafesf, 0.2));
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
	if (Stick2.GetRawButton(6))
	{
		dSolenoid.Set(DoubleSolenoid::kForward);
	}
	else if(Stick2.GetRawButton(5))
	{
		dSolenoid.Set(DoubleSolenoid::kReverse);
	}
	else
		dSolenoid.Set(DoubleSolenoid::kOff);

	double liftval = Stick2.GetRawAxis(3) - Stick2.GetRawAxis(2);

	if((!bottomlimit.Get() || liftval > 0) && (!toplimit.Get() || liftval < 0))
		Lift.Set(liftval);
	else
		Lift.Set(0);

	SmartDashboard::PutString("DB/String 0", "Channel 12:");
	SmartDashboard::PutString("DB/String 1", ToString(Pdp.GetCurrent(12)));

	SmartDashboard::PutString("DB/String 2", "Channel 13:");
	SmartDashboard::PutString("DB/String 3", ToString(Pdp.GetCurrent(13)));

	SmartDashboard::PutString("DB/String 4", "Channel 14:");
	SmartDashboard::PutString("DB/String 5", ToString(Pdp.GetCurrent(14)));

	SmartDashboard::PutString("DB/String 6", "Channel 15:");
	SmartDashboard::PutString("DB/String 7", ToString(Pdp.GetCurrent(15)));
}
void Team1257Robot::AutonomousInit()
{

}

void Team1257Robot::AutonomousPeriodic()
{
	if(!bottomlimit.Get() && !toplimit.Get())
	{
		dSolenoid.Set(DoubleSolenoid::kForward);
		Wait(.2);
		Lift.Set(.9);
		Wait(2.5);
		Lift.Set(0);
		Right.Set(-.5);
		Left.Set(.5);
		Wait(1);
		Right.Set(0);
		Left.Set(0);
		Lift.Set(-.55);
		Wait(1);
		dSolenoid.Set(DoubleSolenoid::kReverse);
		Wait(2);
		Center.Set(-.5);
		Right.Set(.3);
		Left.Set(-.3);
		Wait(.5);
		Center.Set(0);
		Left.Set(0);
		Right.Set(0);
		Lift.Set(-.55);
		Wait(2);
		dSolenoid.Set(DoubleSolenoid::kForward);
		Wait(1);
		Lift.Set(.35);
		Wait(1);
		Right.Set(.5);
		Left.Set(.5);
		Wait(1.3);
		Right.Set(-.75);
		Left.Set(.75);
		Wait(2.5);
		Right.Set(0);
		Left.Set(0);
	}
	else
		Lift.Set(0);

}

void Team1257Robot::TestInit()
{

}

void Team1257Robot::TestPeriodic()
{
	Right.Set(.5);
	Left.Set(.5);
}

void Team1257Robot::DisabledPeriodic()
{

}

double Team1257Robot::accel(Joystick& Stick, int axis, double& current, double sf, double inc)
{
	double raw = Stick.GetRawAxis(axis);
	if(raw > current && raw > 0) // If speeding up, increment it there, instead of sudden jerk
		current += inc;
	if(raw < current && raw < 0) // Same as above, taking into account negative joyStick1 values
		current -= inc;
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
