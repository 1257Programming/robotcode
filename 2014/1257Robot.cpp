#include "1257Robot.h"

void Team1257Robot::Autonomous()
{
	Timer time;
	time.Start();
	
	Drive.SetLeftRightMotorOutputs(-.25, -2.5);
	Wait(2.750);
	Drive.SetLeftRightMotorOutputs(0, 0);
	Wait(.500);
	fire.Set(fire.kForward);
	Wait(.500);
	fire.Set(fire.kOff);
	Wait(.100);
}
void Team1257Robot::OperatorControl()
{
	Lcd->Clear();
	Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Teleoperated!");
	Lcd->UpdateLCD();
	while(IsOperatorControl() && IsEnabled())
	{
		drive(); // XBox Controller 1 drives robot
		arms();//XBox Controller 2 conrols arms / shooter
		if(!pressureSense.Get())
			Lcd->Printf(DriverStationLCD::kUser_Line2, 1, "Pressurizing");
		else
			Lcd->Printf(DriverStationLCD::kUser_Line2, 1, "Pressurized!");
		//Lcd->Printf(dLcd::kUser_Line2, 1, "%i", pressureSense.Get());
		Lcd->UpdateLCD();
		
		if(!pressureSense.Get()) 
		{
			compress.Set(compress.kForward);
		}
		else
			compress.Set(compress.kOff);
		shoot();
	}
	
}

void Team1257Robot::shoot()
{
	if(Stick2.GetRawButton(6))
	{
		fire.Set(fire.kForward);
	}
	else if(Stick2.GetRawButton(2))
	{
		Timer time;
		time.Start();
		while(time.Get() <= 0.5)
		{
			Drive.SetLeftRightMotorOutputs(0.5, 0.5);
		}
		Drive.SetLeftRightMotorOutputs(0, 0);
		Wait(.500);
		fire.Set(fire.kForward);
	}
	else if(Stick2.GetRawButton(1))
	{
		Timer time;
		time.Start();
		while(time.Get() <= 0.5)
		{
			Drive.SetLeftRightMotorOutputs(0.5, 0.5);
		}
		Drive.SetLeftRightMotorOutputs(0, 0);
		while(time.Get() >= .5 && time.Get() <= 1)
			fire.Set(fire.kForward);
		fire.Set(fire.kOff);
	}
	else
	{
		fire.Set(fire.kOff);
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
		Drive.TankDrive(accel(Stick1, 2, leftspeed, sf), accel(Stick1, 5, rightspeed, sf), false);
	}
	else if (Stick1.GetRawAxis(3) < 0) // Back button; just one, not both
	{
		if(Stick1.GetRawButton(6))
			sf = 1;
		Drive.ArcadeDrive(accel(Stick1, 5, speed, sf), accel(Stick1, 1, curve, sf), false); 
	}
	else if (Stick1.GetRawAxis(3) > 0)
	{
		Drive.ArcadeDrive(accel(Stick1, 2, speed, sf), accel(Stick1, 4, curve, sf), false); 
	}
	/*else if(Stick1.GetRawButton(5) && Stick1.GetRawAxis(3))
	{
		Drive.ArcadeDrive(1, 1, false); //turbo
	}*/
	else
		Drive.SetLeftRightMotorOutputs(0, 0);
	//while(Stick1.GetRawButton(1))
		//		Drive.SetLeftRightMotorOutputs(-.25, -.25);
}

void Team1257Robot::arms()
{
	ArmShoulder1.Set(accel(Stick2, 3, shoulderspeed, 1));
	ArmShoulder2.Set(-accel(Stick2, 3, shoulderspeed, 1));
	
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
