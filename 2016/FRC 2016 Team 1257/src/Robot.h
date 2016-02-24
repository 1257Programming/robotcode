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
#define BOTTOM_ARM_LENGTH 16.5 // INCHES
#define TOP_ARM_LENGTH 17.875 // INCHES
#define BOTTOM_ARM_START_ANGLE 169 //DEG
#define TOP_ARM_START_ANGLE 2.7 //GREES

// Autonomous Values (All Units in Inches)
#define DIST_TO_DEFENSE 40 // INCHES, ROBOT LENGTH SUBTRACTED (74 - 34)
#define DIST_LOW_BAR 82 // INCHES, ROBOT LENGTH ADDED (48 + 34)
// Distances to get
#define DIST_MOAT 80
#define DIST_RAMPARTS 80
#define DIST_ROCK_WALL 80
#define DIST_ROUGH_TERRAIN 80
#define DIST_CHEVAL_DE_FRISE 80

//Global Functions
inline double dabs(double d) { return d > 0.0 ? d : -d; } // Absolute value of a double precision floating point number
inline bool isReasonable(double axisVal) { return dabs(axisVal) > 0.2; } // Ensures the axis is intentionally engaged

class Robot: public IterativeRobot 
{
public:
	// Object Definitions
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
	DigitalInput breakBeam;
	BuiltInAccelerometer biAccel;

	Joystick Driver;
	Joystick Operator;
	
	DriverStation dStation = DriverStation.GetInstance();

	PIDController pidGyro;
	PIDController pidIntake;
	
	PWM pwmOne(1); 
	PWM pwmTwo(2); 
	PWM pwmThree(3); 
	PWM pwmFour(4); 
	
	// Break beam in ball intake
	DigitalInput breakBeam(1);

	// Functions for lights
	void LEDRaveMode();
	void LEDIntakeOutBlue();
	void LEDIntakeOutRed();
	void LEDIntakeInBlue();
	void LEDIntakeInRed();
	void LEDTeleopWithBallBlue();
	void LEDTeleopWithBallRed();
	void LEDTeleopIdleBlue();
	void LEDTeleopIdleRed();
	void IdleAutonomous();
	
	// Variable Definitions

	double bottomhorizangle = 0;
	double tophorizangle = 0;

	double moveVal = 0;
	double turnVal = 0;

	bool breakbeamenabled = false;

	Robot();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();

	void SetDriveMotors(float left, float right);
	void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs);
	bool isArmOverextended();
	double degtorad(double deg);
	double avg(double firstValue, double secondValue);

	// Autonomous Code
	void drivetoDefense();
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
	void roughTerrain();
	void lowBar();
	void driveToDefense();
	void driveForward(double distance, bool isFast);
	void turn(double endAngle);
};
