#include "1257Robot.h"
#define VOLT_TO_PRESS(voltage) (((voltage - .6)/1.8)*60)

void Team1257Robot::Autonomous()
{
	Lcd->Clear();
	Lcd->UpdateLCD();
	NetworkTable* table = NetworkTable::GetTable("SmartDashboard");
	table->PutNumber("ROBORUN", 1);
	Wait(1);
	table->PutNumber("ROBORUN", 0);
	Wait(.5);
	bool result = table->GetBoolean("RESULT");
	if(result)
	{
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Yay!");
		Lcd->UpdateLCD();
		drivetime(.6, .6, 2);
		Wait(.5);
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Firing!");
		Lcd->UpdateLCD();
		fire.Set(fire.kForward);
		Wait(1);
		fire.Set(fire.kOff);
	}
	else
	{
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "NO!!");
		Lcd->UpdateLCD();
		drivetime(.6, .6, 2);
		Wait(3);
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Firing!");
		Lcd->UpdateLCD();
		fire.Set(fire.kForward);
		Wait(1);
		fire.Set(fire.kOff);
	}
}
/*
void Team1257Robot::Autonomous()
{
	Timer time;
	time.Start();
	while(IsAutonomous() && IsEnabled() && time.Get() <= 10)
	{
		if(time.Get() <= 2.25)
			Drive.SetLeftRightMotorOutputs(.4, .4); //Make faster for Monty for more time for Vision
		else if(time.Get() <= 3.25)
		{
			Drive.SetLeftRightMotorOutputs(0, 0);
		}

		else if(time.Get() <= 3.75)
		{
			fire.Set(fire.kForward);
			Wait(.5);
			fire.Set(fire.kOff);
		}
	}
	time.Stop();
}
*/
	
	/*
	Lcd->Clear();
	Lcd->UpdateLCD();
	NetworkTable* table = NetworkTable::GetTable("SmartDashboard");
	Timer time;
	time.Start();
	while(IsAutonomous() && IsEnabled() && time.Get() <= 10)
	{
		if(time.Get() <= 2.25)
			Drive.SetLeftRightMotorOutputs(.25, .25); //Make faster for Monty for more time for Vision
		else if(time.Get() <= 3.25)
		{
			Drive.SetLeftRightMotorOutputs(0, 0);
		}

		else if((time.Get() <= 3.75))
		{
				//fire.Set(fire.kForward);
			Wait(.5);
				//fire.Set(fire.kOff);
		}
	}
}*/
void Team1257Robot::OperatorControl()
{
	Lcd->Clear();
	Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Teleoperated!");
	
	Lcd->UpdateLCD();
	while(IsOperatorControl() && IsEnabled())
	{
		Lcd->Printf(DriverStationLCD::kUser_Line3, 1, "%f", transd.GetVoltage());
		Lcd->Printf(DriverStationLCD::kUser_Line4,1,"%f", VOLT_TO_PRESS(transd.GetVoltage()));
		drive(); // XBox Controller 1 drives robot
		arms();//XBox Controller 2 conrols arms / shooter
		if(!pressureSense.Get())
		{
			Lcd->Printf(DriverStationLCD::kUser_Line2, 1, "Pressurizing");
			compress.Set(compress.kForward);
		}
		else
		{
			Lcd->Printf(DriverStationLCD::kUser_Line2, 1, "Pressurized!");
			compress.Set(compress.kOff);
		}
		//Lcd->Printf(dLcd::kUser_Line2, 1, "%i", pressureSense.Get());
		
		Lcd->UpdateLCD();
			
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
			fire.Set(fire.kForward);
			Wait(.15);
			fire.Set(fire.kOff);
	}
	else if(Stick2.GetRawButton(1))
	{
				fire.Set(fire.kForward);
				Wait(.1);
				fire.Set(fire.kOff);
	}
	else
	{
		fire.Set(fire.kOff);
	}
}

void Team1257Robot::Test()
{
	Lcd->Clear();
	Lcd->UpdateLCD();
	NetworkTable* table = NetworkTable::GetTable("SmartDashboard");
	table->PutNumber("ROBORUN", 1);
	Wait(1);
	table->PutNumber("ROBORUN", 0);
	Wait(.5);
	bool result = table->GetBoolean("RESULT");
	if(result)
	{
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Yay!");
		Lcd->UpdateLCD();
		Drive.SetLeftRightMotorOutputs(.6, .6); //Make faster for Monty for more time for Vision
		Wait(2);
		Drive.SetLeftRightMotorOutputs(0, 0);
		Wait(.5);
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Firing!");
		Lcd->UpdateLCD();
		//fire.Set(fire.kForward);
		Wait(1);
		fire.Set(fire.kOff);
	}
	else
	{
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "NO!!");
		Lcd->UpdateLCD();
		Drive.SetLeftRightMotorOutputs(.6, .6);
		Wait(2);
		Drive.SetLeftRightMotorOutputs(0, 0);
		Wait(3);
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Firing!");
		Lcd->UpdateLCD();
		//fire.Set(fire.kForward);
		Wait(1);
		fire.Set(fire.kOff);
	}
}


void Team1257Robot::drive()
{
	double sf;
	sf = 1.0;
	if(Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
	{
		Drive.TankDrive(-accel(Stick1, 2, leftspeed, sf), -accel(Stick1, 5, rightspeed, sf), false);
	}
	else if (Stick1.GetRawAxis(3) < 0) // Back button; just one, not both
	{
		Drive.ArcadeDrive(-accel(Stick1, 5, speed, sf), -accel(Stick1, 1, curve, sf), false); 
	}
	else if (Stick1.GetRawAxis(3) > 0)
	{
		Drive.ArcadeDrive(-accel(Stick1, 2, speed, sf), -accel(Stick1, 4, curve, sf), false); 
	}
	
	else if(Stick1.GetRawButton(1))
	{
		Drive.ArcadeDrive(-accel(Stick1, 2, speed, sf), -accel(Stick1, 1, curve, sf), false);
	}
	
	else
		Drive.SetLeftRightMotorOutputs(0, 0);
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
