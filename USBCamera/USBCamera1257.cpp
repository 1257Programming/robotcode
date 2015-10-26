/*
 * USBCamera1257.cpp
 *
 *  Created on: Oct 26, 2015
 *      Author: Nathan
 */

#include <USBCamera1257.h>

USBCamera1257::USBCamera1257(const char* name) {
	imaqError = IMAQdxOpenCamera(name, IMAQdxCameraControlModeController, &session);
	if(imaqError != IMAQdxErrorSuccess) {
			return;
	}
	imaqError = IMAQdxConfigureGrab(session);
	if(imaqError != IMAQdxErrorSuccess) {
			return;
	}
}
void USBCamera1257::begin()
{
	IMAQdxStartAcquisition(session);
}
void USBCamera1257::end()
{
	IMAQdxStopAcquisition(session);
}
IMAQdxError USBCamera1257::getLastError()
{
	return imaqError;
}
IMAQdxError USBCamera1257::grab(Image* img)
{
	imaqError = IMAQdxGrab(session, img, true, NULL);
	return imaqError;
}
USBCamera1257::~USBCamera1257() {}

