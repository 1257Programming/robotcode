#include "WPILib.h"
#include <pthread.h>
#include <ctime>
pthread_mutex_t ex;
extern "C"
{
	typedef struct _data{
			int * count;
			bool * running;
	}timer_data;
	void * countUp(void* args)
	{
		timer_data dat = *(timer_data*)(args);
		bool *prunning = dat.running;
		int *pcount = dat.count;

		while(*prunning)
		{
			pthread_mutex_lock(&ex);
			(*pcount)++;
			pthread_mutex_unlock(&ex);
		}
		return NULL;
	}
}
class CycleCounter
{
	pthread_t counter = 0;
	int * count = new int;
	bool* running = new bool;
public:
	CycleCounter()
	{
		*count = 0;
		*running = false;

	}
	void Start()
	{
		pthread_mutex_init(&ex, NULL);
		timer_data * dat = new timer_data;
		dat->count =count;
		dat->running = running;
		*running = true;
		*count = 0;
		pthread_create(&counter, NULL, countUp, dat);
	}
	void Stop()
	{
		*running = false;
		pthread_mutex_destroy(&ex);
	}
	int Get()
	{
		return *count;
	}
	void Reset()
	{
		*count = 0;
	}
};
class TestBot : public IterativeRobot
{
public:
	Talon Left;
	Talon Right;
	RobotDrive Drive;
	Joystick Stick;

	double leftspeed = 0;
	double rightspeed = 0;
	double speed = 0;
	double curve = 0;
	BuiltInAccelerometer bia;

	TestBot(): Left(0), Right(1), Drive(Left, Right), Stick(0), bia(Accelerometer::kRange_8G)
	{
		Drive.SetExpiration(.1);
	}

	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestInit();
	void TestPeriodic();
	double accel(Joystick& stick, int axis, double& current, double sf);

	double dabs(double x)
	{
		if(x >= 0)
			return x;
		else
			return -x;
	}
};
