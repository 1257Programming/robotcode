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
//#define BOTTOM_ARM_LENGTH 16.5 // INCHES
//#define TOP_ARM_LENGTH 21 // INCHES
#define BOTTOM_WEDGE_START_ANGLE 149 //DEG
//#define TOP_ARM_START_ANGLE 2.7 //GREES
#define INTAKE_HIGH_PWM 3800
#define INTAKE_LOW_PWM 2900

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
  	CANTalon wedgeHinge;
//  	CANTalon bottomArmHinge;
//  	CANTalon topArmHinge;
  	PWM pwmOne;
  	PWM pwmTwo;
  	PWM pwmThree;
  	PWM pwmFour;

  	USBCamera Lifecam;
  	ADXRS450_Gyro gyro;
  	Encoder encDriveLeft;
  	Encoder encDriveRight;
  	Encoder encBottomHinge;
//  	Encoder encTopHinge;
  	DigitalInput breakBeam;
  	BuiltInAccelerometer biAccel;

  	Joystick Driver;
  	Joystick Operator;

  	PIDController pidGyro;
  	PIDController pidIntake;

  	std::thread* dashcam = NULL;
  	Image* dashframe = NULL;

  	//Variable Definitions

  	double wedgehorizangle = 0;
  	long intakestart = 0;
  	long intakebottom = 0;

  	double moveVal = 0;
	double turnVal = 0;

	bool breakbeamenabled = true;
	bool haveball = false;
	bool autoran = false;
	bool feedenabled = false;

	Robot();
	void DisabledInit();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestInit();
	void TestPeriodic();

	void SetDriveMotors(float left, float right);
	void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs);
//	bool isArmOverextended();
	double degtorad(double deg);
	void intakeAdjust(bool pos); // true to raise, false to lower
	void wedgeAdjust(bool pos); //same argument
	void stopIntake();
	void reach();
	void scorepath();
	void forcross();
	void backcross();
	void portcullis();
	void cheval();

	void dashstatus();
	void dashcamera();

	void LEDRaveMode();
	void LEDIntakeOutBlue();
	void LEDIntakeOutRed();
	void LEDIntakeInBlue();
	void LEDIntakeInRed();
	void LEDTeleopWithBallBlue();
	void LEDTeleopWithBallRed();
	void LEDTeleopIdleBlue();
	void LEDTeleopIdleRed();
	void LEDAutonomousIdle();
};
