#include "WPILib.h"
#include "math.h"
#include "vision.h"
#include "string.h"

#define LEFT_STICK 1

typedef DriverStationLCD dLcd; //cant type that massive class!

float sf = 0.5;

class Robot : public SimpleRobot
{
public:
        Robot();
        void Autonomous();
        void OperatorControl();
        inline void drive(double x, double y);
        void drive();
        void arcadeDrive();
        void print(char* msg, int i);
        void arm();
        void accel(double x, double y);
public:
        DriverStationLCD* lcd;
        RobotDrive rd;
        Joystick stick;
        Gyro angle;
        Vision vis;
        Timer time;
        Relay relay;
        double speed;
};
