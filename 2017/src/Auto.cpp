#include "Robot.h"

void Robot::DisabledInit()
{
  	//Init motors/gear
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	SmartDashboard::PutString("DB/String 0", "Left Peg");
	SmartDashboard::PutString("DB/String 2", "Center Peg");
	SmartDashboard::PutString("DB/String 3", "Right Peg");
	SmartDashboard::PutString("DB/String 4", "Cross Baseline");
}

void Robot::AutonomousInit()
{
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
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
			DriveForward(81.2);
			TurnRobot(60);
			DriveForward(27);

			ScoringSequence();
		}
		// Score in center peg
		else if(SmartDashboard::GetBoolean("DB/Button 2", false))
		{
			DriveForward(59.2);
			ScoringSequence();
		}
		// Score in right peg
		else if(SmartDashboard::GetBoolean("DB/Button 3", false))
		{
			DriveForward(81.2);
			TurnRobot(-60);
			DriveForward(27);
			ScoringSequence();
		}
		// Cross baseline
		else if(SmartDashboard::GetBoolean("DB/Button 4", false))
		{
			DriveForward(95);
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

// Drive forward, distance is in inches
void Robot::DriveForward(double distance)
{
	double maxSpeed = 0.6;
	DriveEnc.Reset();

	if(distance > 0)
	{
		while(DriveEnc.GetDistance() < distance)
		{
			DriveTrain.ArcadeDrive(maxSpeed, 0, false);
		}
	}
	else if(distance < 0)
	{
		while(DriveEnc.GetDistance() < -distance)
		{
			DriveTrain.ArcadeDrive(-maxSpeed, 0, false);
		}
	}
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
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
			DriveTrain.ArcadeDrive(0, maxTurnSpeed, false);
		}
	}
	else if (angle < 0)
	{

		while (Gyro.GetAngle() > angle)
		{
			DriveTrain.ArcadeDrive(0, -maxTurnSpeed, false);
		}
	}
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
}
