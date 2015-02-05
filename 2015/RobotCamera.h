/*
 * RobotCamera.h
 *
 *  Created on: Feb 4, 2015
 *      Author: Rohan
 */

#ifndef ROBOTCAMERA_H_
#define ROBOTCAMERA_H_

#define NULL 0

namespace Camera
{

class RobotCamera
{
protected:
	IMAQdxSession session;
	Image *frame;
	IMAQdxError imaqError;

	const char* name;
	ImageType type;

public:
	RobotCamera(const char* name, ImageType type):
		imaqError(), session()
	{
		frame = NULL;
		this->name = name;
		this->type = type;
	}

	~RobotCamera()
	{
		delete frame;
	}

	void Init()
	{
		frame = imaqCreateImage(type, 0);

		imaqError = IMAQdxOpenCamera(name, IMAQdxCameraControlModeController, &session);
		if(imaqError != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
		}

		imaqError = IMAQdxConfigureGrab(session);
		if(imaqError != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
		}

		IMAQdxStartAcquisition(session);
	}

	Image* Run()
	{
		IMAQdxGrab(session, frame, true, NULL);
		if(imaqError != IMAQdxErrorSuccess)
		{
			DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
		}

		return frame;
	}

	void Release()
	{
		IMAQdxStopAcquisition(session);
	}

};

}
#endif /* ROBOTCAMERA_H_ */
