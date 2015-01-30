#include "WPILib.h"
#include <sstream>

class Team1257Robot : public IterativeRobot
{
public:
	CANTalon Left;
	CANTalon Right;
	CANTalon Center;
	CANTalon Lift;
	Joystick Stick1;
	Joystick Stick2;
	LiveWindow* Lw;
	PowerDistributionPanel Pdp;
	double forwardback = 0;
	double strafe = 0;
	double turn = 0;
	DoubleSolenoid dSolenoid;

	Team1257Robot();

	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestInit();
	void TestPeriodic();
	double accel(Joystick& stick, int axis, double& current, double sf);
	inline double dAbs(double x);
};
