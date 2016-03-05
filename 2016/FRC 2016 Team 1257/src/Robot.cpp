#include "Robot.h"

Robot::Robot() :
	frontLeftDrive(3),
        backLeftDrive(4),
        frontRightDrive(5),
        backRightDrive(1),
	intakePivot(2),
	intakeSpin(0),
	wedgeHinge(0),
//	topArmHinge(6),
	pwmOne(1),
	pwmTwo(2),
	pwmThree(3),
	pwmFour(4),
	Lifecam("cam0", false),
	gyro(),
	encDriveLeft(4, 5, false),
	encDriveRight(2, 3, true),
	encBottomHinge(6, 7, false),
//	encTopHinge(8, 9, true),
	breakBeam(1),
	biAccel(),
        Driver(0),
        Operator(1),
	pidGyro((.9/90.0), .0002, 0, &gyro, &frontLeftDrive),
	pidIntake(0, 0, 0, &intakePivot, &intakePivot)
{
	Lifecam.OpenCamera();
	Lifecam.SetBrightness(0);
}
	
void Robot::RobotInit()
{
	encBottomHinge.SetDistancePerPulse(360.0/497.0); // Degrees
//	encTopHinge.SetDistancePerPulse(360.0/497.0); 	 // Degrees
	encDriveLeft.SetDistancePerPulse(PI * WHEEL_DIAMETER / AMTRES); // res pulses per 1 rev, 1 rev = 2 pi rad
	encDriveRight.SetDistancePerPulse(PI * WHEEL_DIAMETER / AMTRES); // inches (7.625 in. outer wheel diameter?)
	intakestart = intakePivot.GetPulseWidthPosition();
	intakebottom = intakestart - 1759;
}

void Robot::DisabledInit()
{
	//Dashboard Key:
	SmartDashboard::PutString("DB/String 0", "Forward Crossing");
	SmartDashboard::PutString("DB/String 1", "Backward Crossing");
	SmartDashboard::PutString("DB/String 2", "Portcullis Crossing");
	SmartDashboard::PutString("DB/String 3", "Cheval Crossing");
	SmartDashboard::PutString("DB/String 4", "Select None for Just Reach");
	SmartDashboard::PutString("DB/String 5", "Defense Position");
	SmartDashboard::PutString("DB/String 6", "Adjust By Entry Field");
	SmartDashboard::PutString("DB/String 7", "0 for No Ball");
	SmartDashboard::PutString("DB/String 8", "DON'T FORGET CAMERA");
}

void Robot::AutonomousInit()
{
	if(SmartDashboard::GetBoolean("DB/Button 3", false))
	{
		if(!CameraServer::GetInstance()->IsAutoCaptureStarted())
		{
		std::shared_ptr<USBCamera> cam(&Lifecam);
		CameraServer::GetInstance()->StartAutomaticCapture(cam);
		}
	}

	autoran = false;
	LEDAutonomousIdle();
	gyro.Reset();
	pidGyro.Reset();
	pidGyro.SetSetpoint(0);
}

void Robot::AutonomousPeriodic()
{
	if(!autoran)
	{
		if(SmartDashboard::GetBoolean("DB/Button 0", false))
		{
			forcross();
		}
		else if(SmartDashboard::GetBoolean("DB/Button 1", false))
		{
			backcross();
		}
		else if(SmartDashboard::GetBoolean("DB/Button 2", false))
		{
			std::cout<<"Cheval"<<std::endl;
		}
		else if(SmartDashboard::GetBoolean("DB/Button 3", false))
		{
			std::cout<<"Portcullis"<<std::endl;
		}
		else
		{

		}

		// For Later Competiton Ball Scoring
		switch((int)SmartDashboard::GetNumber("DB/Slider 0", 0))
		{

		case 1:
		{
			std::cout<<"Switch Case 1"<<std::endl;
			break;
		}
		case 2:
		{

			break;
		}
		case 3:
		{

			break;
		}
		case 4:
		{

			break;
		}
		case 5:
		{

			break;
		}
		default:
		{

		}

		}
	}
	autoran = true;
}

void Robot::TeleopInit()
{
    	//set all the motor values to 0
	pidGyro.Disable();
	if(SmartDashboard::GetBoolean("DB/Button 3", false))
	{
		if(!CameraServer::GetInstance()->IsAutoCaptureStarted())
		{
		std::shared_ptr<USBCamera> cam(&Lifecam);
		CameraServer::GetInstance()->StartAutomaticCapture(cam);
		}
	}
}

void Robot::TeleopPeriodic()
{
	 //Driver Code
	SmartDashboard::PutNumber("INTAKE PIVOT:", intakePivot.GetPulseWidthPosition());
//	SmartDashboard::PutNumber("OVEREXTENDED?", (int)isArmOverextended());
//	SmartDashboard::PutNumber("BOTTOM DIST:", BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle)));
//	SmartDashboard::PutNumber("TOP DIST:", ((BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle))) + (TOP_ARM_LENGTH * cos(degtorad(tophorizangle)))));
	SmartDashboard::PutNumber("WEDGE ANGLE:", wedgehorizangle);
//	SmartDashboard::PutNumber("TOP ANGLE:", tophorizangle);
	SmartDashboard::PutNumber("BREAKBEAM:", (int)breakBeam.Get());
	SmartDashboard::PutNumber("Z ACCEL:", biAccel.GetZ());
	SmartDashboard::PutNumber("LEFT INCHES", encDriveLeft.GetDistance());
	SmartDashboard::PutNumber("RIGHT INCHES", encDriveRight.GetDistance());

	if (Driver.GetRawButton(BUTTON_A)) //If the 'A' button is pressed
	{
		if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
        	{
            		moveVal = -Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
        	}
        	else
        	{
            		moveVal = 0;
        	}
		if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
        	{
			turnVal = -Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
        	}
		else
        	{
            		turnVal = 0;
        	}

		ArcadeDrive(moveVal, turnVal, false);
	}

	else if (Driver.GetRawButton(BUTTON_LB)) //If the left bumper is pressed
    	{
	        if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
	        {
	        	moveVal = -Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
	        }
	        else
	        {
	        	moveVal = 0;
	        }
	        if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_RIGHT_X)))
	        {
	            turnVal = -Driver.GetRawAxis(AXIS_ANALOG_RIGHT_X);
	        }
	        else
	        {
	            turnVal = 0;
	        }

        	ArcadeDrive(moveVal, turnVal, false);
    	}
	else if (Driver.GetRawButton(BUTTON_RB)) //If the right bumper is pressed
    	{
		if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_RIGHT_Y)))
		{
			moveVal = -Driver.GetRawAxis(AXIS_ANALOG_RIGHT_Y);
		}
	        else
	        {
	        	moveVal = 0;
	        }
	        if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
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
      	
	wedgehorizangle = encBottomHinge.GetDistance() + BOTTOM_WEDGE_START_ANGLE;
//	tophorizangle = encTopHinge.GetDistance() + TOP_ARM_START_ANGLE;

	if(breakBeam.Get())
	{
		haveball = false;
	}

	//Operator Code
	if (Operator.GetRawButton(BUTTON_A) && (!haveball || !breakbeamenabled)) //If 'A' is being pressed, spin the intake inwards
	{
		intakeSpin.Set(1);
		if(!breakBeam.Get() && breakbeamenabled)
		{
			std::thread ballclaim(&Robot::stopIntake, this);
			ballclaim.detach();
		}
		if(DriverStation::GetInstance().GetAlliance() == DriverStation::kRed)
		{
			if(haveball)
			{
				LEDTeleopWithBallRed();
			}
			else
			{
				LEDIntakeInRed();
			}
		}
		else
		{
			if(haveball)
			{
				LEDTeleopIdleBlue();
			}
			else
			{
				LEDIntakeInBlue();
			}
		}

	}
	else if (Operator.GetRawButton(BUTTON_B)) //If 'B' is being pressed, spin the intake outwards
	{
		intakeSpin.Set(-1);
		if(DriverStation::GetInstance().GetAlliance() == DriverStation::kRed)
		{
			LEDIntakeOutRed();
		}
		else
		{
			LEDIntakeOutBlue();
		}

	}
	else
	{
		intakeSpin.Set(0);
		if(DriverStation::GetInstance().GetAlliance() == DriverStation::kRed)
		{
			LEDTeleopIdleRed();
		}
		else
		{
			LEDTeleopIdleBlue();
		}

	}

	if(Operator.GetRawButton(BUTTON_X))
	{
		breakbeamenabled = true;
	}

	else if(Operator.GetRawButton(BUTTON_Y))
	{
		breakbeamenabled = false;
	}

	if (Operator.GetRawAxis(AXIS_TRIGGER_LEFT)) //If the left trigger is pressed, lower the intake
	{
		intakePivot.Set(Operator.GetRawAxis(AXIS_TRIGGER_LEFT));
	}
	else if (Operator.GetRawAxis(AXIS_TRIGGER_RIGHT)) //If the right trigger is pressed, raise the intake
	{
		intakePivot.Set(-Operator.GetRawAxis(AXIS_TRIGGER_RIGHT));
	}
	else
	{
		intakePivot.Set(0);
	}

	if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y))/* && ((!isArmOverextended() && ((bottomhorizangle > 45))) || Operator.GetRawAxis(5) > 0)*/) // If the right stick is moved vertically, rotate the bottom hinge
	{
		if((Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y) < 0 && wedgehorizangle > 5) || (Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y) > 0 && wedgehorizangle < 140))
		{
			wedgeHinge.Set(.75 * Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y));
		}
		else
			wedgeHinge.Set(0);
	}
	else
	{
		wedgeHinge.Set(0);
	}

//	if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y))) // If the left stick is moved vertically, rotate the top hinge
//	{
//		if(!isArmOverextended())
//		{
//			topArmHinge.Set(.75 * Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
//		}
//
//		else if(((tophorizangle < 0) && (Operator.GetRawAxis(1) > 0)) || ((tophorizangle > 0) && (Operator.GetRawAxis(1) < 0)))
//		{
//			topArmHinge.Set(.75 * Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
//		}
//
//		else
//		{
//			topArmHinge.Set(0);
//		}
//	}
//	else
//	{
//		topArmHinge.Set(0);
//	}
}

void Robot::TestInit()
{
	wedgeAdjust(false);
}

void Robot::TestPeriodic()
{

}

void Robot::SetDriveMotors(float left, float right)
{
	frontLeftDrive.Set(left);
	backLeftDrive.Set(left);
	frontRightDrive.Set(right);
	backRightDrive.Set(right);
}

void Robot::ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs)
{
// local variables to hold the computed PWM values for the motors
  float leftMotorOutput;
  float rightMotorOutput;

  if (squaredInputs)
  {
	// square the inputs (while preserving the sign) to increase fine control
	// while permitting full power
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
		  leftMotorOutput = moveValue - rotateValue;
		  rightMotorOutput = std::max(moveValue, rotateValue);
	  }
	  else
	  {
		leftMotorOutput = std::max(moveValue, -rotateValue);
		rightMotorOutput = moveValue + rotateValue;
	  }
  }
  else
  {
	  if (rotateValue > 0.0)
	  {
		  leftMotorOutput = -std::max(-moveValue, rotateValue);
		  rightMotorOutput = moveValue + rotateValue;
	  }
	  else
	  {
		  leftMotorOutput = moveValue - rotateValue;
		  rightMotorOutput = -std::max(-moveValue, -rotateValue);
	  }
  }

  SetDriveMotors(leftMotorOutput, -rightMotorOutput);
}

void Robot::intakeAdjust(bool pos)
{
	Timer timeout;
	timeout.Start();
	if(pos)
	{
		while(intakePivot.GetPulseWidthPosition() < (intakestart - 200))
		{
			intakePivot.Set(-.8);
			if(timeout.Get() > 4)
			{
				break;
			}
		}
	}

	else
	{
		while(intakePivot.GetPulseWidthPosition() > (intakebottom + 200))
		{
			intakePivot.Set(.8);
			if(timeout.Get() > 4)
			{
				break;
			}
		}
	}
	intakePivot.Set(0);
}

void Robot::wedgeAdjust(bool pos)
{
	wedgehorizangle = encBottomHinge.GetDistance() + BOTTOM_WEDGE_START_ANGLE;
	Timer timeout;
	timeout.Start();
	if(pos)
	{
		while(wedgehorizangle < 130)
		{
			wedgehorizangle = encBottomHinge.GetDistance() + BOTTOM_WEDGE_START_ANGLE;
			wedgeHinge.Set(.8);

			if(timeout.Get() > 1.5)
			{
				break;
			}
		}
	}
	else
	{
		while(wedgehorizangle > 15)
		{
			wedgehorizangle = encBottomHinge.GetDistance() + BOTTOM_WEDGE_START_ANGLE;
			wedgeHinge.Set(-.8);

			if(timeout.Get() > 4)
			{
				break;
			}
		}
	}
	wedgeHinge.Set(0);
}

void Robot::stopIntake()
{
	Timer time;
	time.Start();
	while(time.Get() < .15)
	{
		intakeSpin.Set(Operator.GetRawButton(1));
	}
	intakeSpin.Set(0);
	haveball = true;
}

//bool Robot::isArmOverextended()
//{
//	if(BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle)) >= 14.0)
//	{
//		return true;
//	}
//
//	return (((BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle))) + (TOP_ARM_LENGTH * cos(degtorad(tophorizangle)))) >= 14.0);
//}

double Robot::degtorad(double deg)
{
	return (deg * PI / 180.0);
}


START_ROBOT_CLASS(Robot)

