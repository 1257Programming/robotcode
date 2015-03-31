#include "WPILib.h" // Necessary header file for using WPILib functions and classes
#include <sstream> // Stringstreams are used for conversion of numbers into string
#include "CameraServer.h" // New class used to send camera images to the dashboard

class Team1257Robot : public IterativeRobot // By inheriting from IterativeRobot, our program's main class does not require loops to continue operating
{
public:
	CANTalon Left; // The Talon SRX controller for the Left motor that drives the forward wheels on the left side
	CANTalon Right; // The Talon SRX  controller for the Right motor that drives the forward wheels on the right side
	CANTalon Center; // The Talon SRX for the Center motor that drives the Center slide wheel
	CANTalon Lift; // The Talon SRX for the Elevator's motor
	Joystick Stick1; // Manages input from the XBox 360 controller used to control the drive train
	Joystick Stick2; // Manages input from the XBox 360 controller used to control the elevator and arms
	PowerDistributionPanel Pdp; // Used for displaying and managing current drawn by motors
	double forwardback = 0; // Variable determining the current output value for drive Talons in forward/backward motion
	double strafe = 0; // Variable determining the current output value for drive Talon of the center
	double turn = 0; // Variable determining the current output value for drive Talons of the left and right in rotation/pivoting
	DoubleSolenoid dSolenoid; // The Solenoid valve connected to the Pneumatics Control Module
	
	DoubleSolenoid stepLiftSolenoid;
	
	DigitalInput bottomlimit; // A limit switch that is pressed as the elevator hits the bottom
	DigitalInput toplimit; // A limit switch that is pressed as the elevator hits the bottom

	Gyro angle; // ADW22307 Gyroscope sensor used to retrieve rotational angles in autonomous

	bool lsignore = false;

	Team1257Robot(); // Function Prototype of class constructor

	void AutonomousInit(); // Runs at AUTO start
	void AutonomousPeriodic(); // Runs throughout AUTO
	void TeleopInit(); // Runs at Teleop start
	void TeleopPeriodic(); // Runs throughout teleop
	void TestInit(); // Runs upon entering Test Mode
	void TestPeriodic(); // Test code
	bool TestAngle(int angle); // Returns false until the robot has completed a turn of measure "angle"
	double accel(Joystick& stick, int axis, double& current, double sf, double inc = 0.5); // Function for managing sudden shifts in joystick values
	inline double dAbs(double x); // Absolute value function
};
