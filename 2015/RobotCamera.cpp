/*
 * RobotCamera.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: Rohan
 */

#include "RobotCamera.h"

using namespace Camera;

RobotCamera::RobotCamera(const char* name, ImageType type) :
		imaqError(), session() {
	frame = NULL;
	this->name = name;
	this->type = type;
}

RobotCamera::~RobotCamera() {
	delete frame;
}

void RobotCamera::Init()
{

	frame = imaqCreateImage(type, 0);

	imaqError = IMAQdxOpenCamera(name, IMAQdxCameraControlModeController, &session);
	if (imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long) imaqError) + "\n");
	}

	imaqError = IMAQdxConfigureGrab(session);
	if (imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long) imaqError) + "\n");
	}

	IMAQdxStartAcquisition(session);
}

Image* RobotCamera::Run()
{
	IMAQdxGrab(session, frame, true, NULL);
	if(imaqError != IMAQdxErrorSuccess)
	{
		DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
	}

	return frame;
}

void RobotCamera::Release()
{
	IMAQdxStopAcquisition(session);
}


