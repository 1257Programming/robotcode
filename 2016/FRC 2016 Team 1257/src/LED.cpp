#include "Robot.h"

void Robot::LEDRaveMode()
{
  pwmOne.SetRaw(0);
  pwmTwo.SetRaw(0);
  pwmThree.SetRaw(0);
  pwmFour.SetRaw(0);
}

void Robot::LEDIntakeOutBlue()
{
  pwmOne.SetRaw(0);
  pwmTwo.SetRaw(0);
  pwmThree.SetRaw(0);
  pwmFour.SetRaw(1);
}

void Robot::LEDIntakeOutRed()
{
  pwmOne.SetRaw(1);
  pwmTwo.SetRaw(0);
  pwmThree.SetRaw(0);
  pwmFour.SetRaw(1);
}

void Robot::LEDIntakeInBlue()
{
  pwmOne.SetRaw(0);
  pwmTwo.SetRaw(0);
  pwmThree.SetRaw(1);
  pwmFour.SetRaw(0);
}

void Robot::LEDIntakeInRed()
{
  pwmOne.SetRaw(1);
  pwmTwo.SetRaw(0);
  pwmThree.SetRaw(1);
  pwmFour.SetRaw(0);
}

void Robot::LEDTeleopWithBallBlue()
{
  pwmOne.SetRaw(0);
  pwmTwo.SetRaw(0);
  pwmThree.SetRaw(1);
  pwmFour.SetRaw(1);
}

void Robot::LEDTeleopWithBallRed()
{
  pwmOne.SetRaw(1);
  pwmTwo.SetRaw(0);
  pwmThree.SetRaw(1);
  pwmFour.SetRaw(1);
}

void Robot::LEDTeleopIdleBlue()
{
  pwmOne.SetRaw(0);
  pwmTwo.SetRaw(1);
  pwmThree.SetRaw(0);
  pwmFour.SetRaw(0);
}

void Robot::LEDTeleopIdleRed()
{
  pwmOne.SetRaw(1);
  pwmTwo.SetRaw(1);
  pwmThree.SetRaw(0);
  pwmFour.SetRaw(0);
}

void Robot::LEDAutonomousIdle()
{
  pwmOne.SetRaw(1);
  pwmTwo.SetRaw(1);
  pwmThree.SetRaw(0);
  pwmFour.SetRaw(1);
}

