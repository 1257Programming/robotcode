#include "Robot.h"

Robot::Robot() :
	FrontLeftDrive(1), // CAN
	BackLeftDrive(2), // CAN
	FrontRightDrive(3), // CAN
	BackRightDrive(4), // CAN
	GearSlide(5), // CAN
	ClimbMotor(6), // CAN
	LeftFlap(7, 3), // PCM
	RightFlap(0, 1), // PCM
	ClimbRelease(0),
	Driver(0),
	Operator(1),
	GearEnc(4, 5),
	HaveGear(0), // DIO
	ActuateFlaps(1), // DIO
	//NavX(SerialPort::Port::kUSB), //NavX Micro Sensor
	FrontDist(6, 7),
	LifeCam(),
	VisionSink(),
	RobotTimer(),
	Gyro(SPI::kOnboardCS0) //SPI
{
	moveVal = 0;
	turnVal = 0;
	gearVal = 0;

	XPrevState = false;
	hasAutoRun = false;
	LeftFlapState = false;
	RightFlapState = false;
	LBPrevState = false;
	RBPrevState = false;
	targetInSight = false;
	Automation = true;
	YPrevState = false;
}

void Robot::RobotInit()
{
	//Ultrasonic Init
	FrontDist.SetAutomaticMode(true);
	//Gear Init
	GearSlide.Set(0);
	//Climb Motor Init
	ClimbMotor.Set(0);
	//Camera Init
	LifeCam = CameraServer::GetInstance()->StartAutomaticCapture();
	LifeCam.SetResolution(640, 480);
	LifeCam.SetExposureManual(0);
	LifeCam.SetBrightness(5);
	VisionSink = CameraServer::GetInstance()->GetVideo();
	ArcadeDrive(0, 0);
}

void Robot::TeleopInit()
{
	//Init Motors/Gear
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);
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

		ArcadeDrive(moveVal, turnVal, false);
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

		ArcadeDrive(moveVal, turnVal, false);
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

		ArcadeDrive(moveVal, turnVal, false);
	}
	else
	{
		moveVal = 0;
		turnVal = 0;
		ArcadeDrive(0, 0);
	}

	// Operator
	gearVal = Operator.GetRawAxis(AXIS_TRIGGER_LEFT) - Operator.GetRawAxis(AXIS_TRIGGER_RIGHT);

	if (Operator.GetRawButton(BUTTON_A))
	{
		LeftFlap.Set(DoubleSolenoid::kForward);
		RightFlap.Set(DoubleSolenoid::kForward);
		LeftFlapState = true;
		RightFlapState = true;
	}
	else if (Operator.GetRawButton(BUTTON_B))
	{
		RightFlap.Set(DoubleSolenoid::kReverse);
		LeftFlap.Set(DoubleSolenoid::kReverse);
		LeftFlapState = false;
		LeftFlapState = false;
	}
	else if (Operator.GetRawButton(BUTTON_LB))
	{
		if (!LBPrevState)
		{
			if (LeftFlapState)
			{
				LeftFlap.Set(DoubleSolenoid::kReverse);
				LeftFlapState = false;
			}
			else
			{
				LeftFlap.Set(DoubleSolenoid::kForward);
				LeftFlapState = true;
			}

			LBPrevState = true;
		}
	}
	else if (Operator.GetRawButton(BUTTON_RB))
	{
		if (!RBPrevState)
		{
			if (RightFlapState)
			{
				RightFlap.Set(DoubleSolenoid::kReverse);
				RightFlapState = false;
			}
			else
			{
				RightFlap.Set(DoubleSolenoid::kForward);
				RightFlapState = true;
			}

			RBPrevState = true;
		}
	}
	else
	{
		LeftFlap.Set(DoubleSolenoid::kOff);
		RightFlap.Set(DoubleSolenoid::kOff);
		LBPrevState = false;
		RBPrevState = false;
	}

	if (Operator.GetRawButton(BUTTON_Y) && IsReasonable(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
	{
		ClimbMotor.Set(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
	}
	else
	{
		ClimbMotor.Set(0);
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
		// Automation flaps
		if (!ActuateFlaps.Get())
		{
			RightFlap.Set(DoubleSolenoid::kReverse);
			LeftFlap.Set(DoubleSolenoid::kReverse);
			LeftFlapState = false;
			LeftFlapState = false;
		}

		if (!targetInSight && FrontDist.GetRangeInches() <= 20)
		{
			LeftFlap.Set(DoubleSolenoid::kForward);
			RightFlap.Set(DoubleSolenoid::kForward);
			LeftFlapState = true;
			RightFlapState = true;
		}

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

	//Vision
	if(Operator.GetRawButton(BUTTON_X))
	{
		XPrevState = true;
		ScoringSequence(); //The function checks if it should be switched off after every one of its loops
	}
	else
	{
		XPrevState = false;
	}

}

void Robot::TestInit()
{
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	//Center gear holder
	while (!GearSlide.IsFwdLimitSwitchClosed())
	{
		GearSlide.Set(.5);
	}
	/*while (encoder value < encoder value while centered)
	{
		GearSlide.Set(.5);
	}*/
	GearSlide.Set(-.5);
	Wait(2);
	GearSlide.Set(0);
	Wait(1);

	// Release Climber
	ClimbRelease.SetAngle(180);
	Wait(1);

	// Run climber
	ClimbMotor.Set(1);
	Wait(1);
	ClimbMotor.Set(0);
	Wait(1);

	// Test drive
	ArcadeDrive(1, 1);
	Wait(1);
	ArcadeDrive(-1, -1);
	Wait(1);
	ArcadeDrive(0, 0);

	//Actuate flaps
	LeftFlap.Set(DoubleSolenoid::kForward);
	RightFlap.Set(DoubleSolenoid::kForward);
	Wait(1);
	LeftFlap.Set(DoubleSolenoid::kReverse);
	RightFlap.Set(DoubleSolenoid::kReverse);
	Wait(1);
}

void Robot::TestPeriodic()
{
	ArcadeDrive(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	if (Operator.GetRawButton(BUTTON_A) || Driver.GetRawButton(BUTTON_A))
	{
		ClimbRelease.SetAngle(0);
	}
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

START_ROBOT_CLASS(Robot)
