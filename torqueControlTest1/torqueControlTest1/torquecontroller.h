#ifndef TORQUECONTROLLER_H
#define TORQUECONTROLLER_H

#include <iostream>

using namespace std;

class torquecontroller
{
public:
  torquecontroller();
  short int pidcontrol(short int current);

  short int controltargetcurrent;
  float Bias, Integral_bias, Last_Bias, Pwm;

private:
  float torqueKp;
  float torqueKi;
  float torqueKd;
};

#endif // TORQUECONTROLLER_H

/************************ (C) COPYRIGHT ROBO Medical ***********END OF FILE****/
