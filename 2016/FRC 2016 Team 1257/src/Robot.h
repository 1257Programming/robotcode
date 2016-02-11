#include "WPILib.h"

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
  	double moveVal = 0;
	double turnVal = 0;
	double bottomHingeAngle = 0;
	double topHingeAngle = 0;
	bool hasRun = false;

	Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void SetDriveMotors(float left, float right);
	void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs);
	
	//New Functions
	bool isOverextended();
	void setHingeAngles();
	
	//Autonomous Functions
	void position1();
	void position2();
	void position3();
	void position4();
	void position5();
	void portcullis();
	void chevalDeFrise();
	void moat();
	void ramparts();
	void drawbridge();
	void sallyPort();
	void rockWall();
	void rougherrain();
	void lowBar();
	
};
