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
	pidGyro((0.9/90.0), 0.0002, 0, &gyro, &frontLeftDrive),
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
	// encDriveLeft.Reset();
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

	// pidIntake.Reset();
	// pidIntake.Disable();
	// intakePivot.ControlMode = intakePivot.ControlMode::kPosition;
}

void Robot::AutonomousPeriodic()
{
	SmartDashboard::PutNumber("GYRO ERROR:", pidGyro.GetError());
	SetDriveMotors(frontLeftDrive.Get(), .5);
	backLeftDrive.Set(1 - frontLeftDrive.Get());
	frontLeftDrive.Set(backLeftDrive.Get());
	// pidIntake.SetSetpoint(3240);
	// pidIntake.Enable();
	// SmartDashboard::PutNumber("INTAKE ERROR:", pidIntake.GetError());
	// SmartDashboard::PutNumber("Z ACCEL:", biAccel.GetZ());

}

void Robot::TeleopInit()
{
	// Set all the motor values to 0
	pidGyro.Disable();
}

void Robot::TeleopPeriodic()
{
	// Driver Code
	// Send information about the robot to SmartDashboard
	SmartDashboard::PutNumber("INTAKE PIVOT:", intakePivot.GetPulseWidthPosition());
	SmartDashboard::PutNumber("OVEREXTENDED?", (int)isArmOverextended());
	SmartDashboard::PutNumber("BOTTOM DIST:", BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle)));
	SmartDashboard::PutNumber("TOP DIST:", ((BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle))) + (TOP_ARM_LENGTH * cos(degtorad(tophorizangle)))));
	SmartDashboard::PutNumber("BOTTOM ANGLE:", bottomhorizangle);
	SmartDashboard::PutNumber("TOP ANGLE:", tophorizangle);
	SmartDashboard::PutNumber("BREAKBEAM:", (int)breakBeam.Get());
	SmartDashboard::PutNumber("Z ACCEL:", biAccel.GetZ());

	if (Driver.GetRawButton(BUTTON_A)) // If the 'A' button is pressed
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

	else if (Driver.GetRawButton(BUTTON_LB)) // If the left bumper is pressed
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

	else if (Driver.GetRawButton(BUTTON_RB)) // If the right bumper is pressed
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

	else // If nothing is being pressed, then set speed to 0
	{
		moveVal = 0;
		turnVal = 0;
		SetDriveMotors(0, 0);
	}
	
	// Calculates the angle of the arms with the horizontal
	bottomhorizangle = encBottomHinge.GetDistance() + BOTTOM_ARM_START_ANGLE;
	tophorizangle = encTopHinge.GetDistance() + TOP_ARM_START_ANGLE;

	// Operator Code
	if (Operator.GetRawButton(BUTTON_A) && (!breakbeamenabled || breakBeam.Get())) // If 'A' is being pressed, spin the intake inwards
	{
		intakeSpin.Set(1);
	}
	else if (Operator.GetRawButton(BUTTON_B)) // If 'B' is being pressed, spin the intake outwards
	{
		intakeSpin.Set(-1);
	}
	else // If neither is being pressed, do not spin the intake
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

	if (Operator.GetRawAxis(AXIS_TRIGGER_LEFT)) // If the right trigger is pressed, lift the intake
	{
		intakePivot.Set(Operator.GetRawAxis(AXIS_TRIGGER_LEFT));
	}
	else if (Operator.GetRawAxis(AXIS_TRIGGER_RIGHT)) // If the left trigger is pressed, lower the intake
	{
		intakePivot.Set(-Operator.GetRawAxis(AXIS_TRIGGER_RIGHT));
	}
	else // If neither is being pressed, do not move the intake
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
// Local variables to hold the computed PWM values for the motors
float leftMotorOutput;
float rightMotorOutput;

if (squaredInputs)
{
	// Square the inputs (while preserving the sign) to increase fine control
	// While permitting full power
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
	if (BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle)) >= 14.0)
	{
		return true;
	}

	return (((BOTTOM_ARM_LENGTH * cos(degtorad(bottomhorizangle))) + (TOP_ARM_LENGTH * cos(degtorad(tophorizangle)))) >= 14.0);
}

double Robot::degtorad(double deg)
{
	return (deg * PI / 180.0);
}

double Robot::avg(double firstValue, double secondValue) // Outputs average of two values
{
	return ((firstValue + secondValue) / 2);
}

// Autonomous Functions
void Robot::driveToDefense() // Drives from the starting position up to the defense
{
	driveForward(DIST_TO_DEFENSE, true);
}

// Position 1-5 functions will drive the robot from the defenses to the low goal
void Robot::position1()
{
	const double dist1 = 1; // Define the distances and angles as constants
	const double dist2 = 1;
	const double angle1 = 1;

	driveForward(dist1, true); // Drive forward, turn, and then drive forward again
	turn(angle1);
	driveForward(dist2, true);
}
void Robot::position2()
{
	const double dist1 = 1; // Define the distances and angles as constants
	const double dist2 = 1;
	const double angle1 = 1;

	driveForward(dist1, true); // Drive forward, turn, and then drive forward again
	turn(angle1);
	driveForward(dist2, true);
}
void Robot::position3()
{
	const double dist1 = 1; // Define the distances and angles as constants
	const double dist2 = 1;
	const double dist3 = 1;
	const double angle1 = 1;
	const double angle2 = 1;

	driveForward(dist1, true); // Drive forward, turn, and then drive forward again
	turn(angle1);
	driveForward(dist2, true);
	turn(angle2);
	driveForward(dist3, true);
}
void Robot::position4()
{
	const double dist1 = 1; // Define the distances and angles as constants
	const double dist2 = 1;
	const double dist3 = 1;
	const double angle1 = 1;
	const double angle2 = 1;

	driveForward(dist1, true); // Drive forward, turn, and then drive forward again
	turn(angle1);
	driveForward(dist2, true);
	turn(angle2);
	driveForward(dist3, true);
}
void Robot::position5()
{
	const double dist1 = 1; // Define the distances and angles as constants
	const double dist2 = 1;
	const double angle1 = 1;

	driveForward(dist1, true); // Drive forward, turn, and then drive forward again
	turn(angle1);
	driveForward(dist2, true);
}

// These functions will cross the specified defense
void Robot::lowBar()
{
	driveForward(DIST_LOW_BAR, false);
}
void Robot::portcullis()
{
	// Yep, we totally know how to do this
	// Robot.UseMagic();
}
void Robot::chevalDeFrise()
{
	driveForward(12, false); // Drive up to the top of the ramp
	// Use the intake to push it down
	driveForward(DIST_CHEVAL_DE_FRISE, false);
}
void Robot::moat()
{
	driveForward(DIST_MOAT, false); //TODO 
}
void Robot::ramparts()
{
	driveForward(DIST_RAMPARTS, false);
}
void Robot::drawbridge()
{
	// Yep, we totally know how to do this
	// Robot.UseMagic();
}
void Robot::sallyPort()
{
	// Yep, we totally know how to do this
	// Robot.UseMagic();
}
void Robot::rockWall()
{
	driveForward(DIST_ROCK_WALL, true); //TODO
}
void Robot::roughTerrain()
{
	driveForward(DIST_ROUGH_TERRAIN, false); //TODO
}

void Robot::driveForward(double distance, bool isFast) // Drives forward for the distance specified
{	
	double speed = 0.4; // Sets the default speed to 0.4
	if (isFast) // If isFast is true, then change the speed to 0.9
	{
		speed = 0.9;
	}

	while(avg(encDriveLeft.GetDistance(), encDriveRight.GetDistance()) < distance)
	{
		SetDriveMotors(speed, speed); // Keeps driving the robot forward until the robot has traveled far enough
	}
	SetDriveMotors(0, 0); // Stops the robot and resets the encoders
	encDriveLeft.Reset();
	encDriveRight.Reset();
}

void Robot::turn(double endAngle) // Turns for the specified angle
{
 	// Positive angle is clockwise turn, negative angle is counterclockwise turn
	double startAngle = gyro.GetAngle(); // Assumes the angle during autonomous will never exceed 360 deg
	if (startAngle > endAngle)
	{
		while (gyro.GetAngle() > endAngle)
		{
			SetDriveMotors(-0.9, 0.9);
		}
	}
	else if (gyro.GetAngle() < endAngle)
	{
		while (gyro.GetAngle() < endAngle)
		{
			SetDriveMotors(0.9, -0.9);
		}
	}
 	SetDriveMotors(0, 0);
 	encDriveLeft.Reset();
 	encDriveRight.Reset();
}

START_ROBOT_CLASS(Robot)
