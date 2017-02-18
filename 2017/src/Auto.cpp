#include "Robot.h"

void Robot::DisabledInit()
{
  	//Init motors/gear
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	SmartDashboard::PutString("DB/String 0", "Left Peg");
	SmartDashboard::PutString("DB/String 1", "Center Peg Red");
	SmartDashboard::PutString("DB/String 2", "Center Peg Blue");
	SmartDashboard::PutString("DB/String 3", "Right Peg");
	SmartDashboard::PutString("DB/String 4", "Cross Baseline");
}

void Robot::AutonomousInit()
{
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	ClimbRelease.SetAngle(180);

	// Score in left peg, then cross baseline
	if(SmartDashboard::GetBoolean("DB/Button 0", false))
	{
		DriveRobot(178.6);
		TurnRobot(120);
		DriveRobot(39.84);

		ScoringSequence();

		DriveRobot(-66.07);
		TurnRobot(-120);
		DriveRobot(194.6);
	}
	// Score in center peg on red alliance, then cross baseline
	else if(SmartDashboard::GetBoolean("DB/Button 1", false))
	{
		DriveRobot(24.00);
		TurnRobot(-30);
		DriveRobot(26.75);
		TurnRobot(30);
		DriveRobot(24.71);

		ScoringSequence();

		DriveRobot(-24.71);
		TurnRobot(-90);
		DriveRobot(107.3);
		TurnRobot(90);
		DriveRobot(339.1);
	}
	// Score in center peg on blue alliance, then cross baseline
	else if(SmartDashboard::GetBoolean("DB/Button 2", false))
	{
		DriveRobot(24.00);
		TurnRobot(30);
		DriveRobot(26.75);
		TurnRobot(-30);
		DriveRobot(24.71);

		ScoringSequence();

		DriveRobot(-24.71);
		TurnRobot(-90);
		DriveRobot(107.3);
		TurnRobot(90);
		DriveRobot(339.1);
	}
	// Score in right peg, then cross baseline
	else if(SmartDashboard::GetBoolean("DB/Button 3", false))
	{
		DriveRobot(176.7);
		TurnRobot(-120);
		DriveRobot(36.07);

		ScoringSequence();

		DriveRobot(-51.35);
		TurnRobot(75);
		DriveRobot(285.6);
		TurnRobot(45);
	}
	// Cross baseline
	else if(SmartDashboard::GetBoolean("DB/Button 4", false))
	{
		DriveRobot(378.3);
	}
	else
	{
		DriveTrain.SetLeftRightMotorOutputs(0, 0);
		GearSlide.Set(0);
		ClimbMotor.Set(0);
	}
}

void Robot::AutonomousPeriodic()
{
	//Init motors/gear
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
	//TODO use encoders better, and find out about squared inputs
}
