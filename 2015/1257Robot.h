#include "WPILib.h"
#include <sstream>
#include "CameraServer.h"

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
	AnalogInput Ultra;

	DigitalInput bottomlimit;
	DigitalInput toplimit;

	Team1257Robot();
	~Team1257Robot();

	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestInit();
	void TestPeriodic();
	void DisabledPeriodic();
	double accel(Joystick& stick, int axis, double& current, double sf, double inc = 0.5);
	inline double dAbs(double x);
};
