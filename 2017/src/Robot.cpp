#include "Robot.h"
#include "Vision.cpp"

Robot::Robot() :
	FrontLeftDrive(1), // CAN
    BackLeftDrive(2), // CAN
    FrontRightDrive(3), // CAN
    BackRightDrive(4), // CAN
	DriveTrain(FrontLeftDrive, BackLeftDrive, FrontLeftDrive, BackRightDrive),
	GearSlide(5), // CAN
	ClimbMotor(6), // CAN
	LeftFlap(7, 3), // PCM
	RightFlap(0, 1), // PCM
	ClimbRelease(0),
    Driver(0),
    Operator(1),
	GearEnc(4, 5),
	LeftBreakBeam(1),
	RightBreakBeam(1),
	HaveGear(0), // DIO
	ActuateFlaps(1), // DIO
	LeftLimit(2), // DIO
	RightLimit(3), // DIO
	//TODO find encoder channels from Alan
	//NavX(/*SPI or I2C*/), //NavX Micro Sensor
	FrontDist(6, 7),
	LifeCam(),
	VisionSink()

{
	FrontDist.SetAutomaticMode(true);
	gearAtLeftEdge = false; //TODO: Add values to these to RobotInit
	gearAtRightEdge = false;
	blockedOnTheRight = false;
	blockedOnTheLeft = false;
	isCentered = false;
	isScoringSequenceEnabled = false;
	prevXButtonVal = false;
	moveVal = 0;
	turnVal = 0;
	gearVal = 0;
}

void Robot::RobotInit()
{
	DriveTrain.SetLeftRightMotorOutputs(0, 0); //Motor Init
	GearSlide.Set(0); // Gear Init
	ClimbMotor.Set(0); //Climb motor Init
	//Camera
	LifeCam = CameraServer::GetInstance()->StartAutomaticCapture();
	LifeCam.SetResolution(640, 480);
	LifeCam.SetExposureManual(0);
	LifeCam.SetBrightness(5);
	VisionSink = CameraServer::GetInstance()->GetVideo();
}

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
//Init Motors/Gear
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
	ScoringSequence();
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

void Robot::TeleopInit()
{
	//Init Motors/Gear
	DriveTrain.SetLeftRightMotorOutputs(0, 0);
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
        SetDriveMotors(0, 0);
    }

    // Operator
    gearVal = Operator.GetRawAxis(AXIS_TRIGGER_LEFT) - Operator.GetRawAxis(AXIS_TRIGGER_RIGHT);
    if (LeftLimit.Get() && gearVal < 0)
    {
    	GearSlide.Set(gearVal);
    	// reset some encoder values
    }
    else if (RightLimit.Get() && gearVal > 0)
    {
    	GearSlide.Set(gearVal);
    	// reset some encoder values
    }
    else
    {
    	GearSlide.Set(gearVal);
    }

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

		if (!TargetInSight && FrontDist.GetRangeInches() <= 20)
		{
			LeftFlap.Set(DoubleSolenoid::kForward);
			RightFlap.Set(DoubleSolenoid::kForward);
			LeftFlapState = true;
			RightFlapState = true;
		}

    }
    // Vision

}

void Robot::TestInit()
{
	SetDriveMotors(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	//Center gear holder
	while (!LeftLimit.Get())
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
	ClimbMotor.Set(1); // Subject to change to -1
	Wait(1);
	ClimbMotor.Set(0);
	Wait(1);

	// Test drive
	SetDriveMotors(1, 1);
	Wait(1);
	SetDriveMotors(-1, -1);
	Wait(1);
	SetDriveMotors(0, 0);

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
	SetDriveMotors(0, 0);
	GearSlide.Set(0);
	ClimbMotor.Set(0);

	if (Operator.GetRawButton(BUTTON_A) || Driver.GetRawButton(BUTTON_A))
	{
		ClimbRelease.SetAngle(0);
	}
}
/*
void Robot::DriveForward(double distance)
{

}

// NavX Helper Functions: THESE DO NOT WORK YET
double Robot::DistanceTraveled()
{
	return sqrt(static_cast<double>(NavX.GetDisplacementX() * NavX.GetDisplacementX() + NavX.GetDisplacementY() * NavX.GetDisplacementY()));
}

void Robot::DriveForward(double distance)
{
	NavX.ResetDisplacement();
	// if we need to we can repeatedly store the displacement for more accuracy
	// tell it to drive forward
	while(DistanceTraveled() < distance)
    {
		DriveTrain.ArcadeDrive(.85, 0, false);
	}
	DriveTrain.ArcadeDrive(0, 0, false);
}

// TODO: Turn
void Robot::TurnRobot(double angle)
{
	NavX.ResetYaw();
	// - angle means turn counterclockwise, + angle means turn clockwise
	if(angle < 0)
    {
		while (NavX.GetYaw() > angle)
        {
			DriveTrain.ArcadeDrive(0, -.85, false);
		}

	}
  else if (angle > 0)
    {
		while (NavX.GetYaw() < angle)
        {
			DriveTrain.ArcadeDrive(0, .85, false);
		}
	}
	DriveTrain.ArcadeDrive(0, 0, false);
}
*/
START_ROBOT_CLASS(Robot)
