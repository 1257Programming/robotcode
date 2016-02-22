#include "Robot.h"

Robot::Robot() :
		frontLeftDrive(3),
        backLeftDrive(4),
        frontRightDrive(5),
        backRightDrive(1),
		intakePivot(2),
		intakeSpin(0),
		bottomArmHinge(0),
		topArmHinge(6),
		gyro(),
		encDriveLeft(4, 5, false),
		encDriveRight(2, 3, true),
		encBottomHinge(6, 7, false),
		encTopHinge(8, 9, true),
		breakBeam(1),
		biAccel(),
        Driver(0),
        Operator(1),
		pidGyro((.9/90.0), .0002, 0, &gyro, &frontLeftDrive),
		pidIntake(0, 0, 0, &intakePivot, &intakePivot)
{

}
	
void Robot::RobotInit()
{
	encBottomHinge.SetDistancePerPulse(360.0/497.0); // Degrees
	encTopHinge.SetDistancePerPulse(360.0/497.0); 	 // Degrees
	encDriveLeft.SetDistancePerPulse(PI * WHEEL_DIAMETER / AMTRES); // res pulses per 1 rev, 1 rev = 2 pi rad
	encDriveRight.SetDistancePerPulse(PI * WHEEL_DIAMETER / AMTRES); // inches (7.625 in. outer wheel diameter?)
}

  	
void Robot::AutonomousInit()
{
	//encDriveLeft.Reset();
	gyro.Reset();
	pidGyro.Reset();
	pidGyro.Disable();

	SetDriveMotors(-.5, .5);
	pidGyro.SetSetpoint(0);
	pidGyro.SetAbsoluteTolerance(1.0);
	pidGyro.Enable();
	backLeftDrive.Set(frontLeftDrive.Get());
	frontRightDrive.Set(frontLeftDrive.Get());
	backRightDrive.Set(frontLeftDrive.Get());
	SmartDashboard::PutNumber("GYRO ERROR:", pidGyro.GetError());

	//pidIntake.Reset();
	//pidIntake.Disable();
	//intakePivot.ControlMode = intakePivot.ControlMode::kPosition;
}

void Robot::AutonomousPeriodic()
{
	SmartDashboard::PutNumber("GYRO ERROR:", pidGyro.GetError());
	SetDriveMotors(frontLeftDrive.Get(), .5);
	backLeftDrive.Set(1 - frontLeftDrive.Get());
	frontLeftDrive.Set(backLeftDrive.Get());
	//pidIntake.SetSetpoint(3240);
	//pidIntake.Enable();
	//SmartDashboard::PutNumber("INTAKE ERROR:", pidIntake.GetError());
	//SmartDashboard::PutNumber("Z ACCEL:", biAccel.GetZ());

}

void Robot::TeleopInit()
{
    	//set all the motor values to 0
	pidGyro.Disable();
}

void Robot::TeleopPeriodic()
{
	      	//Driver Code
	SmartDashboard::PutNumber("INTAKE PIVOT:", intakePivot.GetPulseWidthPosition());
	SmartDashboard::PutNumber("OVEREXTENDED?", (int)isArmOverextended());
	SmartDashboard::PutNumber("BOTTOM DIST:", BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle)));
	SmartDashboard::PutNumber("TOP DIST:", ((BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle))) + (TOP_ARM_LENGTH * cos(degtorad(tophorizangle)))));
	SmartDashboard::PutNumber("BOTTOM ANGLE:", bottomhorizangle);
	SmartDashboard::PutNumber("TOP ANGLE:", tophorizangle);
	SmartDashboard::PutNumber("BREAKBEAM:", (int)breakBeam.Get());
	SmartDashboard::PutNumber("Z ACCEL:", biAccel.GetZ());

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
      	
	bottomhorizangle = encBottomHinge.GetDistance() + BOTTOM_ARM_START_ANGLE;
	tophorizangle = encTopHinge.GetDistance() + TOP_ARM_START_ANGLE;
      	//Operator Code
	if (Operator.GetRawButton(BUTTON_A) && (!breakbeamenabled || breakBeam.Get())) //If 'A' is being pressed, spin the intake inwards
	{
		intakeSpin.Set(1);
	}
	else if (Operator.GetRawButton(BUTTON_B)) //If 'B' is being pressed, spin the intake outwards
	{
		intakeSpin.Set(-1);
	}
	else
	{
		intakeSpin.Set(0);
	}

	if(Operator.GetRawButton(BUTTON_X))
	{
		breakbeamenabled = true;
	}

	else if(Operator.GetRawButton(BUTTON_Y))
	{
		breakbeamenabled = false;
	}

	if (Operator.GetRawAxis(AXIS_TRIGGER_LEFT)) //If the right trigger is pressed, lift the intake
	{
		intakePivot.Set(Operator.GetRawAxis(AXIS_TRIGGER_LEFT));
	}
	else if (Operator.GetRawAxis(AXIS_TRIGGER_RIGHT)) //If the left trigger is pressed, lower the intake
	{
		intakePivot.Set(-Operator.GetRawAxis(AXIS_TRIGGER_RIGHT));
	}
	else
	{
		intakePivot.Set(0);
	}

	if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y)) && ((!isArmOverextended() && ((bottomhorizangle > 45))) || Operator.GetRawAxis(5) > 0)) // If the right stick is moved vertically, rotate the bottom hinge
	{
		bottomArmHinge.Set(.75 * Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y));
	}
	else
	{
		bottomArmHinge.Set(0);
	}

	if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y))) // If the left stick is moved vertically, rotate the top hinge
	{
		if(!isArmOverextended())
		{
			topArmHinge.Set(.75 * Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
		}

		else if(((tophorizangle < 0) && (Operator.GetRawAxis(1) > 0)) || ((tophorizangle > 0) && (Operator.GetRawAxis(1) < 0)))
		{
			topArmHinge.Set(.75 * Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
		}

		else
		{
			topArmHinge.Set(0);
		}
	}
	else
	{
		topArmHinge.Set(0);
	}
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

bool Robot::isArmOverextended()
{
	if(BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle)) >= 14.0)
	{
		return true;
	}

	return (((BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle))) + (TOP_ARM_LENGTH * cos(degtorad(tophorizangle)))) >= 14.0);
}

double Robot::degtorad(double deg)
{
	return (deg * PI / 180.0);
}


START_ROBOT_CLASS(Robot)

