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
	Lcd->Clear();
	Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Using real robot.");
	Lcd->UpdateLCD();
	while(IsOperatorControl() && IsEnabled())
	{
		if(Stick1.GetRawButton(7) && !realbot)
		{
			realbot = true; //Realbot
			Lcd->Clear();
			Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Using real robot.");
			Lcd->UpdateLCD();
		}
		if(Stick1.GetRawButton(8) && realbot)
		{
			realbot = false; //Testbot
			Lcd->Clear();
			Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Using TestBot.");
			Lcd->UpdateLCD();
		}
		drive(); // XBox Controller 1 drives robot
		arms();//XBox Controller 2 conrols arms
	}
}

void Team1257Robot::Test()
{
	
}

void Team1257Robot::drive()
{
	double sf = .5;
	if(!realbot)
		sf *= -1;
	if(Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
	{
		Drive.TankDrive(-accel(Stick1, 2, leftspeed, sf), -accel(Stick1, 5, rightspeed, sf), false);
	}
	else if (Stick1.GetRawAxis(3)) // Back button; just one, not both
	{
		Drive.ArcadeDrive(-accel(Stick1, 5, speed, sf), -accel(Stick1, 1, curve, sf), false);
	}
}

void Team1257Robot::arms()
{
	double sf =.5;
	if(Stick2.GetRawButton(5) && Stick2.GetRawButton(6))
		sf = 1;
	LeftArm.Set(-accel(Stick2, 1, leftarmspeed, sf));
	RightArm.Set(accel(Stick2, 4, rightarmspeed, sf));
	DigitalInput LimitSwitch(1);
	if(!LimitSwitch.Get())
		ArmShoulder.Set(0);
	else ArmShoulder.Set(accel(Stick2, 3, shoulderspeed, .4));
}

double Team1257Robot::accel(Joystick& stick, int axis, double& current, double sf)
{
	double raw = stick.GetRawAxis(axis);
	if(raw > current && raw > 0) // If speeding up, increment it there, instead of sudden jerk
		current += .05;
	if(raw < current && raw < 0) // Same as above, taking into account negative joystick values
		current -= .05;
	if(dabs(raw) < dabs(current)) // If the target speed is lesser, reduce to that instantly, like for stopping
		current = raw;
	if(dabs(raw) < .1) // Taking into account SLIGHTLY off-centered axes
		current = 0;
	return (current * sf);
}

START_ROBOT_CLASS(Team1257Robot);

