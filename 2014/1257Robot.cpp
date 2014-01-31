#include "1257Robot.h"

void Team1257Robot::Autonomous()
{
	while(IsAutonomous() && IsEnabled())
	{
		Drive.SetLeftRightMotorOutputs(-.3, -.3);
	}
}

void Team1257Robot::OperatorControl()
{
	while(IsOperatorControl() && IsEnabled())
	{
		drive(); // XBox Controller 1 drives robot
		//XBox Controller 2 conrols arms
		if(Stick2.GetRawAxis(1) > .1 || Stick2.GetRawAxis(1) < -.1)
				LeftArm.Set(.2 * Stick2.GetRawAxis(1));
		if(Stick2.GetRawAxis(4) > .1 || Stick2.GetRawAxis(4) < -.1)
						LeftArm.Set(.2 * Stick2.GetRawAxis(1));
		double shoulderspeed = (Stick2.GetRawAxis(2) + Stick2.GetRawAxis(5)) / 2;
		if(shoulderspeed > .1 || shoulderspeed < -.1)
			ArmShoulder.Set(.2 * shoulderspeed);
	}
}

void Team1257Robot::Test()
{
	
}

void Team1257Robot::drive()
{
	double reading1;
	double reading2;
	
	if(Stick1.GetRawAxis(3))
	{
		reading1 = -Stick1.GetRawAxis(5); //raw speed
		reading2 = Stick1.GetRawAxis(1); // raw curve
		
		if(reading1 > speed1)
			speed1 += .05;
		if(reading1 < .1 && reading1 > -.1)
			speed1 = 0;
		if(reading2 > speed2)
			speed2 += .05;
		if(reading2 < .1 && reading2 > -.1)
			speed2 = 0;
		
		Drive.ArcadeDrive(-.5 * speed1, .5 * speed2, false);

	}
	else if(Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
	{
		reading1 = -Stick1.GetRawAxis(2);
		reading2 = -Stick1.GetRawAxis(5);
		
		if(reading1 > speed1)
			speed1 += .05;
		if(reading1 < .1 && reading1 > -.1)
			speed1 = 0;
		if(reading2 > speed2)
			speed2 += .05;
		if(reading2 < .1 && reading2 > -.1)
			speed2 = 0;
		Drive.SetLeftRightMotorOutputs(-.5 * speed1, -.5 * speed2);
	}
	else
		Drive.SetLeftRightMotorOutputs(0, 0);
}

START_ROBOT_CLASS(Team1257Robot);
