#include <iostream>
#include <string>
#include <vector>
#include "Robot.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

void findTargets(Mat& rawImage, vector<vector<Point> >& contours);
bool isGearCentered(int pegLocationX, int imageCenterX);
int getPegLocation(vector<vector<Point> > contours );
bool isGearCentered(int pegLocationX, int imageCenterX);
double getMotorVelocity(int pegLocation, int imageWidth);
void adjustThreshold(Scalar& threshold);

void Robot::ScoringSequence()
{
	// TODO: set camera exposure really really really low
	// put in robotInit?
	//CameraServer.getInstance().startAutomaticCapture();
	//CvSink cvSink = CameraServer.getInstance().getVideo();
	//UsbCamera usbCamera = new UsbCamera("USB Camera 0", 0);

	bool isCentered = false;
	vector<vector<Point> > contours;
	Mat rawImage;
	while(!isCentered)
	{
		  VisionSink.GrabFrame(rawImage);
		  int attempts = 0;
		  bool exceptionThrown = true;
		  while(attempts < 4 && exceptionThrown)
		  {
			  exceptionThrown = false;
			  try
			  {
				   findTargets(rawImage, contours);
			  }
			  catch(char const* error)
			  {
				  SmartDashboard::PutString("Scoring Sequence Info", error);
				  exceptionThrown = true;
			  }
			  attempts++;
		  }
		  if(exceptionThrown)
		  {
			  SmartDashboard::PutString("Scoring Sequence Info", "Robot not in range of targets");
			  return; //TODO: MAYBE BAD IDEA
		  } else {
			  SmartDashboard::PutString("Scoring Sequence Info", "Targets Identified");
		  }

		  int imageCenterX = rawImage.cols / 2;
		  int pegLocationX = getPegLocation(contours);
		  bool isCentered = isGearCentered(pegLocationX, imageCenterX);

		  SmartDashboard::PutNumber("Peg Location", pegLocationX);
		  SmartDashboard::PutBoolean("Peg Centered", isCentered);
		  if(!isCentered)
		  {
			  float velocity = getMotorVelocity(pegLocationX, rawImage.cols);
			  SmartDashboard::PutNumber("Bagel Slicer Velocity", velocity);
			  GearSlide.Set(velocity);
		  }
		  else
		  {
			  GearSlide.Set(0);
		  }
	}
	SmartDashboard::PutNumber("Bagel Slicer Velocity", 0);
	SmartDashboard::PutString("ScoringSequence Info", "Bagel slicer in position");
}


// Takes in an image of the field, and filters it for green light.
// Also modifies contours based on this processed image
void findTargets(Mat& image, vector<vector<Point> >& contours)
{
	// How many more times the function is willing to adjust the thresholds
	int adjustmentsUntilFailure = 4;

	// BGR ranges for pixels we want to turn on
	Scalar minGreen = Scalar(50, 130, 0);
	Scalar maxGreen = Scalar(100, 255, 20);

	// Filter the image for noise
	GaussianBlur(image, image, Size(15, 15), 0); //TODO: test if needed

	while(adjustmentsUntilFailure > 0)
	{
		inRange( image, minGreen, maxGreen, image);

		//Morphological Opening
		//Removes small white patterns in an image
		erode( image, image, getStructuringElement( MORPH_ELLIPSE, Size(3, 3)) );
		dilate( image, image, getStructuringElement( MORPH_ELLIPSE, Size(3, 3)) );

		//Create a 2D array of points to store the various contours
		findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		//If you can't see at least 2 contours, adjust the thresholds and decrement the counter
		if(contours.size() < 1)
		{
			adjustThreshold(minGreen);
			--adjustmentsUntilFailure;
		}
		else
		{
			return;
		}
	}
	// If targets are not found throw an error
	throw "No targets were found";
}

// Checks whether the difference between the peg location and the image center is in a range
inline bool isGearCentered(int pegLocationX, int imageCenterX)
{
	return abs(pegLocationX - imageCenterX) < 50;
}

int getPegLocation(vector<vector<Point> > contours )
{
	int leftmostX = 3000;
	int rightmostX = 0;

	for(unsigned int i = 0; i < contours.size(); i++)
	{
		for(unsigned int j = 0; j < contours[i].size(); j++)
		{
			int currentPointX = contours[i][j].x;

			if(currentPointX < leftmostX)
			{
			  leftmostX  = currentPointX;
			}
			else if (currentPointX > rightmostX)
			{
			  rightmostX = currentPointX;
			}
		}
	}
	return (leftmostX + rightmostX) / 2;
}

double getMotorVelocity(int pegLocation, int imageWidth)
{
	double leftTolerance = .3;  // 30%
	double rightTolerance = .7; // 70%
	double maxSpeed =.4;

	//If  pegLocation is greater than 70% of imageWidth, return the maxSpeed going right
	if (pegLocation > (rightTolerance * imageWidth) )
	{
			return maxSpeed;
	}
	//If  pegLocation is less than 30% of imageWidth, return the maxSpeed going left
	else if(pegLocation < (leftTolerance * imageWidth) )
	{
			return -maxSpeed;
	}
	else
	{
		//linearly from maxSpeed at leftTolerance to -maxSpeed at rightTolerance
		// m = 2maxSpeed / (rightTolerance - leftTolerance) = 5maxSpeed
		// vel = 5maxSpeed * (pegLocation / imageWidth - 0.5)
		return   maxSpeed * (static_cast<float>(pegLocation) / static_cast<float>(imageWidth) - 0.5) / (.5 - leftTolerance);
	}
}

// Decrease the green value of a given BGR threshold by 5
inline void adjustThreshold(Scalar& threshold)
{
	threshold.val[1] -= 5;
}

void Robot::DriveToPeg()
{
	const int pegLength = 10; //Really 10.5", but trivial for coding purposes
	if(FrontDist.GetRangeInches() > pegLength)
	{
		DriveTrain.ArcadeDrive(0.85, 0);
	}
	else
	{
		DriveTrain.SetLeftRightMotorOutputs(0, 0);
	}
}
}
