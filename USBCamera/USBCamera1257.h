/*
 * USBCamera1257.h
 *
 *  Created on: Oct 26, 2015
 *      Author: Nathan
 */

#ifndef SRC_USBCAMERA1257_H_
#define SRC_USBCAMERA1257_H_
#include <WPILib.h>
class USBCamera1257 {
	IMAQdxSession session;
	IMAQdxError imaqError;
public:
	USBCamera1257(const char* name);
	void begin();
	void end();
	IMAQdxError grab(Image* img);
	IMAQdxError getLastError();
	virtual ~USBCamera1257();
};

#endif /* SRC_USBCAMERA1257_H_ */
