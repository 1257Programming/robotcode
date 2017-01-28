/*
 * robot.h
 *
 *  Created on: Jan 1337, 420
 */
#ifndef ROBOT_H_
#define ROBOT_H_
#include "WPILib.h"
#include "CANTalon.h"

// Robot Measurements
#define WHEEL_DIAMETER 6 //Inches
#define PULSES_PER_REV 2048

// Math Constants
#define PI 3.14

class Robot : public IterativeRobot{

  private:

  	XboxController Driver;
    XboxController Operator;

  	AnalogGyro Gyro;
  	Encoder LeftEnc;
  	Encoder RightEnc;
    CANTalon FrontLeftDrive;
    CANTalon BackLeftDrive;
    CANTalon FrontRightDrive;
    CANTalon BackRightDrive;
    CANTalon GearPivot;
    CANTalon ClimbMotor;

    RobotDrive DriveTrain;

    double Kp = .03; // Relic of years past

  public:
    Robot();
	// Virtual Functions
    void RobotInit();
  	void AutonomousInit();
  	void AutonomousPeriodic();
  	void TeleopInit();
  	void TeleopPeriodic();
  	// Helper Functions
  	void LeftSpawn();
  	void CenterSpawn();
  	void RightSpawn();
  	void TeleopDriver();
  	void TeleopOperator();
  	void DriveForward(float distance, double speed);
  	void Turn(double angle);

};

#endif
