#include "1257Robot.h"

using namespace std;

template<typename T>
std::string ToString(T t)
{
	std::stringstream ss;
	ss << t;
	string res = "";
	ss >> res;
	return res;
}


Team1257Robot::Team1257Robot():
	Left(0), Right(1), Center(2), Lift(3), Stick1(0), Stick2(1), dSolenoid(4, 5), Ultra(0),
	bottomlimit(0), toplimit(1)
{
	Lw = LiveWindow::GetInstance();
}

Team1257Robot::~Team1257Robot()
{

}

void Team1257Robot::TeleopInit()
{
	CameraServer::GetInstance()->StartAutomaticCapture("cam1");
}

void Team1257Robot::TeleopPeriodic() //NOTE: THE MOTORS TURN ARE FLIPPED, SO TO HAVE THEM GO IN THE SAME DIRECTION, ONE OF THEM MUST BE NEGATED
{
	double sf = .8;//scale factor for scaling drive values
	double strafesf = 1; //scale factor for the strafe wheel since it is weaker

	//1ST CONTROLER

	if (Stick1.GetRawButton(5) && !Stick1.GetRawButton(6))
		/*Safety switches - these need to be clicked to do anything
		 * just one bumper, not both
		 * which bumper is clicked determines which analog is move forward/back/left/right
		 * Here - if the left bumper is clicked, the left analog is is move forward/back/left/right
		 * the right analog is pivot
		 */
	{
		Center.Set(-(float)accel(Stick1, 0, strafe, strafesf, 0.2)); //smoothly set the speed of the strafe to the left/right values of the left analog
		if(!Stick1.GetRawAxis(4) || dAbs(Stick1.GetRawAxis(4)) < .2)
			/*
			 * account for imperfections in the analog and prevent extremely small pushes of the analog from affecting the robot
			 * make sure that the pivot analog isn't being pushed - if so, give priority to the turning analog
			 */
		{
			Left.Set(-(float)accel(Stick1, 1, forwardback, sf)); //smoothly set the value of the left motor
				//to the negative of left analog forward/back (Look at the note above for why)
			Right.Set((float)accel(Stick1, 1, forwardback, sf)); //smoothly set the value of the right motor to
				//the left analog forward/back
		}
		else //turn
		{
			Left.Set((float)accel(Stick1, 4, turn, sf)); //smoothly set the value of the left motor to the
				//left analog forward/back
			Right.Set((float)accel(Stick1, 4, turn, sf)); //smoothly set the value of the right motor to the
				//left analog forward/back
		}
	}
	else if (!Stick1.GetRawButton(5) && Stick1.GetRawButton(6))
	{
		/*Safety switches - these need to be clicked to do anything
		 * just one bumper, not both
		 * which bumper is clicked determines which analog is move forward/back/left/right
		 * Here - if the right bumper is clicked, the right analog is is move forward/back/left/right
		 * the left analog is pivot
		 */
		Center.Set(-(float)accel(Stick1, 4, strafe, strafesf, 0.2)); //smoothly set the speed of the strafe to the value of the right analog
		if(!Stick1.GetRawAxis(0) || dAbs(Stick1.GetRawAxis(0)) < .2)
			/*
			 * account for imperfections in the analog and prevent extremely small pushes of the analog from affecting the robot
			 * make sure that the pivot analog isn't being pushed - if so, give priority to the turning analog
			 */
		{
			Left.Set(-(float)accel(Stick1, 5, forwardback, sf)); //smoothly set the value of the left motor
			//to the negative of right analog forward/back (Look at the note above for why)
			Right.Set((float)accel(Stick1, 5, forwardback, sf)); //smoothly set the value of the right motor to
			//the right analog forward/back
		}
		else //turn
		{
			Left.Set((float)accel(Stick1, 0, turn, sf)); //smoothly set the value of the left motor to the
			//right analog forward/back
			Right.Set((float)accel(Stick1, 0, turn, sf)); //smoothly set the value of the right motor to the
			//right analog forward/back
		}

	}
	else //safety switches not clicked -> DON'T MOVE
	{
		Left.Set(0);
		Right.Set(0);
		Center.Set(0);
	}

	//2ND CONTROLER

	if (Stick2.GetRawButton(6)) //Right bumper clicked
	{
		dSolenoid.Set(DoubleSolenoid::kForward); //open the arms
	}
	else if(Stick2.GetRawButton(5)) //left bumper clicked
	{
		dSolenoid.Set(DoubleSolenoid::kReverse); //close the arms
	}
	else //f neither
		dSolenoid.Set(DoubleSolenoid::kOff); //DON'T DO ANYTHING!

	double liftval = Stick2.GetRawAxis(3) - Stick2.GetRawAxis(2); //get the value of the left and right triggers together

	if((!bottomlimit.Get() || liftval > 0) && (!toplimit.Get() || liftval < 0))
		/*
		 * neither the top nor bottom limit switches are clicked
		 * if it is at the bottom limit but wants to move up, allow it to move
		 * if it is at the top limit but wants to move down, allow it to move
		 */
		Lift.Set(liftval); //move the lift
	else
		Lift.Set(0); //DON'T MOVE

	//push values to the dashboard

	SmartDashboard::PutString("DB/String 0", "Channel 12:");
	SmartDashboard::PutString("DB/String 1", ToString(Pdp.GetCurrent(12)));

	SmartDashboard::PutString("DB/String 2", "Channel 13:");
	SmartDashboard::PutString("DB/String 3", ToString(Pdp.GetCurrent(13)));

	SmartDashboard::PutString("DB/String 4", "Channel 14:");
	SmartDashboard::PutString("DB/String 5", ToString(Pdp.GetCurrent(14)));

	SmartDashboard::PutString("DB/String 6", "Channel 15:");
	SmartDashboard::PutString("DB/String 7", ToString(Pdp.GetCurrent(15)));
}
void Team1257Robot::AutonomousInit()
{

}

void Team1257Robot::AutonomousPeriodic()
{
	if(!bottomlimit.Get() && !toplimit.Get())
	{
		dSolenoid.Set(DoubleSolenoid::kForward);
		Wait(.2);
		Lift.Set(.9);
		Wait(2.5);
		Lift.Set(0);
		Right.Set(-.5);
		Left.Set(.5);
		Wait(1);
		Right.Set(0);
		Left.Set(0);
		Lift.Set(-.55);
		Wait(1);
		dSolenoid.Set(DoubleSolenoid::kReverse);
		Wait(2);
		Center.Set(-.5);
		Right.Set(.3);
		Left.Set(-.3);
		Wait(.5);
		Center.Set(0);
		Left.Set(0);
		Right.Set(0);
		Lift.Set(-.55);
		Wait(2);
		dSolenoid.Set(DoubleSolenoid::kForward);
		Wait(1);
		Lift.Set(.35);
		Wait(1);
		Right.Set(.5);
		Left.Set(.5);
		Wait(1.3);
		Right.Set(-.75);
		Left.Set(.75);
		Wait(2.5);
		Right.Set(0);
		Left.Set(0);
	}
	else
		Lift.Set(0);

}

void Team1257Robot::TestInit()
{

}

void Team1257Robot::TestPeriodic()
{
	Right.Set(.5);
	Left.Set(.5);
}

void Team1257Robot::DisabledPeriodic()
{

}

double Team1257Robot::accel(Joystick& Stick, int axis, double& current, double sf, double inc)
{
	double raw = Stick.GetRawAxis(axis);
	if(raw > current && raw > 0) // If speeding up, increment it there, instead of sudden jerk
		current += inc;
	if(raw < current && raw < 0) // Same as above, taking into account negative joyStick1 values
		current -= inc;
	if(dAbs(raw) < dAbs(current)) // If the target speed is lesser, reduce to that instantly, like for stopping
		current = raw;
	if(dAbs(raw) < .2) //Taking into account SLIGHTLY off-centered axes
		current = 0;
	return (current * sf);
}

inline double Team1257Robot::dAbs(double x)
{
	if(x >= 0)
		return x;
	else
		return -x;
}

START_ROBOT_CLASS(Team1257Robot);
