#include "Robot.h"

Robot::Robot() :
	frontLeftDrive(0),
	backLeftDrive(1),
	frontRightDrive(4),
	backRightDrive(5),
	intakePivot(6),
	intakeSpin(0),
	bottomArmHinge(2),
	topArmHinge(3),
	driveTrain(frontLeftDrive, backLeftDrive, frontRightDrive, backRightDrive),
	Driver(0),
	Operator(1)
{

}

void Robot::RobotInit()
{

}


void Robot::AutonomousInit()
{

}

void Robot::AutonomousPeriodic()
{

}

void Robot::TeleopInit()
{

}

void Robot::TeleopPeriodic()
{
	//Driver Code
	if (Driver.GetRawButton(BUTTON_A)) //If the 'A' button is pressed
	{
		if (isReasonableAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
			moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
		if (isReasonableAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
			turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);

		driveTrain.ArcadeDrive(moveVal, turnVal, false);
	}
	else if (Driver.GetRawButton(BUTTON_LB)) //If the left bumper is pressed
	{
		if (isReasonableAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
			moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
		if (isReasonableAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
			turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);

		driveTrain.ArcadeDrive(moveVal, turnVal, false);
	}
	else if (Driver.GetRawButton(BUTTON_RB)) //If the right bumper is pressed
	{
		if (isReasonableAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
			moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
		if (isReasonableAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
			turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);

		driveTrain.ArcadeDrive(moveVal, turnVal, false);
	}
	else // Tom wuz here
	{
		moveVal = 0;
		turnVal = 0;
		driveTrain.SetLeftRightMotorOutputs(0, 0);
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

	else if (Operator.GetRawAxis(AXIS_TRIGGER_RIGHT)) //If the right trigger is pressed, lift the intake
	{
		intakePivot.Set(Operator.GetRawAxis(AXIS_TRIGGER_RIGHT));
	}
	else if (Operator.GetRawAxis(AXIS_TRIGGER_LEFT)) //If the left trigger is pressed, lower the intake
	{
		intakePivot.Set(-Operator.GetRawAxis(AXIS_TRIGGER_LEFT));
	}
	else if (isReasonableAxisVal(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y))) // If the right stick is moved vertically, rotate the bottom hinge
	{
		bottomArmHinge.Set(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y));
	}
	else if (isReasonableAxisVal(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y))) // If the left stick is moved vertically, rotate the top hinge
	{
		topArmHinge.Set(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y));
	}
}

void Robot::TestPeriodic()
{

}

START_ROBOT_CLASS(Robot)
