#include "Robot.h"

void Robot::DisabledInit()
{
  	//Init motors/gear
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	SmartDashboard::PutString("DB/String 0", "Left Peg");
	SmartDashboard::PutString("DB/String 2", "Center Peg");
	SmartDashboard::PutString("DB/String 3", "Right Peg");
	SmartDashboard::PutString("DB/String 4", "Cross Baseline");
}

void Robot::AutonomousInit()
{
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
	ClimbRelease.SetAngle(180);
	LeftFlap.Set(DoubleSolenoid::kForward);
	RightFlap.Set(DoubleSolenoid::kForward);
}

void Robot::AutonomousPeriodic()
{
	while(!hasAutoRun) //Prevents auto from running more than once
	{
		// Score in left peg
		if(SmartDashboard::GetBoolean("DB/Button 0", false))
		{
			DriveFor(1.9);
			TurnRobot(60);
			DriveFor(1.5);
			ScoringSequence();
		}
		// Score in center peg
		else if(SmartDashboard::GetBoolean("DB/Button 2", false))
		{
			DriveFor(1.6);
			ScoringSequence();
		}
		// Score in right peg
		else if(SmartDashboard::GetBoolean("DB/Button 3", false))
		{
			DriveFor(1.9);
			TurnRobot(-60);
			DriveFor(1.5);
			ScoringSequence();
		}
		// Cross baseline
		else if(SmartDashboard::GetBoolean("DB/Button 4", false))
		{
			DriveFor(2);
		}
		else
		{
			ArcadeDrive(0, 0);
			GearSlide.Set(0);
			ClimbMotor.Set(0);
		}
		hasAutoRun = true;
		ArcadeDrive(0, 0);
		GearSlide.Set(0);
		ClimbMotor.Set(0);
	}
}

// Drive forward at maxSpeed for a specific amount of time
void Robot::DriveFor(double seconds)
{
	double maxSpeed = 0.6;
	RobotTimer.Reset();
	RobotTimer.Start();
	while(!RobotTimer.HasPeriodPassed(seconds))
	{
		ArcadeDrive(maxSpeed, 0);
	}
	ArcadeDrive(0, 0);
	RobotTimer.Stop();
}

// Turn, angle is from -180 to 180 degrees
void Robot::TurnRobot(double angle)
{
	double maxTurnSpeed = 0.6;
	Gyro.Reset();

	// - angle means turn counterclockwise, + angle means turn clockwise
	if(angle > 0)
	{
		while (Gyro.GetAngle() < angle)
		{
			ArcadeDrive(0, maxTurnSpeed, false);
		}
	}
	else if (angle < 0)
	{

		while (Gyro.GetAngle() > angle)
		{
			ArcadeDrive(0, -maxTurnSpeed, false);
		}
	}
	ArcadeDrive(0, 0);
}
