#include "1257Robot.h"

void Team1257Robot::Autonomous()
{
	Timer time;
	time.Start();
	while(IsAutonomous() && IsEnabled())
	{	
		if(time.Get() <= .8)
			Drive.SetLeftRightMotorOutputs(.5, .5);
		else if(time.Get() <= 2.5)
			Drive.SetLeftRightMotorOutputs(.25, .25);
		else
			Drive.SetLeftRightMotorOutputs(0, 0);
		Wait(.005);
	}
}

void Team1257Robot::OperatorControl()
{
	Lcd->Clear();
	Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Teleoperated!");
	Lcd->UpdateLCD();
	while(IsOperatorControl() && IsEnabled())
	{
		drive(); // XBox Controller 1 drives robot
		arms();//XBox Controller 2 conrols arms
	}
}

void Team1257Robot::Test()
{
	
}

void Team1257Robot::drive()
{
	double sf;
	sf = .8;
	if(Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
	{
		Drive.TankDrive(-accel(Stick1, 2, leftspeed, sf), -accel(Stick1, 5, rightspeed, sf), false);
	}
	else if (Stick1.GetRawAxis(3) < 0) // Back button; just one, not both
	{
		if(Stick1.GetRawButton(6))
			sf = 1;
		Drive.ArcadeDrive(-accel(Stick1, 5, speed, sf), -accel(Stick1, 1, curve, sf), false); 
	}
	else if (Stick1.GetRawAxis(3) > 0)
	{
		Drive.ArcadeDrive(-accel(Stick1, 2, speed, sf), -accel(Stick1, 4, curve, sf), false); 
		if(Stick1.GetRawButton(6))
					sf = 1;
	}
	/*else if(Stick1.GetRawButton(5) && Stick1.GetRawAxis(3))
	{
		Drive.ArcadeDrive(1, 1, false); //turbo
	}*/
	else
		Drive.SetLeftRightMotorOutputs(0, 0);
	Wait(0.005);
}

void Team1257Robot::arms()
{
	bool limitswitchenabled = true;
	if(Stick2.GetRawButton(1))
	{
		limitswitchenabled = true;
		Lcd->Printf(DriverStationLCD::kUser_Line3, 1, "LS  Enabled");
	}
	else if(Stick2.GetRawButton(2))
	{
		limitswitchenabled = false;
		Lcd->Printf(DriverStationLCD::kUser_Line3, 1, "LS Disabled");
	}

	if(!LimitSwitch.Get() || Stick2.GetRawAxis(3) > 0 || !limitswitchenabled)
	{
		ArmShoulder1.Set(accel(Stick2, 3, shoulderspeed, .6));
		ArmShoulder2.Set(-accel(Stick2, 3, shoulderspeed, .6));
	}
	else
	{
		ArmShoulder1.Set(0);
		ArmShoulder2.Set(0);
	}
	
	Lcd->Printf(DriverStationLCD::kUser_Line2, 1, "Limit switch: %i", LimitSwitch.Get());
	Lcd->UpdateLCD();
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
	if(dabs(raw) < .2) // Taking into account SLIGHTLY off-centered axes
		current = 0;
	return (current * sf);
}

START_ROBOT_CLASS(Team1257Robot);
