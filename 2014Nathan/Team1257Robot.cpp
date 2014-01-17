#include "Team1257Robot.h"
#include <cassert>
Team1257Robot::Team1257Robot():
team1257Robot(LEFT,RIGHT),
LeftStick(LEFT),
RightStick(RIGHT),
LeftArm(LARM),
RightArm(RARM),
Arm1(LARM),
Arm2(RARM),
armBend(BEND)
{
	team1257LCD = DriverStationLCD::GetInstance();
	team1257Robot.SetExpiration(.1);
	leftStickIsEnabled = false;
	rightStickIsEnabled = false;
	SetWhichDrive(TANK_DRIVE);
}
void Team1257Robot::Autonomous()
{
	while(IsAutonomous())
	{
		Drive(.3,.3);
		Wait(4);
		Drive(0,0);
		while(true);
	}
}
void Team1257Robot::OperatorControl()
{
	while(IsOperatorControl())
	{
		CheckSetDriveSticks();
		this->printf("Driving with %i",this->GetWhichDrive());
		switch(GetWhichDrive())
		{
		case TANK_DRIVE:
			TankDrive();
			break;
		case ARCADE_DRIVE:
			ArcadeDrive();
			break;
		case ZTWIST_DRIVE:
			ZTwistDrive();
			break;
		case XY_DRIVE:
			XYDrive();
			break;
		}
		if(this->GetLeftArm()->GetRawButton(1) && this->GetRightArm()->GetRawButton(1))
		{
			this->SetArmsX((this->GetLeftArm()->GetX() + this->GetRightArm()->GetX())/2);
			this->SetArmsY((this->GetLeftArm()->GetY() + this->GetRightArm()->GetY())/2);
		}
                else if(this->GetLeftStick()->GetRawButton(12)
                {
                        this->SetArmsX((this->GetLeftArm()->GetX() + this->GetLeftArm->GetZ())/2);
                        this->SetArmsY((this->GetLeftArm()->GetY() + this->GetLeftArm->GetTwist())/2);
                }
		this->GetLCD()->Clear();
		this->GetLCD()->UpdateLCD();
	}
}
void Team1257Robot::TankDrive()
{
	if(this->leftStickIsEnabled && this->rightStickIsEnabled)
		this->GetDrive()->SetLeftRightMotorOutputs(-.5*this->GetLeftStick()->GetY(),-.5*this->GetRightStick()->GetY());
	else if (this-leftStickIsEnabled)
		this->GetDrive()->SetLeftRightMotorOutputs(-.5*this->GetLeftStick()->GetY(),-.5*this->GetLeftStick()->GetTwist());
}
void Team1257Robot::ArcadeDrive()
{
	if(this->rightStickIsEnabled && this->leftStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick(),1,this->GetRightStick(),2,false);
	}
	else if (this->leftStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick(),false);
	}
	else if (this->rightStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetRightStick(),false);
	}
	else this->Drive(0,0);
}
void Team1257Robot::ZTwistDrive()
{
	if(rightStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetRightStick()->GetZ(), this->GetRightStick()->GetTwist(),false);
	}
	else if (leftStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick()->GetZ(),this->GetLeftStick()->GetTwist(),false);
	}
}
void Team1257Robot::XYDrive()
{
	if(rightStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetRightStick()->GetX(), this->GetRightStick()->GetY(),false);
	}
	else if (leftStickIsEnabled)
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick()->GetX(),this->GetLeftStick()->GetY(),false);
	}
}
void Team1257Robot::Drive(float left, float right)
{
	this->GetDrive()->SetLeftRightMotorOutputs(-left, -right);
}
int Team1257Robot::GetWhichDrive()
{
	return this->whichDrive;
}
void Team1257Robot::SetWhichDrive(int value)
{
	this->whichDrive = value;
}
void Team1257Robot::CheckSetDriveSticks()
{
	if(this->GetLeftStick()->GetRawButton(5) && this->GetLeftStick()->GetRawButton(6))
	{
		SetWhichDrive(TANK_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(1) && this->GetRightStick()->GetRawButton(1))
	{
		SetWhichDrive(TANK_DRIVE);
		rightStickIsEnabled = leftStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(1))
	{
		SetWhichDrive(ARCADE_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetRightStick()->GetRawButton(1))
	{
		SetWhichDrive(ARCADE_DRIVE);
		rightStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(12))
	{
		SetWhichDrive(ZTWIST_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetRightStick()->GetRawButton(12))
	{
		SetWhichDrive(ZTWIST_DRIVE);
		rightStickIsEnabled = true;
	}
	else if(this->GetLeftStick()->GetRawButton(11))
	{
		SetWhichDrive(XY_DRIVE);
		leftStickIsEnabled = true;
	}
	else if(this->GetRightStick()->GetRawButton(11))
	{
		SetWhichDrive(XY_DRIVE);
		rightStickIsEnabled = true;
	}
}
void Team1257Robot::SetArmsX(float value)
{
	this->GetVictorInstanceFromNumber(1)->Set(value);
	this->GetVictorInstanceFromNumber(2)->Set(-value);
}
void Team1257Robot::SetArmsY(float value)
{
	this->GetArmBend()->Set(value);
}
void Team1257Robot::printf(char * Template , ...)
{
	va_list subs;
	va_start(subs, Template);
	this->GetLCD()->VPrintf(this->GetLCD()->kUser_Line1,1,Template,subs);
	va_end(subs);
	this->GetLCD()->UpdateLCD();
}
RobotDrive * Team1257Robot::GetDrive()
{
	return &(this->team1257Robot);
}
Joystick * Team1257Robot::GetRightStick()
{
	return Joystick::GetStickForPort(2);
}
Joystick * Team1257Robot::GetLeftStick()
{
	return Joystick::GetStickForPort(1);
}
Joystick * Team1257Robot::GetRightArm()
{
	return Joystick::GetStickForPort(4);
}
Joystick * Team1257Robot::GetLeftArm()
{
	return Joystick::GetStickForPort(3);
}
Victor * Team1257Robot::GetArmBend()
{
	return &(this->armBend);	
}
Victor * Team1257Robot::GetVictorInstanceFromNumber(int num)
{
	switch(num)
	{
	case 1:
		return &this->Arm1;
	case 2:
		return &this->Arm2;
	default:
		return NULL;
	}
}
DriverStationLCD * Team1257Robot::GetLCD()
{
	return this->team1257LCD;
}
RobotBase * FRC_userClassFactory() 
{ 
	return new Team1257Robot(); 
} 
extern "C" { 
	int32_t FRC_UserProgram_StartupLibraryInit() 
	{ 
		RobotBase::startRobotTask((FUNCPTR)FRC_userClassFactory); 
		return 0; 
	} 
}
