#include "WPILib.h"
#include "math.h"

//Important Constants
#define PI 3.141592653589793238462643383279

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

// Arm Hinge Constants
#define ARM_DISTANCE_PER_PULSE 0.72434607645
#define BOTTOM_HINGE_START 2
#define TOP_HINGE_START 2
#define BOTTOM_ARM_LENGTH 19.0
#define TOP_ARM_LENGTH 18.0

// Global Functions
inline double toDegrees(double radians) { return radians * 180.0 / PI; }
inline double toRadians(double degrees) { return degrees * PI / 180.0; }

class Robot: public IterativeRobot
{
  	// Object Definitions
  	CANTalon frontLeftDrive;
  	CANTalon backLeftDrive;
  	CANTalon frontRightDrive;
    CANTalon backRightDrive;
  	CANTalon intakePivot;
  	Talon intakeSpin;
  	CANTalon bottomArmMotor;
  	CANTalon topArmMotor;
   	//PIDController turning;
    //PIDController bottomArmPID;
    //PIDController topArmPID;
  	RobotDrive driveTrain;
  	Joystick Driver;
  	Joystick Operator;
    Encoder encBottomArm;
  	Encoder encTopArm;
  	Encoder encLeftDrive;
    Encoder encRightDrive;
  
  	// Autonomous Chooser Definitions
    Command *autonomousCommand;
    SendableChooser *chooser;
    
    // Variable Definitions
  	double moveVal = 0;
	double turnVal = 0;
    double bottomHingeAngle = 0;
	double topHingeAngle = 0;
  	bool hasRun = false;
      
public:
  	// Public Member Functions
  
  	// Ensures the axis is being intentionally engaged
	inline bool isGoodAxisVal(double axisVal) { return fabs(axisVal) > 0.2; }
  	
  	// Returns true if the arms ever extends 15 inches past the robot perimeter
  	bool isOverextended()
	{	
      	if (BOTTOM_ARM_LENGTH * cos(bottomHingeAngle) >= 14.5)
        {
        	return true;  
        }
      	return BOTTOM_ARM_LENGTH * cos(bottomHingeAngle) + TOP_ARM_LENGTH * cos(topHingeAngle) >= 14.5;
    }
  	// Read a value from the encoder and finds the angle with the horizontal line that the arm makes for both hinges
    void setHingeAngles()
    {
    	topHingeAngle = TOP_HINGE_START - encTopArm.GetDistance(); // We might need to change the sign to positive
      	bottomHingeAngle = BOTTOM_HINGE_START - encBottomArm.GetDistance();
    }
  
  	Robot():
        frontLeftDrive(0), 
        backLeftDrive(1),		
        frontRightDrive(2), 
        backRightDrive(3), 
		intakePivot(4),
		intakeSpin(5),
		bottomArmMotor(6),
		topArmMotor(7),
  		//turning(),  P, I, D, Source, Output
  		//bottomArmPID(), 
  		//topArmPID(),
        driveTrain(frontLeftDrive, backLeftDrive, frontRightDrive, backRightDrive),
        Driver(0),
        Operator(1),
  		encBottomArm(0, 1),
  		encTopArm(2, 3),
  		encLeftDrive(4, 5),
    	encRightDrive(6, 7)
  	{
 		// Encoder Setup
        encBottomArm.SetDistancePerPulse(ARM_DISTANCE_PER_PULSE);
      	encTopArm.SetDistancePerPulse(ARM_DISTANCE_PER_PULSE);
  	}
	
	void RobotInit()
  	{
  		// Set up options in SmartDashboard for all of the possible autonomous positions
  		chooser = new SendableChooser; // Skeleton code for autonomous SmartDashboard selection
  		chooser->AddObject("Portcullis", autonomousCommand);
  	    chooser->AddObject("Cheval de Frise", autonomousCommand);
  	    chooser->AddObject("Moat", autonomousCommand);
  	    chooser->AddObject("Ramparts", autonomousCommand);
  	    chooser->AddObject("Drawbridge", autonomousCommand);
  	    chooser->AddObject("Sally Port", autonomousCommand);
  	    chooser->AddObject("Rock Wall", autonomousCommand);
  	    chooser->AddObject("Rough Terrain", autonomousCommand);
  	    chooser->AddObject("Low Bar", autonomousCommand);
  	    SmartDashboard::PutData("Autonomous Starting Position", chooser);
  	}
  
	void AutonomousInit()
	{
        autonomousCommand = (Command*) chooser->GetSelected();
        autonomousCommand->Start();
        hasRun = false; // Makes sure this autonomous code only runs once
	}

	void AutonomousPeriodic()
	{
		Scheduler::GetInstance() -> Run;
        if(!hasRun) // Makes sure this autonomous code only runs once
        {
        	// Autonomous code that only runs once goes here
            hasRun = true;
        }
	}

	void TeleopInit()
	{
    	// Sets all the motor speeds to 0
        frontLeftDrive.Set(0); 
        backLeftDrive.Set(0);
        frontRightDrive.Set(0); 
        backRightDrive.Set(0); 
		intakePivot.Set(0);
		intakeSpin.Set(0);
		bottomArmMotor.Set(0);
		topArmMotor.Set(0);
	}

	void TeleopPeriodic()
    {
      	setHingeAngles();
      
      	// Driver Code
        if (Driver.GetRawButton(BUTTON_A)) // If the 'A' button is pressed, use Arcade Drive
        {	
          	if (isGoodAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y))) 
            {
            	moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y); 
            }
            if (isGoodAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
            {
                turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
            }
          	driveTrain.ArcadeDrive(moveVal, turnVal, false);
        }
        else if (Driver.GetRawButton(BUTTON_LB)) // If the left bumper is pressed, use Arcade Drive
        {
        	if (isGoodAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
            { 
                moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
            }
            if (isGoodAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
            { 
                turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
            }
            driveTrain.ArcadeDrive(moveVal, turnVal, false);
        }
      	else if (Driver.GetRawButton(BUTTON_RB)) // If the right bumper is pressed, use Arcade Drive
        {
        	if (isGoodAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
            { 
                moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
            }
            if (isGoodAxisVal(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
            { 
                turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
            }
            driveTrain.ArcadeDrive(moveVal, turnVal, false);
        }
      	else // If nothing is being pressed, don't move
        {
        	moveVal = 0;
          	turnVal = 0;
          	driveTrain.SetLeftRightMotorOutputs(0, 0);
        } 
      	
      	// Operator Code
      	if (Operator.GetRawButton(BUTTON_A)) // If 'A' is being pressed, spin the intake inwards quickly
        {
        	intakeSpin.Set(1);
        }
        else if (Operator.GetRawButton(BUTTON_B)) // If 'B' is being pressed, spin the intake outwards quickly
        {
        	intakeSpin.Set(-1);
        }
        else if (Operator.GetRawButton(BUTTON_X)) // If 'X' is pressed, spin the intake inwards slowly
        {
         	intakeSpin.Set(0.5); 
        }
        else if (Operator.GetRawButton(BUTTON_Y)) // If 'Y' is pressed, spin the intake outwards slowly
        {
            intakeSpin.Set(-0.5);
        }
      	else // If nothing is being pressed, then do not spin the intake
        {
        	intakeSpin.Set(0);  
        }

        if (Operator.GetRawAxis(AXIS_TRIGGER_RIGHT)) // If the right trigger is pressed, lift the intake
        {
            intakePivot.Set(Operator.GetRawAxis(AXIS_TRIGGER_RIGHT));
        }
      	else if (Operator.GetRawAxis(AXIS_TRIGGER_LEFT)) // If the left trigger is pressed, lower the intake
        {
            intakePivot.Set(-Operator.GetRawAxis(AXIS_TRIGGER_LEFT));
        }
      	else // If no trigger is being pressed, then don't pivot the intake
        {
            intakePivot.Set(0);
        }
                
        if (isGoodAxisVal(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y)) && !isOverextended()) // If the right stick is moved vertically, rotate the bottom arm hinge 
        {
            bottomArmMotor.Set(0.4 * pow(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y), 2)); // Scale factor is 0.4
        }
      	else // If the right stick is not being pressed, then not rotate the bottom arm hinge
        {
            bottomArmMotor.Set(0);
      	}
        
        if (isGoodAxisVal(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y)) && !isOverextended()) // If the left stick is moved vertically, rotate the top arm hinge
        {
            topArmMotor.Set(0.4 * pow(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y), 2)); // Scale factor is 0.4
        }
      	else // If the left stick is not being engaged, do not rotate the top arm hinge
        {
            topArmMotor.Set(0);
      	}
      
    }

	void TestPeriodic()
	{
    	
	}
  
  	
};
  
START_ROBOT_CLASS(Robot)
// You can never have too many comments
