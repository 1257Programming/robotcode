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

//Global Functions
inline double dabs(double d) { return d > 0.0 ? d : -d; } // Absolute value of a double precision floating point number
inline bool isReasonable(double axisVal) { return dabs(axisVal) > 0.2; } // Ensures the axis is intentionally engaged
			
class Robot: public IterativeRobot
{
  	//Object Definitions
  	CANTalon frontLeftDrive;
  	CANTalon backLeftDrive;
  	CANTalon frontRightDrive;
    CANTalon backRightDrive;
  	CANTalon intakePivot;
  	Talon intakeSpin;
  	CANTalon bottomArmHinge;
  	CANTalon topArmHinge;
  	RobotDrive driveTrain;
  	Joystick Driver;
  	Joystick Operator;
  
  	//Variable Definitions
  	double moveVal = 0;
	double turnVal = 0;  

//RobotDrive (SpeedController &frontLeftMotor, SpeedController &rearLeftMotor, SpeedController &frontRightMotor, SpeedController &rearRightMotor)
public:
  	Robot() :
        frontLeftDrive(0), 
        backLeftDrive(1),
        frontRightDrive(2), 
        backRightDrive(3), 
		intakePivot(4),
		intakeSpin(5),
		bottomArmHinge(6),
		topArmHinge(7),
        driveTrain(frontLeftDrive, backLeftDrive, frontRightDrive, backRightDrive),
        Driver(0),
        Operator(1)
  	{
  		 
  	}
	
	void RobotInit()
	{
    	
	}

  	
	void AutonomousInit()
	{
    	
	}

	void AutonomousPeriodic()
	{
		
	}

	void TeleopInit()
	{
    	//set all the motor values to 0
        
	}

	void TeleopPeriodic()
    {
      	//Driver Code
        if (Driver.GetRawButton(BUTTON_A)) //If the 'A' button is pressed
        {	
          	if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
            {
            	moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
            }
            if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
            {
                turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
            }
          	driveTrain.ArcadeDrive(moveVal, turnVal, false);
        }
        else if (Driver.GetRawButton(BUTTON_LB)) //If the left bumper is pressed
        {
        	if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
            { 
                moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
            }
            if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
            { 
                turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
            }
            driveTrain.ArcadeDrive(moveVal, turnVal, false);
        }
      	else if (Driver.GetRawButton(BUTTON_RB)) //If the right bumper is pressed
        {
        	if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y)))
            { 
                moveVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_Y);
            }
            if (isReasonable(Driver.GetRawAxis(AXIS_ANALOG_LEFT_X)))
            { 
                turnVal = Driver.GetRawAxis(AXIS_ANALOG_LEFT_X);
            }
            driveTrain.ArcadeDrive(moveVal, turnVal, false);
        }
      	else
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
      	else
        {
        	intakeSpin.Set(0);  
        }

        if (Operator.GetRawAxis(AXIS_TRIGGER_RIGHT)) //If the right trigger is pressed, lift the intake
        {
            intakePivot.Set(Operator.GetRawAxis(AXIS_TRIGGER_RIGHT));
        }
      	else if (Operator.GetRawAxis(AXIS_TRIGGER_LEFT)) //If the left trigger is pressed, lower the intake
        {
            intakePivot.Set(-Operator.GetRawAxis(AXIS_TRIGGER_LEFT));
        }
      	else 
        {
            intakePivot.Set(0);
        }
                
        if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y))) // If the right stick is moved vertically, rotate the bottom hinge 
        {
            bottomArmHinge.Set(Operator.GetRawAxis(AXIS_ANALOG_RIGHT_Y));
        }
      	else 
        {
            bottonArmHinge.Set(0);
      	}
        if (isReasonable(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y))) // If the left stick is moved vertically, rotate the top hinge
        {
            topArmHinge.Set(Operator.GetRawAxis(AXIS_ANALOG_LEFT_Y));
        }
      	else 
        {
            topArmHinge.Set(0);
      	}
    }

	void TestPeriodic()
	{
    	
	}
};

START_ROBOT_CLASS(Robot)//the truth is out there

