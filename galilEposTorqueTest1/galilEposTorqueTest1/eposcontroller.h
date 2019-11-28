#ifndef EPOSCONTROLLER_H
#define EPOSCONTROLLER_H

#include "Definitions.h"
#include <QtDebug>
#include <iostream>

#define MAX_AXIS_NUMBER 1

using namespace std;

typedef enum{
    NOMODE,
    HOMING,
    PROFILE_POSITION,
    POSITION,
    INTERPOLATED_POSITION,
    PROFILE_VELOCITY,
    VELOCITY,
    CURRENT,
    MASTER_ENCODER,
    STEP_DIRECTION
}EposMode;

class EposController
{
public:
    EposController();
    ~EposController();

    bool initMotor(ushort axis = 0);
    bool disableMotor(ushort axis = 0);
    bool setenablestate(ushort axis = 0);
    bool setdisablestate(ushort axis = 0);
    bool analogConfig(ushort axis = 0);
    bool getAnalogInput(ushort axis = 0);
    bool currentConfig(ushort axis = 0);
    bool getMotorCurrent(ushort axis, short int* pos);
    bool setMotorCurrent(ushort axis, short int pos);
    bool movePositionAbsolutely(ushort axis, long pos);
    bool movePositionRelatively(ushort axis, long pos);
    bool movePositionTracking(ushort axis, long pos);
    bool moveVelocity(ushort axis, long v);
    bool moveSinusoidal(ushort axis, double amplitude_angle, double frequency, double time = 15);
    bool getMotorPosition(ushort axis, int &pos);
    bool getMotorLockedPosition(ushort axis, int &pos);
    bool setMotorVelocity(ushort axis, long rpm);
    bool definePosition(ushort axis, int pos = 0);

    bool clearMotorFault(ushort axis);

    ushort analogValue;

private:
    void* g_pKeyHandle;
    bool enabled[MAX_AXIS_NUMBER];
    EposMode currentMode[MAX_AXIS_NUMBER];
    long countsPerRevolution[MAX_AXIS_NUMBER];
    uint velocity[MAX_AXIS_NUMBER];
    uint acceleration[MAX_AXIS_NUMBER];
    uint deceleration[MAX_AXIS_NUMBER];

    ushort analogInputNb;

};

#endif // EPOSCONTROLLER_H

/************************ (C) COPYRIGHT ROBO Medical ***********END OF FILE****/
