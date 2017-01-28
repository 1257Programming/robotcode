#include "Robot.h"
using namespace std;


/**
 * return the absolute value of a double
 * @num the value to find the magnitude of
 */
inline double dabs(double num)
{
	return num > 0.0 ? num : -num;
}

/**
 * Returns true if the given value is beyond a given threshold
 * @value the value to be compared to the threshold
 * @threshold the minimum acceptable value
 */
inline bool isReasonable(double value, double threshold = 0.2)
{
	return dabs(value) > threshold;
}


Robot::Robot() :
		Driver(0),
		Operator(1),
		Gyro(2),
		LeftEnc(3, 5),
		RightEnc(4, 6), //TODO: find the right port numbers
		FrontLeftDrive(1),
		BackLeftDrive(2),
		FrontRightDrive(3),
		BackRightDrive(4),
		GearPivot(5),
		ClimbMotor(6),
		DriveTrain(&FrontLeftDrive, &BackLeftDrive, &FrontRightDrive, &BackRightDrive)
{
}

void Robot::RobotInit()
{
	LeftEnc.SetDistancePerPulse(PI * WHEEL_DIAMETER / PULSES_PER_REV);
	RightEnc.SetDistancePerPulse(PI * WHEEL_DIAMETER / PULSES_PER_REV);
}

/**
 * Drives forward a given distance at a given speed
 * @speed The speed of the motor, where 1 is full speed forward and -1 full speed backward
 * @distance The distance in feet for the robot to travel
 */
void Robot::DriveForward(float distance, double speed=0.85)
{
	Gyro.Reset();
  	RightEnc.Reset();
  	//LeftEnc.Reset(); //optional unless we use left encoder too.
	double turnVal = 0;

	while (RightEnc.GetDistance() < distance) //TODO: See if this is the best way to track distance
	{
		//If the robot is facing left, turn right
		if (Gyro.GetAngle() > 5)
		{
			turnVal += 0.01;
		}

		//If the robot is facing right, turn left
		else if (Gyro.GetAngle() < -5)
		{
			turnVal -= 0.01;
		}
		//TODO: Add break conditions

		DriveTrain.ArcadeDrive(speed, turnVal);
	}
}

/*
 * Turns the robot a given number of degrees
 * @angle: A given value
 */
void Robot::Turn(double angle)
{
  /*
	// begin toms proposal
  	if (angle < -180) {
    while ((angle += 360) < -180); }
  else if (angle > 180) { while ((angle -= 360) > 180); }
  // end toms proposal

  	if (angle > 180)
    {
    	angle -= 360;
    }
  	else if (angle < -180)
    {
        angle += 360;
    }
     */
	Gyro.Reset();
	double turnSpeed = 0.5;
	while (isReasonable(Gyro.GetAngle() - angle, 10))
	{
		if (Gyro.GetAngle() > angle)
		{
			DriveTrain.ArcadeDrive(0, turnSpeed);
		}
		else if (Gyro.GetAngle() < angle)
		{
			DriveTrain.ArcadeDrive(0, -turnSpeed);
		}
	}
}

void Robot::LeftSpawn()
{
	//Third case, where the Robot will approach the left gearing station.
	//Starts directly in front of the gearing station on the starting line.
	//1. Drive forward 185.3 inches.
	//2. Turn 90 degrees to the right.
	//3. Op/vision takes over.
	//4. Turn to the left 75.1339739 degrees.
	//5. Move forward 432.644578 inches

	DriveForward(185.3);
	Turn(90);

	//Op/Vision
	Turn(75.1339739);
	DriveForward(434.596655);
}

void Robot::CenterSpawn()
{
	//Second case, where the robot is going towards the center gear station.
	//Starts at the staring line directly in front of the gearing station.
	//1. Move forward 93.3 inches
	//2. Op and vision take over and load the gearing station.
	//3. Turn left 90 degrees
	//4. Move forward 50.5 inches.
	//5. Turn to the right 77.7101432 degrees.
	//6. Move forward 524.049094 inches



  	DriveForward(93.3);
	//Operator/Vision takes over
	Turn(-90);
	DriveForward(50.5);
	Turn(77.7101432);
	DriveForward(524.049094);
}

void Robot::RightSpawn()
{
	//(Starting at the start in front of the gear station.)
	//1. Go straight 185.3 inches.
	//2. Turn 90 degrees to the left.
	//3. Operator/Vision takes over to put the gear into the station.
	//4. Robot turns right 63.165784 degrees.
	//5. Robot goes straight 470.742236 inches.
	//TODO ??? for the above comment

 	DriveForward(185.3);
		//whenever the bot needs to turn
	Turn(-90);
		//The robot should be in front of the gearing station. By this point,
		//operator/vision should take over.
	Turn(63.165784);
	DriveForward(470.742236);

}

void Robot::AutonomousInit()
{

}

void Robot::AutonomousPeriodic()
{
	//CHOOSE CONTROL SCHEME

}

void Robot::TeleopInit()
{
  //NONE NEEDED NULL VALUE RETURNED
}

// Driver control schema
void Robot::TeleopDriver()
{
	double moveVal = 0;
	double turnVal = 0;
	// If A button is pressed
	if (Driver.GetAButton())
	{
		// If Y axis on left joystick is reasonable, set moveVal
        if (isReasonable(Driver.GetY(GenericHID::kLeftHand)))
        {
            moveVal = Driver.GetY(GenericHID::kLeftHand);
        }
        else
        {
            moveVal = 0;
        }
		// If X axis on left joystick is reasonable, set turnVal
        if (isReasonable(Driver.GetX(GenericHID::kLeftHand)))
        {
			turnVal = Driver.GetX(GenericHID::kLeftHand);
        }
		else
        {
            turnVal = 0;
        }
    }

	// If left bumper is pressed
	else if (Driver.GetBumper(GenericHID::kLeftHand))
    {
      	// If Y axis on left joystick is reasonable, set moveVal
        if (isReasonable(Driver.GetY(GenericHID::kLeftHand)))
        {
        	moveVal = Driver.GetY(GenericHID::kLeftHand);
        }
        else
        {
        	moveVal = 0;
        }
      	// If X axis on right joystick is reasonable, set turnVal
        if (isReasonable(Driver.GetX(GenericHID::kRightHand)))
        {
            turnVal = Driver.GetX(GenericHID::kRightHand);
        }
        else
        {
            turnVal = 0;
        }
    }

  	// If right bumper is pressed
  	else if (Driver.GetBumper(GenericHID::kRightHand))
    {
        // If Y axis on right joystick is reasonable, set moveVal
      	if (isReasonable(Driver.GetY(GenericHID::kRightHand)))
        {
        	moveVal = Driver.GetY(GenericHID::kRightHand);
        }
        else
        {
        	moveVal = 0;
        }
    	// If X axis on left joystick is reasonable, set turnVal
        if (isReasonable(Driver.GetX(GenericHID::kLeftHand)))
        {
            turnVal = Driver.GetX(GenericHID::kLeftHand);
        }
        else
        {
            turnVal = 0;
        }
    }

	else
	{
        moveVal = 0;
        turnVal = 0;
	}

    DriveTrain.ArcadeDrive(moveVal, turnVal, false);
}

// Operator control schema
void Robot::TeleopOperator()
{
  	double motorSpeed = 1;
	// Climber
	if (Operator.GetYButton())
	{
		ClimbMotor.Set(motorSpeed);
	}
    else
    {
        ClimbMotor.Set(0);
    }

    // Gear holder moving
	double gearSpeed = 0;

	if (Operator.GetTriggerAxis(GenericHID::kLeftHand)) //Press the left trigger and move the gear thing left
	{
		gearSpeed -= Operator.GetTriggerAxis(GenericHID::kLeftHand);
	}
	// TODO: Figure out if this is what we want
	else if (Operator.GetTriggerAxis(GenericHID::kRightHand)) //Press the right trigger and move the gear thing right
	{
		gearSpeed += Operator.GetTriggerAxis(GenericHID::kRightHand);
	}
	else
	{
		gearSpeed = 0;
	}
	GearPivot.Set(gearSpeed);
}

void Robot::TeleopPeriodic()
{
    TeleopDriver();
  	TeleopOperator();
}


START_ROBOT_CLASS(Robot)

