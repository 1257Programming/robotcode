#include "Robot.h"

Robot::Robot() :
	FrontLeftDrive(1), // CAN
	BackLeftDrive(2), // CAN
	FrontRightDrive(3), // CAN
	BackRightDrive(4), // CAN
	GearSlide(5), // CAN
	ClimbMotor(6), // CAN
	Doors(0, 1), // PCM
	Pusher(2, 3), // PCM
	Flaps(4, 5), // PCM
	Driver(0),
	Operator(1),
	RobotTimer(),
	matchTimer(),
	Gyro(SPI::kOnboardCS0) //SPI
{
	moveVal = 0;
	turnVal = 0;
	gearVal = 0;

	Automation = true;
	YPrevState = false;
}

void Robot::RobotInit()
{
	//Gear Init
	GearSlide.Set(0);
	//Climb Motor Init
	ClimbMotor.Set(0);
}

void Robot::TeleopInit()
{
	//Init Motors/Gear
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
	Flaps.Set(DoubleSolenoid::kForward);
}

void Robot::TeleopPeriodic()
{
	// Driver
	if (Driver.GetRawButton(BUTTON_A))
	{
		if (IsReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
		{
			moveVal = -Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
		}
		else
		{
			moveVal = 0;
		}

		if (IsReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
		{
			turnVal = -Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
		}
		else
		{
			turnVal = 0;
		}

		ArcadeDrive(moveVal, turnVal, true);
	}
	else if (Driver.GetRawButton(BUTTON_LB))
	{
		if (IsReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
		{
			moveVal = -Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
		}
		else
		{
			moveVal = 0;
		}

		if (IsReasonable(Driver.GetRawAxis(AXIS_ANALOG_RIGHT_X)))
		{
			turnVal = -Driver.GetRawAxis(AXIS_ANALOG_RIGHT_X);
		}
		else
		{
			turnVal = 0;
		}

		ArcadeDrive(moveVal, turnVal, true);
	}
	else if (Driver.GetRawButton(BUTTON_RB))
	{
		if (IsReasonable(Driver.GetRawAxis(AXIS_ANALOG_RIGHT_Y)))
		{
			moveVal = -Driver.GetRawAxis(AXIS_ANALOG_RIGHT_Y);
		}
		else
		{
			moveVal = 0;
		}

		if (IsReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
		{
			turnVal = -Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
		}
		else
		{
			turnVal = 0;
		}

		ArcadeDrive(moveVal, turnVal, true);
	}
	else
	{
		moveVal = 0;
		turnVal = 0;
		ArcadeDrive(0, 0);
	}

	gearVal = Operator.GetRawAxis(AXIS_TRIGGER_LEFT) - Operator.GetRawAxis(AXIS_TRIGGER_RIGHT);

	// GMA
		if (Operator.GetRawButton(BUTTON_A))
		{
			Flaps.Set(DoubleSolenoid::kForward);
		}
		else if (Operator.GetRawButton(BUTTON_B))
		{
			Flaps.Set(DoubleSolenoid::kReverse);
		}
		else
		{
			Flaps.Set(DoubleSolenoid::kOff);
		}

		if (Operator.GetRawButton(BUTTON_LB) && !wait)
		{
			Doors.Set(DoubleSolenoid::kForward);
			matchTimer.Stop();
			matchTimer.Reset();
			matchTimer.Start();
			SmartDashboard::PutNumber("startTime", startTime);
			wait = true;
			open = true;
		}
		if (Operator.GetRawButton(BUTTON_RB) && !wait)
		{
			Pusher.Set(DoubleSolenoid::kReverse);
			matchTimer.Stop();
			matchTimer.Reset();
			matchTimer.Start();
			SmartDashboard::PutNumber("startTime", startTime);
			wait = true;
			open = false;
		}
		if (wait && matchTimer.HasPeriodPassed(.1))
		{
			if (open)
			{
				Pusher.Set(DoubleSolenoid::kForward);
				wait = false;
			}
			else
			{
				Doors.Set(DoubleSolenoid::kReverse);
				wait = false;
			}
		}

	if (IsReasonable(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y)) && Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y) < 0)
	{
		ClimbMotor.Set(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
	}
	else
	{
		ClimbMotor.Set(0);
	}

	if (Driver.GetRawButton(BUTTON_Y))
	{
		ClimbMotor.Set(-1);
	}

	// Automation
	if (Operator.GetRawButton(BUTTON_Y) && !YPrevState)
	{
		if (Automation)
		{
			Automation = false;
			SmartDashboard::PutString("DB/String 6", "Sensors ignored");
		}
		else
		{
			Automation = true;
			SmartDashboard::PutString("DB/String 6", "Sensors used");
		}

		YPrevState = true;
	}
	else
	{
		YPrevState = false;
	}

	if (Automation)
	{
		// Limit switches on gearmaster 9000
		if ((!GearSlide.IsFwdLimitSwitchClosed() || gearVal < 0) && (!GearSlide.IsRevLimitSwitchClosed() || gearVal > 0))
		{
			GearSlide.Set(gearVal);
		}
		else
		{
			GearSlide.Set(0);
		}
	}
	else
	{
		GearSlide.Set(gearVal);
	}

	LimitDriveCurrent(MAX_CURRENT);

	//Update Smartdashboard
	SmartDashboard::PutNumber("Gyro", Gyro.GetAngle());
	SmartDashboard::PutNumber("Front Left Drive", FrontLeftDrive.GetOutputCurrent());
	SmartDashboard::PutNumber("Back Left Drive", BackLeftDrive.GetOutputCurrent());
	SmartDashboard::PutNumber("Front Right Drive", FrontRightDrive.GetOutputCurrent());
	SmartDashboard::PutNumber("Back Right Drive", BackRightDrive.GetOutputCurrent());
}

void Robot::TestInit()
{

}

void Robot::TestPeriodic()
{
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
}


void Robot::SetDriveMotors(float left, float right)
{
    FrontLeftDrive.Set(left);
    BackLeftDrive.Set(left);
    FrontRightDrive.Set(-right);
    BackRightDrive.Set(-right);
}

void Robot::ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs /*= false*/)
{
    float LeftMotorOutput;
    float RightMotorOutput;

    if (squaredInputs)
    {
        if (moveValue >= 0.0)
        {
            moveValue = (moveValue * moveValue);
        }
        else
        {
            moveValue = -(moveValue * moveValue);
        }

        if (rotateValue >= 0.0)
        {
            rotateValue = (rotateValue * rotateValue);
        }
        else
        {
            rotateValue = -(rotateValue * rotateValue);
        }
    }

    if (moveValue > 0.0)
    {
        if (rotateValue > 0.0)
        {
            LeftMotorOutput = moveValue - rotateValue;
            RightMotorOutput = std::max(moveValue, rotateValue);
        }
        else
        {
            LeftMotorOutput = std::max(moveValue, -rotateValue);
            RightMotorOutput = moveValue + rotateValue;
        }
    }
    else
    {
        if (rotateValue > 0.0)
        {
            LeftMotorOutput = -std::max(-moveValue, rotateValue);
            RightMotorOutput = moveValue + rotateValue;
        }
        else
        {
            LeftMotorOutput = moveValue - rotateValue;
            RightMotorOutput = -std::max(-moveValue, -rotateValue);
        }
    }

    SetDriveMotors(LeftMotorOutput, RightMotorOutput);
}

void Robot::LimitDriveCurrent(int maxCurrent)
{
	bool exceedFLCurrent = FrontLeftDrive.GetOutputCurrent() > maxCurrent;
	bool exceedBLCurrent = BackLeftDrive.GetOutputCurrent() > maxCurrent;
	bool exceedFRCurrent = FrontRightDrive.GetOutputCurrent() > maxCurrent;
	bool exceedBRCurrent = BackRightDrive.GetOutputCurrent() > maxCurrent;
	if(exceedFLCurrent || exceedBLCurrent || exceedFRCurrent || exceedBRCurrent)
	{
		SmartDashboard::PutBoolean("Maxed Out Current", true);
		//Halve the speeds of each motor when the maximum current is exceeded
		FrontLeftDrive.Set( FrontLeftDrive.Get() / 2 );
		BackLeftDrive.Set( BackLeftDrive.Get() / 2 );
		FrontRightDrive.Set( FrontRightDrive.Get() / 2 );
		BackRightDrive.Set( BackRightDrive.Get() / 2 );
	}
	else
	{
		SmartDashboard::PutBoolean("Maxed Out Current", false);
	}
}

START_ROBOT_CLASS(Robot)
