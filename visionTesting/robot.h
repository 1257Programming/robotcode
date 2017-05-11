/*
 * Robot.h
 *
 *  Created on: Apr 17, 2017
 *      Author: zhama + Tim
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

#include "WPILib.h"
#include "CANTalon.h"
#include <math.h>

using namespace frc;

class TrainingRobit : public IterativeRobot
{
private:
	CANTalon FrontLeftDrive;
	CANTalon BackLeftDrive;
	CANTalon FrontRightDrive;
	CANTalon BackRightDrive;
	RobotDrive DriveTrain;

	ADXRS450_Gyro Gyro;

	int numberofMotors;
public:
	TrainingRobit();
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void DisabledInit();
	void UpdateDashboard();

};
#endif /* SRC_ROBOT_H_ */
