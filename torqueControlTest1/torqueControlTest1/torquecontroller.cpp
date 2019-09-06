#include "torquecontroller.h"

torquecontroller::torquecontroller()
{
    Bias = 0;
    Integral_bias = 0;
    Last_Bias = 0;
    Pwm = 0;
    torqueKp = 1;
    torqueKi = 0.0;
    torqueKd = 2;
}

short int torquecontroller::pidcontrol(short int current)
{
    Bias = current - controltargetcurrent;
    Integral_bias += Bias;
    Pwm = Pwm + (torqueKp*Bias + torqueKi*Integral_bias + torqueKd*(Bias-Last_Bias));

    cout << "Bias:" << Bias << endl;
    cout << "Integral_Bias:" << Integral_bias << endl;
    cout << "Delta_Bias:" << Bias-Last_Bias << endl;

    Last_Bias=Bias;
    return Pwm;
}
