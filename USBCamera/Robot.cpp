#include "USBCamera1257.h"
class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
	USBCamera1257 cam = USBCamera1257("cam0");
	Image* img;
	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		img = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		if(cam.getLastError() != IMAQdxErrorSuccess)
		{
			//handle the error
		}
	}

	void AutonomousInit()
	{
		cam.begin();
	}

	void AutonomousPeriodic()
	{
		if(cam.grab(img) != IMAQdxErrorSuccess)
		{
			//handle error
		}else
		{
			CameraServer::GetInstance()->SetImage(img);
		}
		Wait(0.005);
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{

	}

	void TestPeriodic()
	{
		lw->Run();
	}
	void DisabledInit()
	{
		cam.end();
	}
	void DisabledPeriodic(){}
};

START_ROBOT_CLASS(Robot);
