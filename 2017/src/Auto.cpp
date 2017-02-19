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
}

void Robot::AutonomousPeriodic()
{
	/*RobotTimer.Reset();
	RobotTimer.Start();

	while(RobotTimer.HasPeriodPassed(1.0))
	{
		DriveTrain.ArcadeDrive(.4, 0);
	}
	RobotTimer.Stop();
	DriveTrain.SetLeftRightMotorOutputs(0, 0);*/
	while(!hasAutoRun)
	{
		// Score in left peg, then cross baseline
		if(SmartDashboard::GetBoolean("DB/Button 0", false))
		{
			SmartDashboard::PutString("Auto Scheme", "Left Peg 1");
			RobotTimer.Reset();
			RobotTimer.Start();

			while(!RobotTimer.HasPeriodPassed(1.9))
			{
				DriveTrain.ArcadeDrive(0.6, 0);
			}
			RobotTimer.Reset();
			RobotTimer.Start();

			while(!RobotTimer.HasPeriodPassed(0.8))
			{
				DriveTrain.ArcadeDrive(0, -0.5);
			}
			DriveTrain.SetLeftRightMotorOutputs(0, 0);
			//DriveRobot(78.5);
			//DriveRobot(39.84);

			ScoringSequence();
			RobotTimer.Reset();
			RobotTimer.Start();

			while(!RobotTimer.HasPeriodPassed(1.5))
			{
				DriveTrain.ArcadeDrive(0.5, 0);
			}
			ScoringSequence();
			RobotTimer.Reset();
			RobotTimer.Start();

			while(!RobotTimer.HasPeriodPassed(0.5))
			{
				DriveTrain.ArcadeDrive(0.3, 0);
			}
			DriveTrain.SetLeftRightMotorOutputs(0, 0);
		}
		// Score in center peg on red alliance, then cross baseline
		else if(SmartDashboard::GetBoolean("DB/Button 1", false))
		{
			DriveRobot(24.00);
			TurnRobot(-30);
			DriveRobot(26.75);
			TurnRobot(30);
			DriveRobot(24.71);

			//ScoringSequence();
		}
		// Score in center peg on blue alliance, then cross baseline
		else if(SmartDashboard::GetBoolean("DB/Button 2", false))
		{
			DriveRobot(24.00);
			TurnRobot(30);
			DriveRobot(26.75);
			TurnRobot(-30);
			DriveRobot(24.71);

			//ScoringSequence();
		}
		// Score in right peg, then cross baseline
		else if(SmartDashboard::GetBoolean("DB/Button 3", false))
		{
			DriveRobot(176.7);
			TurnRobot(-120);
			DriveRobot(36.07);

			//ScoringSequence();
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
		hasAutoRun = true;
		DriveTrain.SetLeftRightMotorOutputs(0, 0);
		GearSlide.Set(0);
		ClimbMotor.Set(0);
	}
}
