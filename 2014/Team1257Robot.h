#include <WPILib.h>

class TestBot : public SimpleRobot
{
public:
	Victor Left;
	Victor Right;
	RobotDrive Tank;
	Joystick Stick1;
	Joystick Stick2;
	DriverStationLCD* Lcd;
	
	TestBot(): Left(1), Right(2), Tank(Left, Right), Stick1(1), Stick2(2)
	{
		Tank.SetExpiration(.1);
		Lcd = DriverStationLCD::GetInstance();
	}
	
	void Autonomous(); // Just drive forward. If possible, avoid hitting the player station barrier.
	void OperatorControl(); //For now, get driving working, but be prepared for other systems to work.
	inline void drive(float left, float right); // Simple convenience function for tank drive and motor value reversal.
	inline void print(char* message, int line);
};

