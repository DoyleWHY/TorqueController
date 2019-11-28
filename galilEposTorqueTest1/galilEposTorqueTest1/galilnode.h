#ifndef GALILNODE_H
#define GALILNODE_H

#define _CRT_SECURE_NO_WARNINGS //use traditional C calls like sprintf()

#include <iostream>   //std::cout
#include <string>     //to_string, string, etc.
#include <cstdio>     //sprintf, etc.
#include <cstring>    //strlen, etc.
#include <assert.h>   //常用于防御式编程

#include "gclib.h"
#include "gclibo.h"

#define GALIL_EXAMPLE_OK G_NO_ERROR //return code for correct code execution
#define GALIL_EXAMPLE_ERROR -100 //return code for error in example code
#define MAX_AXIS_NUMBER 4

typedef enum{
    GALIL_NOMODE,
    GALIL_POSITION,
    GALIL_JOG,
    GALIL_PT,
    GALIL_GR,
    GALIL_HOMING
}GalilMode;

class GalilNode
{
public:
    GalilNode();
    ~GalilNode();

    bool initMotor(uint axis = 0);
    bool setenablestate(uint axis = 0);
    bool disableMotor(uint axis = 0);
    bool getAnalogInput(uint axis = 0);
    bool getMotorCurrent(uint axis, double &pos);
    bool setMotorJogSpeed(uint axis, long speed);

    bool movePositionAbsolutely(uint axis, long pos);
    bool movePositionRelatively(uint axis, long pos);
    bool movePositionTracking(uint axis, long pos);
    bool moveAllPositionTracking(long *pos);
    bool moveVelocity(uint axis, long v);

    bool getMotorPosition(uint axis, int &pos);

    bool setMotorVelocity(uint axis, long rpm);
    bool definePosition(uint axis, int pos = 0);
    bool printPositon(uint axis);
    bool accuracyTest(float *position);

    double analogValue;


private:
    void x_e(GReturn rc);
    void check(GReturn rc);

    int rc;
    bool galilLinked;         //the status of galil connection
    char buf[G_SMALL_BUFFER]; //command buffer
    const char* axisStrSet;   //the string of axis
    char* trimmed;            //position display

    GCon g;
    GalilMode currentMode[MAX_AXIS_NUMBER];

protected:
    long countsPerRevolution[MAX_AXIS_NUMBER];
    uint velocity[MAX_AXIS_NUMBER];
    uint acceleration[MAX_AXIS_NUMBER];
    uint deceleration[MAX_AXIS_NUMBER];
};

#endif // GALILNODE_H
