#ifndef TORQUECONTROLLER_H
#define TORQUECONTROLLER_H

#include <iostream>

using namespace std;

class torquecontroller
{
public:
  torquecontroller();
  long pidcontrol(double current);
  long impedancecontrol(double current);

  double controltargetcurrent, impedanceFactor;
  float Bias, Integral_bias, Last_Bias;
  long Pwm;

private:
  float torqueKp;
  float torqueKi;
  float torqueKd;
};

#endif // TORQUECONTROLLER_H

/************************ (C) COPYRIGHT ROBO Medical ***********END OF FILE****/
