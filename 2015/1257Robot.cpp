#include "1257Robot.h"

using namespace std;

template<typename T>
std::string ToString(T t)
{
	std::stringstream ss; // Declare string stream
	ss << t; // Insert numerical value into stream
	string res = ""; // Declare std::string to store the number string
	ss >> res; // Extract number as std::string
	return res; // Return the result string
}


Team1257Robot::Team1257Robot(): // Initialization of objects based on connection ports
	Left(0), Right(1), Center(2), Lift(3), // PWM
	Stick1(0), Stick2(1), //Driver Station
	dSolenoid(4, 5), // PCM
	bottomlimit(0), toplimit(1), // Digital IO
	angle(0) // Analog Input
{

}

void Team1257Robot::TeleopInit()
{
	//CameraServer::GetInstance()->StartAutomaticCapture("cam1"); // Automatically send camera images
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
	if (Stick2.GetRawButton(5))
	{
		dSolenoid.Set(DoubleSolenoid::kForward);
	}
	else if(Stick2.GetRawButton(6))
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
	// Monitor current drawn by motors by printing PDP current values through CAN
}

bool ran = false; // Variable to ensure that Autonomous only runs once
void Team1257Robot::AutonomousInit()
{
	ran = false; // Makes sure that "ran" is set to false initially
}

void Team1257Robot::AutonomousPeriodic()
{
	while(!ran) // Only run autonomous while the "ran" variable is false
	{
		if(!bottomlimit.Get() && !toplimit.Get()) // Makes sure that neither switch is tripped
		{
			dSolenoid.Set(DoubleSolenoid::kReverse); // Close arms
			Wait(.2); // Wait for .2 seconds
			Lift.Set(.9); // Lift arms with containers grasped
			Wait(1.75); // Continue for 1.75 seconds
			Lift.Set(0);// Stop lifting; high enough
			Right.Set(-.5); // Move forward
			Left.Set(.5); // Move forward
			Wait(.85); // Do so for .85 seconds
			Right.Set(0); // STOP!
			Left.Set(0); // STOP!
			Lift.Set(-.55); // Set container down, then keep lowering to below tote
			Wait(0.5); // Do so for .5 seconds
			dSolenoid.Set(DoubleSolenoid::kForward); // Open the arms
			Right.Set(.3); // Move backwards
			Left.Set(-.3); // To avoid having the arms get caught on tote
			Wait(.4); //keep going back for .4 secons
			Right.Set(0); // Stop moving forward
			Left.Set(0); // Stop moving forward
			Center.Set(-.5); // Slide into place to pick up tote
			Wait(.5); // Do so for .5 seconds
			Center.Set(0); // STOP
			Left.Set(0); // THE
			Right.Set(0); // DRIVE
			Lift.Set(-.55); // Just a reminder that the elevator should STILL be going down
			Wait(3); // JUST 3 MORE SECONDS
			dSolenoid.Set(DoubleSolenoid::kReverse); // Close the arms to grasp the tote
			Lift.Set(.35); // Move the elevator up to lift the tote and container just a bit
			Wait(1); // A second should be long enough

			while(TestAngle(60)) // Make a 90 degree turn, stopping the rotation after a 60-degree reading to account more momentum
			{
				Right.Set(-.5);
				Left.Set(-.5);
			}
			Right.Set(-.65); // Move forward
			Left.Set(.65); // Into AUTO Zone
			Wait(1.6); // Hold high speed for 1.6 seconds
			Right.Set(-0.5); // Reduce speed
			Left.Set(0.5); // to .5 output
			Wait(.3); // hold for just .3 seconds
			Right.Set(-0.3); // Now reduce again
			Left.Set(0.3); // to .3 output
			Wait(.4); // Hold for .4 seconds
			Right.Set(-0.1); // Slow again
			Left.Set(0.1); // To .1 speed (may not actually move robot, just to help slow to a stop)
			Wait(.5); // Hold for .5 seconds
			Right.Set(0); // Stop the robot
			Left.Set(0); // In the AUTO ZONE
			Lift.Set(0); // AUTO is finished
		}
	else
		Lift.Set(0); // Make sure elevator is stopped
	ran = true; // End loop after just one iteration
	}
	// After ran = true, although AutonomousPeriodic continues to run, nothing happens
}

void Team1257Robot::TestInit()
{
	angle.Reset();
}

bool Team1257Robot::TestAngle(int theta)
{
	float a = angle.GetAngle();
	if((int)dAbs(a) < theta)
	{
 		return true;
	}

	else
	{
		return false;
	}
}

void Team1257Robot::TestPeriodic()
{
	float a = angle.GetAngle();
	if((int)dAbs(a) < 60)
	{
		Right.Set(-.5);
		Left.Set(-.5);
	}

	else
	{
		Right.Set(0);
		Left.Set(0);
	}

	SmartDashboard::PutString("DB/String 1", ToString((int)a));
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
