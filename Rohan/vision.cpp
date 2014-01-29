#include "vision.h"

void Vision::aim()
{
	AxisCamera &camera = AxisCamera::GetInstance("10.12.57.11");
	ColorImage *image;
	
	camera.GetImage(image);
	image->Write("/test.jpg");
}
