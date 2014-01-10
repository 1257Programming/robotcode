#include "TestBot.h"

void TestBot::Autonomous()
{
	Lcd->Clear();
	Lcd->UpdateLCD();
	Lcd->Printf(DriverStationLCD::kUser_Line1, 1, "Hello, World!");
	Lcd->UpdateLCD();
	while(IsEnabled() && IsAutonomous())
	{
		drive(0.3, 0.3);
		/*
		 * Options:
		 * 1. Use Ultrasonic to sense player station wall.
		 * 2. Use field dimensions to Plan speed and time to travel appropriate distance.
		 */
	}
}

void TestBot::OperatorControl()
{
	// Tele-Op based on Rohan's flexible system of switching joystick mappings at runtime
	// Quite simple, actually.
	while(IsOperatorControl() && IsEnabled())
	{
		// All of this is for driving. We need to have other things to program.
		
	if(Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
		drive(-.5 * Stick1.GetY(), -.5 * Stick1.GetTwist());
	else if(Stick1.GetRawButton(1) && Stick2.GetRawButton(1))
		drive(-.5 * Stick1.GetY(), -.5 * Stick2.GetY());
	else if(Stick1.GetRawButton(1))
		Tank.ArcadeDrive(&Stick1, false);
	else if(Stick2.GetRawButton(1))
		Tank.ArcadeDrive(&Stick2, false);
	else if(Stick1.GetRawButton(11))
		Tank.ArcadeDrive(Stick1.GetX(), Stick1.GetY(), false);
	else if(Stick2.GetRawButton(11))
		Tank.ArcadeDrive(Stick2.GetX(), Stick2.GetY(), false);
	else if(Stick1.GetRawButton(12))
		Tank.ArcadeDrive(Stick1.GetZ(), Stick1.GetTwist(), false);
	else if(Stick2.GetRawButton(12))
		Tank.ArcadeDrive(Stick2.GetZ(), Stick2.GetTwist(), false);
	else
		drive(0, 0);
	}
}

void TestBot::drive(float left, float right)
{
	Tank.SetLeftRightMotorOutputs(-left, -right);
}

inline void TestBot::print(char* message, int line)
{
	switch(line)
	{
	case 1:
		Lcd->Printf(DriverStationLCD::kUser_Line1, 1, message);
	case 2:
		Lcd->Printf(DriverStationLCD::kUser_Line2, 1, message);
	case 3:
		Lcd->Printf(DriverStationLCD::kUser_Line3, 1, message);
	case 4:
		Lcd->Printf(DriverStationLCD::kUser_Line4, 1, message);
	case 5:
		Lcd->Printf(DriverStationLCD::kUser_Line5, 1, message);
	case 6:
		Lcd->Printf(DriverStationLCD::kUser_Line6, 1, message);
	}
}

START_ROBOT_CLASS(TestBot);
