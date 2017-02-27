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
	SmartDashboard::PutString("DB/String 5", "Turn 90, Drive Straight for 1s");
}

void Robot::AutonomousInit()
{
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
	ClimbRelease.SetAngle(180);
	LeftFlap.Set(DoubleSolenoid::kForward);
	RightFlap.Set(DoubleSolenoid::kForward);
	Gyro.Reset();
}

void Robot::AutonomousPeriodic()
{
	while(!hasAutoRun) //Prevents auto from running more than once
	{
		// Score in left peg
		if(SmartDashboard::GetBoolean("DB/Button 0", false))
		{
			SmartDashboard::PutString("Auto Status", "Scoring on the left peg");
			DriveFor(1.9);
			TurnRobot(60);
			DriveFor(1.5);
			ScoringSequence();
		}
		// Score in center peg
		else if(SmartDashboard::GetBoolean("DB/Button 2", false))
		{
			SmartDashboard::PutString("Auto Status", "Scoring on the center peg");
			DriveFor(1.6);
			ScoringSequence();
		}
		// Score in right peg
		else if(SmartDashboard::GetBoolean("DB/Button 3", false))
		{
			SmartDashboard::PutString("Auto Status", "Scoring on the right peg");
			DriveFor(1.9);
			TurnRobot(-60);
			DriveFor(1.5);
			ScoringSequence();
		}
		// Cross baseline
		else if(SmartDashboard::GetBoolean("DB/Button 4", false))
		{
			SmartDashboard::PutString("Auto Status", "Crossing the Baseline");
			DriveFor(2);
		}
		else if(SmartDashboard::GetBoolean("DB/Button 5", false))
		{
			DriveFor(1);
			TurnRobot(180);
			DriveFor(1);
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
	SmartDashboard::PutString("Auto Status", "Autonomous completed");
}

// Drive forward at maxSpeed for a specific amount of time
void Robot::DriveFor(double seconds, double speed)
{
	RobotTimer.Reset();
	RobotTimer.Start();
	while(!RobotTimer.HasPeriodPassed(seconds))
	{
		ArcadeDrive(speed, 0);
	}
	ArcadeDrive(0, 0);
	RobotTimer.Stop();
}

// Turn, angle is from -180 to 180 degrees
void Robot::TurnRobot(double angle, double speed, bool reset)
{
    if (reset)
    {
        Gyro.Reset();
    }

    while (true)
    {
        while (Gyro.GetAngle() < angle)
        {
            SetDriveMotors(speed, -speed);
        }
        while (Gyro.GetAngle() > angle)
        {
            SetDriveMotors(-speed, speed);
        }
        if (round(Gyro.GetAngle()) == angle)
        {
            break;
        }
    }
    SetDriveMotors(0, 0);
}
