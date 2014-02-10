#include "1257Robot.h"

void Team1257Robot::Autonomous()
{
	while(IsAutonomous() && IsEnabled())
	{
		Drive.SetLeftRightMotorOutputs(-.3, -.3);
	}
}

void Team1257Robot::OperatorControl()
{
	while(IsOperatorControl() && IsEnabled())
	{
		drive(); // XBox Controller 1 drives robot
		arms();//XBox Controller 2 conrols arms
	}
}

void Team1257Robot::Test()
{
	
}

void Team1257Robot::drive()
{
	if(Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
	{
		Drive.TankDrive(accel(Stick1, 2, leftspeed, .5), accel(Stick1, 5, rightspeed, .5), false);
	}
	else if (Stick1.GetRawAxis(3)) // Back button; just one, not both
	{
		Drive.ArcadeDrive(accel(Stick1, 5, speed, .5), accel(Stick1, 1, curve, .5), false);
	}
}

void Team1257Robot::arms()
{
	if(Stick2.GetRawButton(5))
		LeftArm.Set(accel(Stick2, 1, leftarmspeed, .2));
	if(Stick2.GetRawButton(6))
		RightArm.Set(accel(Stick2, 4, rightarmspeed, .2));
	ArmShoulder.Set(accel(Stick2, 3, shoulderspeed, .2));
}

double Team1257Robot::accel(Joystick& stick, int axis, double& current, double sf)
{
	double raw = stick.GetRawAxis(axis);
	if(raw > current && raw > 0) // If speeding up, increment it there, instead of sudden jerk
		current += .05;
	if(raw < current && raw < 0) // Same as above, taking into account negative joystick values
		current -= .05;
	if(dabs(raw) < dabs(current)) // If the target speed is lesser, reduce to that instantly, like for stopping
		current = raw;
	if(dabs(raw) < .1) // Taking into account SLIGHTLY off-centered axes
		current = 0;
	return (current * sf);
}

START_ROBOT_CLASS(Team1257Robot);

