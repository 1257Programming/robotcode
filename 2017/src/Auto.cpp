#include "Robot.h"

void Robot::DisabledInit()
{
  	//Init motors/gear
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	SmartDashboard::PutString("DB/String 0", "Left Peg");
	SmartDashboard::PutString("DB/String 1", "Test");
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
	LeftFlap.Set(DoubleSolenoid::kReverse);
	RightFlap.Set(DoubleSolenoid::kReverse);
	Gyro.Reset();

	if(SmartDashboard::GetBoolean("DB/Button 0", false))
	{
		SmartDashboard::PutString("Auto Status", "Scoring on the left peg");
		DriveFor(1.0);
		TurnRobot(60);
		DriveFor(0.4);
		ScoringSequence();
	}
	//Test Code
	else if(SmartDashboard::GetBoolean("DB/Button 1", false))
	{
		TurnRobot(-60);
	}
	// Score in center peg
	else if(SmartDashboard::GetBoolean("DB/Button 2", false))
	{
		SmartDashboard::PutString("Auto Status", "Scoring on the center peg");
		DriveFor(0.75);
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
	else
	{
		ArcadeDrive(0, 0);
		GearSlide.Set(0);
		ClimbMotor.Set(0);
	}

	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
	SmartDashboard::PutString("Auto Status", "Autonomous completed");
}

void Robot::AutonomousPeriodic()
{
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
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

    SmartDashboard::PutString("Auto Status", "Turning to angle");

    while (round(Gyro.GetAngle()) != angle)
    {
    	SmartDashboard::PutNumber("Gyro", Gyro.GetAngle());
        while (Gyro.GetAngle() < angle)
        {
        	SmartDashboard::PutNumber("Gyro", Gyro.GetAngle());
            SetDriveMotors(speed, -speed);
        }
        while (Gyro.GetAngle() > angle)
        {
        	SmartDashboard::PutNumber("Gyro", Gyro.GetAngle());
            SetDriveMotors(-speed, speed);
        }
    }

    SmartDashboard::PutString("Auto Status", "Success to angle");
    SetDriveMotors(0, 0);
}
