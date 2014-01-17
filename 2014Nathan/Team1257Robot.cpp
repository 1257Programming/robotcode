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
	this->SetLCDInstance(DriverStationLCD::GetInstance());
	this->GetDrive()->SetExpiration(.1);
	this->SetLeftStickEnabled(false);
	this->SetRightStickEnabled(false);
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
		else if(this->GetLeftArm()->GetRawButton(11))
		{
			this->SetArmsX((this->GetLeftArm()->GetX() + this->GetLeftArm()->GetZ())/2);
			this->SetArmsY((this->GetLeftArm()->GetY() + this->GetLeftArm()->GetTwist())/2);
		}
		this->GetLCD()->Clear();
		this->GetLCD()->UpdateLCD();
	}
}
void Team1257Robot::TankDrive()
{
	if(this->GetLeftStickEnabled() && this->GetRightStickEnabled())
	{
		float sf = .1;
		while(sf <= .5)
		{
			this->GetDrive()->SetLeftRightMotorOutputs(-sf*this->GetLeftStick()->GetY(),-(sf)*this->GetRightStick()->GetY());
			sf += .1;
		}
	}
	else if (this->GetLeftStickEnabled())
	{
		float sf = .1;
		while(sf <= .5)
		{
			this->GetDrive()->SetLeftRightMotorOutputs(-sf*this->GetLeftStick()->GetY(),-sf*this->GetLeftStick()->GetTwist());
			sf += .1;
		}
	}
}
void Team1257Robot::ArcadeDrive()
{
	if(this->GetRightStickEnabled() && this->GetLeftStickEnabled())
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick(),1,this->GetRightStick(),2,false);
	}
	else if (this->GetLeftStickEnabled())
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick(),false);
	}
	else if (this->GetRightStickEnabled())
	{
		this->GetDrive()->ArcadeDrive(this->GetRightStick(),false);
	}
	else this->Drive(0,0);
}
void Team1257Robot::ZTwistDrive()
{
	if(this->GetRightStickEnabled())
	{
		this->GetDrive()->ArcadeDrive(this->GetRightStick()->GetZ(), this->GetRightStick()->GetTwist(),false);
	}
	else if (this->GetLeftStickEnabled())
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick()->GetZ(),this->GetLeftStick()->GetTwist(),false);
	}
}
void Team1257Robot::XYDrive()
{
	if(this->GetRightStickEnabled())
	{
		this->GetDrive()->ArcadeDrive(this->GetRightStick()->GetX(), this->GetRightStick()->GetY(),false);
	}
	else if (this->GetLeftStickEnabled())
	{
		this->GetDrive()->ArcadeDrive(this->GetLeftStick()->GetX(),this->GetLeftStick()->GetY(),false);
	}
}
void Team1257Robot::Drive(float left, float right)
{
	float sf = .1;
	while (sf <= 1)
	{
		this->GetDrive()->SetLeftRightMotorOutputs(-sf*left, -sf*right);
		sf += .2;
	}
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
		this->SetLeftStickEnabled(true);
	}
	else if(this->GetLeftStick()->GetRawButton(1) && this->GetRightStick()->GetRawButton(1))
	{
		SetWhichDrive(TANK_DRIVE);
		this->SetLeftStickEnabled(true);
		this->SetRightStickEnabled(true);
	}
	else if(this->GetLeftStick()->GetRawButton(1))
	{
		SetWhichDrive(ARCADE_DRIVE);
		this->SetLeftStickEnabled(true);
	}
	else if(this->GetRightStick()->GetRawButton(1))
	{
		SetWhichDrive(ARCADE_DRIVE);
		this->SetRightStickEnabled(true);
	}
	else if(this->GetLeftStick()->GetRawButton(12))
	{
		SetWhichDrive(ZTWIST_DRIVE);
		this->SetLeftStickEnabled(true);
	}
	else if(this->GetRightStick()->GetRawButton(12))
	{
		SetWhichDrive(ZTWIST_DRIVE);
		this->SetRightStickEnabled(true);
	}
	else if(this->GetLeftStick()->GetRawButton(11))
	{
		SetWhichDrive(XY_DRIVE);
		this->SetLeftStickEnabled(true);
	}
	else if(this->GetRightStick()->GetRawButton(11))
	{
		SetWhichDrive(XY_DRIVE);
		this->SetRightStickEnabled(true);
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
bool Team1257Robot::GetLeftStickEnabled()
{
	return this->leftStickIsEnabled;
}
void Team1257Robot::SetLeftStickEnabled(bool value)
{
	this->leftStickIsEnabled = value;
}
bool Team1257Robot::GetRightStickEnabled()
{
	return this->rightStickIsEnabled;
}
void Team1257Robot::SetRightStickEnabled(bool value)
{
	this->rightStickIsEnabled = value;
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
		return &(this->Arm1);
	case 2:
		return &(this->Arm2);
	default:
		return NULL;
	}
}
DriverStationLCD * Team1257Robot::GetLCD()
{
	return this->team1257LCD;
}
void Team1257Robot::SetLCDInstance(DriverStationLCD * instance)
{
	this->team1257LCD = instance;
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
