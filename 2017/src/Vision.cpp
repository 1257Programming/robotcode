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
int getPegLocation(vector<vector<Point> >& contours );
bool isCentered(int pegLocationX, int imageCenterX);
double getMotorVelocity(int pegLocation, int imageWidth);
inline void adjustThreshold(Scalar& threshold);

// Scores a gear from a position close to the peg
void Robot::ScoringSequence()
{
	//Create the function-wide variables for the image and contours
	Mat videoFrame;
	vector<vector<Point> > contours;

	//If the gear isn't centered, run the scoring sequence
	while(!isGearCentered)
	{
		if(ScoringCanceled())
		{
			SmartDashboard::PutString("Scoring Sequence Status", "Scoring sequence cancelled");
			return;
		}

		VisionSink.GrabFrame(videoFrame);
		//Try to find the contours in the image
		try
		{
			findTargets(videoFrame, contours);
			SmartDashboard::PutString("Scoring Sequence Status", "Targets Identified");
		}
		//If findTargets throws an exception, print it to the smartDashboard and exit the function
		catch(char const* error)
		{
			SmartDashboard::PutString("Scoring Sequence Status", error);
			return;
		}

		//Get image constants from the contours
		int imageCenterX = videoFrame.cols / 2;
		int pegLocationX = getPegLocation(contours);
		isGearCentered = isCentered(pegLocationX, imageCenterX);
		SmartDashboard::PutBoolean("Peg Centered", isGearCentered);

		if(!isGearCentered)
		{
			float velocity = getMotorVelocity(pegLocationX, videoFrame.cols);
			//If the slicer is blocked in the direction it's trying to go
			bool slicerCantMove = (GearSlide.IsFwdLimitSwitchClosed() && velocity < 0) || (GearSlide.IsRevLimitSwitchClosed() && velocity > 0);
			if(slicerCantMove)
			{
				GearSlide.Set(0);
				ArcadeDrive(-velocity, velocity);
				SmartDashboard::PutNumber("Bagel Slicer Velocity", 0);
			}
			else
			{
				GearSlide.Set(velocity);
				ArcadeDrive(0, 0);
				SmartDashboard::PutNumber("Bagel Slicer Velocity", velocity);
			}
		}
		else
		{
			GearSlide.Set(0);
			ArcadeDrive(0, 0);
			SmartDashboard::PutString("Scoring Sequence Status", "Gear centered. Moving forward to peg.");
		}
	}
	DriveToPeg();
	SmartDashboard::PutNumber("Bagel Slicer Velocity", 0);
	SmartDashboard::PutString("Scoring Sequence Status", "Bagel slicer in position");
}

bool Robot::ScoringCanceled()
{
	//If the X button is being pressed
	if(Operator.GetRawButton(BUTTON_X))
	{
		//If the X button wasn't previously being pressed
		if(!XPrevState)
		{
			XPrevState = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		XPrevState = false;
		return false;
	}
}

// Takes in an image from the robot's camera, and filters it for green light.
// Also searches for contours in this processed image
void findTargets(Mat& image, vector<vector<Point> >& contours)
{
	// How many more times the function is willing to adjust the thresholds
	int adjustmentsUntilFailure = 4;

	// BGR ranges for pixels we want to turn on
	Scalar minGreen = Scalar(50, 130, 0);
	Scalar maxGreen = Scalar(100, 255, 20);

	// Filter the image for noise
	GaussianBlur(image, image, Size(15, 15), 0);

	while(adjustmentsUntilFailure > 0)
	{
		inRange( image, minGreen, maxGreen, image);

		//Morphological Opening
		//Removes small white patterns in an image
		erode( image, image, getStructuringElement( MORPH_ELLIPSE, Size(3, 3)) );
		dilate( image, image, getStructuringElement( MORPH_ELLIPSE, Size(3, 3)) );

		//Create a 2D array of points to store the various contours
		findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		//If you can't see at least 1 target, adjust the thresholds and decrement the counter
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

//Returns the location of the peg given a list of contours
int getPegLocation(vector<vector<Point> >& contours )
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

// Checks whether the difference between the peg location and the image center is in a range
bool isCentered(int pegLocationX, int imageCenterX)
{
	int imageWidth = 2 * imageCenterX;
	return abs(pegLocationX - imageCenterX) < (imageWidth / 7);
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

// Drive forward until you get to the peg's location
void Robot::DriveToPeg()
{
	RobotTimer.Reset();
	RobotTimer.Start();
	SmartDashboard::PutString("Scoring Sequence Status", "Scoring gear on peg");
	const int pegLength = 10; //Really 10.5", but trivial for coding purposes
	double forwardSpeed = 0.5;
	//If the robot has driven to the peg or it's been driving for 6 seconds
	while(FrontDist.GetRangeInches() > pegLength && !RobotTimer.HasPeriodPassed(6.0))
	{
		SmartDashboard::PutNumber("Distance to Peg", FrontDist.GetRangeInches());
		if(ScoringCanceled())
		{
			SmartDashboard::PutString("Scoring Sequence Status", "Scoring sequence cancelled");
			return;
		}
		else
		{
			ArcadeDrive(forwardSpeed, 0);
		}
	}
	SmartDashboard::PutString("Scoring Sequence Status", "Gear on peg");
	RobotTimer.Stop();
	ArcadeDrive(0, 0);
}

// Decrease the green value of a given BGR threshold by 5
inline void adjustThreshold(Scalar& threshold)
{
	threshold.val[1] -= 5;
}
