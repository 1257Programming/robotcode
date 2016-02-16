#include "WPILib.h"
#include <cmath>

// X-Box Controller Button IDs
#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_X 3
#define BUTTON_Y 4
#define BUTTON_LB 5
#define BUTTON_RB 6
#define BUTTON_BACK 7
#define BUTTON_START 8
#define BUTTON_ANALOG_CLICK_LEFT 9
#define BUTTON_ANALOG_CLICK_RIGHT 10

// X-Box Controller Axis IDs
#define AXIS_ANALOG_LEFT_X 0
#define AXIS_ANALOG_LEFT_Y 1
#define AXIS_TRIGGER_LEFT 2
#define AXIS_TRIGGER_RIGHT 3
#define AXIS_ANALOG_RIGHT_X 4
#define AXIS_ANALOG_RIGHT_Y 5

#define PI 3.14159265358979323
#define AMTRES 2048 // 2048 PPR
#define WHEEL_DIAMETER 7.625

/* Stuff that Brian added
#define BOTTOM_ARM_LENGTH 16.5 // INCHES
#define TOP_ARM_LENGTH 17.875 // INCHES
#define BOTTOM_ARM_START_ANGLE 0.00001 //TODO:
#define TOP_ARM_START_ANGLE 0.00001    //Define these starting angles ASAP
*/

// Arm Hinge Constants
#define ARM_DISTANCE_PER_PULSE 0.72434607645
#define BOTTOM_HINGE_START 2 //TODO
#define TOP_HINGE_START 2    //Must define these values accurately
#define BOTTOM_ARM_LENGTH 19.0
#define TOP_ARM_LENGTH 18.0

//Global Functions
inline double dabs(double d) { return d > 0.0 ? d : -d; } // Absolute value of a double precision floating point number
inline bool isReasonable(double axisVal) { return dabs(axisVal) > 0.2; } // Ensures the axis is intentionally engaged

class Robot: public IterativeRobot
{
public:
  	//Object Definitions
  	CANTalon frontLeftDrive;
  	CANTalon backLeftDrive;
  	CANTalon frontRightDrive;
    CANTalon backRightDrive;
  	CANTalon intakePivot;
  	Talon intakeSpin;
  	CANTalon bottomArmHinge;
  	CANTalon topArmHinge;

  	ADXRS450_Gyro gyro;
  	Encoder encDriveLeft;
  	Encoder encDriveRight;
  	Encoder encBottomHinge;
  	Encoder encTopHinge;

  	Joystick Driver;
  	Joystick Operator;

  	//Variable Definitions

  	double bottomhorizangle = 0;
  	double tophorizangle = 0;

  	double moveVal = 0;
	double turnVal = 0;
	double bottomHingeAngle = 0;
	double topHingeAngle = 0;

	Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();

	void SetDriveMotors(float left, float right);
	void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs);
	double degtorad(double deg);

	//New Functions
	bool isOverextended();
	void setHingeAngles();
};
