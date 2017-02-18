#include "Robot.h"

void Robot::DisabledInit()
{
  	//Init motors/gear
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	SmartDashboard::PutString("DB/String 0", "Left Gear");
	SmartDashboard::PutString("DB/String 1", "Center Gear");
	SmartDashboard::PutString("DB/String 2", "Right Gear");
	SmartDashboard::PutString("DB/String 3", "Cross Baseline");
}

void Robot::AutonomousInit()
{
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	ClimbRelease.SetAngle(180);

	if(SmartDashboard::GetBoolean("DB/Button 0", false))
	{
		// Score gear on left peg
	}
	else if(SmartDashboard::GetBoolean("DB/Button 1", false))
	{
		// Score gear on center peg
	}
	else if(SmartDashboard::GetBoolean("DB/Button 2", false))
	{
		// Score gear on right peg
	}
	else if(SmartDashboard::GetBoolean("DB/Button 3", false))
	{
		// Cross baseline
	}
	else
	{
		SetDriveMotors(0, 0);
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
	//TODO use encoders better, implement SmartDashboard for choices, and find out about squared inputs
	//WIP NavX code here lol it doesn't work
	ScoringSequence();
}
