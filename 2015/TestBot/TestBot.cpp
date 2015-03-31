#include "TestBot.h"
#include <sstream>
#include <ctime>
template <typename T>

std::string ToString(T t) //convert something else to a string
{
	std::stringstream ss; // Declare string stream
	ss << t; // Insert numerical value into stream
	std::string res = ""; // Declare std::string to store the number string
	ss >> res; // Extract number as std::string
	return res; // Return the result string
}
double t, dt, a, v, x;
timespec ts;
void TestBot::TeleopInit()
{
	clock_gettime(CLOCK_REALTIME, &ts);
	t = ts.tv_nsec / 1000000000;
	a = 0;
	v = 0;
	x = 0;
}
void TestBot::TeleopPeriodic()
{
	clock_gettime(CLOCK_REALTIME, &ts);
	double sf = 1;
	a = bia.GetX() * 9.8;
	dt = ts.tv_nsec/1000000000 - t;
	v += a*dt;
	x += v*dt;
	SmartDashboard::PutNumber("x", x);
	SmartDashboard::PutNumber("v", v);
	SmartDashboard::PutNumber("a", a);
	SmartDashboard::PutNumber("t", t);
	SmartDashboard::PutNumber("dt", dt);
	if(Stick.GetRawButton(5) && Stick.GetRawButton(6))
	{
		Drive.TankDrive(accel(Stick, 1, leftspeed, sf), accel(Stick, 5, rightspeed, sf), false);
	}
	else if (Stick.GetRawAxis(3) > 0) // Back button; just one, not both
	{
		Drive.ArcadeDrive(accel(Stick, 5, speed, sf), accel(Stick, 0, curve, sf), false);
	}
	else if (Stick.GetRawAxis(2) > 0)
	{
		Drive.ArcadeDrive(accel(Stick, 1, speed, sf), accel(Stick, 4, curve, sf), false);
	}
	else if (Stick.GetRawButton(1))
		Drive.ArcadeDrive(accel(Stick, 1, speed, sf), accel(Stick, 0, curve, sf), false);
	else
		Drive.SetLeftRightMotorOutputs(0, 0);
	clock_gettime(CLOCK_REALTIME, &ts);
	t = ts.tv_nsec/1000000000;
}
bool a_robot;
bool a_tote;
bool a_container;
bool a_start;
void TestBot::TestInit()
{
	SmartDashboard::init();
	a_robot = SmartDashboard::GetBoolean("AUTO_ROBOT", true);
	a_tote = SmartDashboard::GetBoolean("AUTO_TOTE", false);
	a_container = SmartDashboard::GetBoolean("AUTO_CONTAINER", true);
	a_start = SmartDashboard::GetBoolean("AUTO_START", true);
}
void TestBot::TestPeriodic()
{
	if(a_start)
	{
		if(a_robot)
		{
			if(a_container)
			{
				//Here is where we grab it
				if(a_tote)
				{
					//tote
				}
			}
			else if(a_tote)
			{
				//Tote
			}
			//Drive
		}
	}
	else if (a_robot)
	{
		if(a_container)
		{
			//Canburglar
		}
		else
		{
			//Drive
		}
	}
	//Stop
}
CycleCounter count;
void TestBot::AutonomousInit()
{
	SmartDashboard::init();
	SmartDashboard::PutBoolean("AUTO_ROBOT", true);
	SmartDashboard::PutBoolean("AUTO_TOTE", false);
	SmartDashboard::PutBoolean("AUTO_CONTAINER", true);
	SmartDashboard::PutBoolean("AUTO_START", true);
	count.Start();
}
void TestBot::AutonomousPeriodic()
{
	SmartDashboard::PutNumber("time", count.Get());
}
double TestBot::accel(Joystick& stick, int axis, double& current, double sf)
{
	double raw = stick.GetRawAxis(axis);
	if(raw > current && raw > 0) // If speeding up, increment it there, instead of sudden jerk
		current += .05;
	if(raw < current && raw < 0) // Same as above, taking into account negative joystick values
		current -= .05;
	if(dabs(raw) < dabs(current)) // If the target speed is lesser, reduce to that instantly, like for stopping
		current = raw;
	if(dabs(raw) < .2) // Taking into account SLIGHTLY off-centered axes
		current = 0;
	return (current * sf);
}


START_ROBOT_CLASS(TestBot);
