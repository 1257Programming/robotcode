/*
 * RobotCamera.h
 *
 *  Created on: Feb 4, 2015
 *      Author: Rohan
 */

#ifndef ROBOTCAMERA_H_
#define ROBOTCAMERA_H_

//#define NULL 0

#include "WPILib.h"

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
	RobotCamera(const char* name, ImageType type);
	~RobotCamera();
	void Init();
	Image* Run();
	void Release();
};

}
#endif /* ROBOTCAMERA_H_ */
