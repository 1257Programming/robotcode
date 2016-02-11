#include "Robot.h"

//Global Functions

//inline double dabs(double d) { return d > 0.0 ? d : -d; } // Absolute value of a double precision floating point number
//inline bool isReasonable(double axisVal) { return dabs(axisVal) > 0.2; } // Ensures the axis is intentionally engaged


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
	encDriveLeft(4, 5),
	encDriveRight(2, 3),
	encBottomHinge(8, 9),
	encTopHinge(6, 7),
        Driver(0),
        Operator(1)
{

}
	
void Robot::RobotInit()
{
	int AMTRes = 1; // TEST TO FIND OUT!!!
	encBottomHinge.SetDistancePerPulse(360.0/497.0); // Degrees
	encTopHinge.SetDistancePerPulse(360.0/497.0); 	 // Degrees
	encDriveLeft.SetDistancePerPulse((2 * PI * 8.0) / AMTRes); // res pulses per 1 rev, 1 rev = 2 pi rad
	encDriveRight.SetDistancePerPulse((2 * PI * 8.0) / AMTRes); // inches (8 in. outer wheel diameter?)
}

  	
void Robot::AutonomousInit()
{

}

void Robot::AutonomousPeriodic()
{

}

void Robot::TeleopInit()
{
    	//set all the motor values to 0
        
}

void Robot::TeleopPeriodic()
{
      	//Driver Code
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
      	
      	//Operator Code
	if (Operator.GetRawButton(BUTTON_A)) //If 'A' is being pressed, spin the intake inwards
	{
		intakeSpin.Set(1);
	}
	else if (Operator.GetRawButton(BUTTON_B)) //If 'B' is being pressed, spin the intake outwards
	{
		intakeSpin.Set(-1);
	}
	else if (Operator.GetRawButton(BUTTON_X)) //If 'X' is pressed, spin the intake inwards slowly
	{
		intakeSpin.Set(0.5);
	}
	else if (Operator.GetRawButton(BUTTON_Y)) //If 'Y' is pressed, spin the intake outwards slowly
	{
		intakeSpin.Set(-0.5);
	}
	else
	{
		intakeSpin.Set(0);
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

	if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y))) // If the right stick is moved vertically, rotate the bottom hinge
	{
		bottomArmHinge.Set(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y));
	}
	else
	{
		bottomArmHinge.Set(0);
	}

	if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y))) // If the left stick is moved vertically, rotate the top hinge
	{
		topArmHinge.Set(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
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
	backLeftDrive.Set(right);
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

  SetDriveMotors(leftMotorOutput, rightMotorOutput);
}

//New Untested Functions:
bool Robot::isOverextended()
{
	if (BOTTOM_ARM_LENGTH * cos(bottomHingeAngle) >= 14.5)
	{
		return true;  
	}
	return BOTTOM_ARM_LENGTH * cos(bottomHingeAngle) + TOP_ARM_LENGTH * cos(topHingeAngle) >= 14.5;
}
// Read a value from the encoder and finds the angle with the horizontal line that the arm makes for both hinges
void Robot::setHingeAngles()
{
topHingeAngle = TOP_HINGE_START - encTopArm.GetDistance(); // We might need to flip the signs after testing
bottomHingeAngle = BOTTOM_HINGE_START - encBottomArm.GetDistance();
}

// Member functions for autonomous mode positions - SKELETON
void Robot::position1()
{
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,-1); // Turn right
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the low goal
}
void Robot::position2()
{
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,-1); // Turn right
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the low goal
}
void Robot::position3()
{
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward 
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(-1,1); // Turn left
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,-1); // Turn right
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the low goal
}
void Robot::position4()
{
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,-1); // Turn right
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward 
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(-1,1); // Turn left
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the low goal
}
void Robot::position5()
{
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward 
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(-1,1); // Turn left
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
	Wait(1);
	driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the low goal  
}

void Robot::portcullis()
{

}
void Robot::chevalDeFrise()
{
        
}
void Robot::moat()
{
    driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
    Wait(1);
    driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the other side
}
void Robot::ramparts()
{
      
}
void Robot::drawbridge()
{
        
}
void Robot::sallyPort()
{
     
}
void Robot::rockWall()
{
    driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
    Wait(1);
    driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the other side
}
void Robot::roughTerrain()
{
    driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
    Wait(1);
    driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the other side
}
void Robot::lowBar()
{
    driveTrain.SetLeftRightMotorOutputs(1,1); // Move forward
    Wait(1);
    driveTrain.SetLeftRightMotorOutputs(0,0); // Stop when you reach the other side
}

START_ROBOT_CLASS(Robot)

