#include "logger_robo.h"
#include "eposcontroller.h"
#include <assert.h>

EposController::EposController()
    : g_pKeyHandle(NULL)
{
    for(size_t i = 0; i < MAX_AXIS_NUMBER; ++i){
        currentMode[i] = NOMODE;
        enabled[i] = false;
        countsPerRevolution[i] = 10855;
        velocity[i] = 100;
        acceleration[i] = 3000000;
        deceleration[i] = 3000000;
    }
    analogInputNb = 1;
}

EposController::~EposController()
{
    if(g_pKeyHandle != NULL) disableMotor(0);
}

bool EposController::initMotor(ushort axis)
{
    LOGFUNC
    uint p_pErrorCode = 0;
    uint g_baudrate = 1000000;
    uint lBaudrate = 0;
    uint lTimeout = 500;

    g_pKeyHandle = VCS_OpenDevice("EPOS2", "MAXON SERIAL V2", "USB", "USB0", &p_pErrorCode);
    cout << g_pKeyHandle << endl;
    if(g_pKeyHandle != NULL && p_pErrorCode == 0)
    {
        if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, &p_pErrorCode) == 0)
            return false;
        if(VCS_SetProtocolStackSettings(g_pKeyHandle, g_baudrate, lTimeout, &p_pErrorCode) == 0)
            return false;
        if(VCS_GetProtocolStackSettings(g_pKeyHandle, &lBaudrate, &lTimeout, &p_pErrorCode) == 0)
            return false;
        if(g_baudrate == (uint)lBaudrate)
        {
            for(int i = 0; i < MAX_AXIS_NUMBER; ++i)
                enabled[i] = true;
            return true;
        }
    }
    return false;
}

bool EposController::disableMotor(ushort axis)
{
    LOGFUNC
    assert(axis <= MAX_AXIS_NUMBER);
    uint p_pErrorCode = 0;
    bool res = false;
    if(axis == 0)
        res = VCS_CloseDevice(g_pKeyHandle, &p_pErrorCode);
    else
        res = VCS_SetDisableState(g_pKeyHandle, axis, &p_pErrorCode);

    return res;
}

bool EposController::setenablestate(ushort axis)
{
    LOGFUNC
    assert(axis <= MAX_AXIS_NUMBER);
    uint p_pErrorCode = 0;
    bool res = false;
    res = VCS_SetEnableState(g_pKeyHandle, axis, &p_pErrorCode);
    return res;
}

bool EposController::setdisablestate(ushort axis)
{
    LOGFUNC
    assert(axis <= MAX_AXIS_NUMBER);
    uint p_pErrorCode = 0;
    bool res = false;
    res = VCS_SetDisableState(g_pKeyHandle, axis, &p_pErrorCode);
    return res;
}

bool EposController::analogConfig(ushort axis)
{
    LOGFUNC
    uint p_pErrorCode = 0;
    bool res = false;
    res = VCS_AnalogInputConfiguration(g_pKeyHandle, axis, analogInputNb, 15, 1, &p_pErrorCode);
    return res;
}

bool EposController::getAnalogInput(ushort axis)
{
    LOGFUNC
    uint p_pErrorCode = 0;
    bool res = false;
    res = VCS_GetAnalogInput(g_pKeyHandle, axis, analogInputNb, &analogValue, &p_pErrorCode);
    return res;
}

bool EposController::currentConfig(ushort axis)
{
    LOGFUNC
    uint p_pErrorCode = 0;
    bool res = false;
    res = VCS_ActivateCurrentMode(g_pKeyHandle, axis, &p_pErrorCode);
    return res;
}

bool EposController::getMotorCurrent(ushort axis, short int* pos)
{
    LOGFUNC
    uint p_pErrorCode = 0;
    bool res = false;
    res = VCS_GetCurrentIs(g_pKeyHandle, axis, pos, &p_pErrorCode);
    return res;
}

bool EposController::setMotorCurrent(ushort axis, short int pos)
{
    LOGFUNC
    uint p_pErrorCode = 0;
    bool res = false;
    res = VCS_SetCurrentMust(g_pKeyHandle, axis, pos, &p_pErrorCode);
    return res;
}

bool EposController::movePositionAbsolutely(ushort axis, long pos)
{
    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    uint p_pErrorCode = 0;

    if(currentMode[axis-1] != PROFILE_POSITION)
    {
        if(VCS_ActivateProfilePositionMode(g_pKeyHandle, axis, &p_pErrorCode) == 0)
        {
            qDebug("Step1");
            std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
            return false;
        }
        if(VCS_SetPositionProfile(g_pKeyHandle, axis,
                                  velocity[axis-1], acceleration[axis-1], deceleration[axis-1], &p_pErrorCode) == 0)
        {
            qDebug("Step2");
            std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
            return false;
        }
        if(VCS_SetEnableState(g_pKeyHandle, axis, &p_pErrorCode) == 0)
        {
            qDebug("Step3");
            std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
            return false;
        }
        currentMode[axis-1] = PROFILE_POSITION;
    }
    if(VCS_MoveToPosition(g_pKeyHandle, axis, pos, 1, 1, &p_pErrorCode) == 0)
    {
        qDebug("Step4");
        std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
        return false;
    }
    else return true;
}

bool EposController::movePositionRelatively(ushort axis, long pos)
{
    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    uint p_pErrorCode = 0;

    if(currentMode[axis-1] != PROFILE_POSITION)
    {
        if(VCS_ActivateProfilePositionMode(g_pKeyHandle, axis, &p_pErrorCode) == 0)
        {
            qDebug("Step1");
            std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
            return false;
        }
        if(VCS_SetPositionProfile(g_pKeyHandle, axis,
                                  velocity[axis-1], acceleration[axis-1], deceleration[axis-1], &p_pErrorCode) == 0)
        {
            qDebug("Step2");
            std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
            return false;
        }
        if(VCS_SetEnableState(g_pKeyHandle, axis, &p_pErrorCode) == 0)
        {
            qDebug("Step3");
            std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
            return false;
        }
        currentMode[axis-1] = PROFILE_POSITION;
    }
    if(VCS_MoveToPosition(g_pKeyHandle, axis, pos, 0, 1, &p_pErrorCode) == 0)
    {
        qDebug("Step4");
        std::cout << "Error code is 0x" << std::hex << p_pErrorCode << std::endl;
        return false;
    }
    else return true;
}

bool EposController::movePositionTracking(ushort axis, long pos)
{

}

bool EposController::moveVelocity(ushort axis, long v)
{
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    uint p_pErrorCode = 0;

    if(currentMode[axis-1] != PROFILE_VELOCITY)
    {
        if(VCS_ActivateProfileVelocityMode(g_pKeyHandle, axis, &p_pErrorCode) == 0)
            return false;
        if(VCS_SetVelocityProfile(g_pKeyHandle, axis, acceleration[axis-1], deceleration[axis-1], &p_pErrorCode) == 0)
            return false;
        if(VCS_SetEnableState(g_pKeyHandle, axis, &p_pErrorCode) == 0)
            return false;
        currentMode[axis-1] = PROFILE_VELOCITY;
    }
    if(VCS_MoveWithVelocity(g_pKeyHandle, axis, v, &p_pErrorCode) == 0)
        return false;
    else return true;
}

bool EposController::moveSinusoidal(ushort axis, double amplitude_angle, double frequency, double time)
{

}

bool EposController::getMotorPosition(ushort axis, int &pos)
{
    LOGFUNC
    if((axis > MAX_AXIS_NUMBER) || (axis < 1))
    {
        pos = -1;
        return false;
    }
    uint p_pErrorCode = 0;
    return VCS_GetPositionIs(g_pKeyHandle, axis, &pos, &p_pErrorCode);
}

bool EposController::getMotorLockedPosition(ushort axis, int &pos)
{

}

bool EposController::setMotorVelocity(ushort axis, long rpm)
{
    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    velocity[axis-1] = rpm;
}

bool EposController::definePosition(ushort axis, int pos)
{
    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    bool res = false;
    uint p_pErrorCode = 0;
    if(pos == 0)
    {
        VCS_ActivateHomingMode(g_pKeyHandle, axis, &p_pErrorCode);
        currentMode[axis-1] = HOMING;
    }
    res = VCS_DefinePosition(g_pKeyHandle, axis, pos, &p_pErrorCode);
    std::cout << res << " Error code is 0x" << std::hex << p_pErrorCode << std::endl;
    return res;
}

bool EposController::clearMotorFault(ushort axis)
{
    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    uint p_pErrorCode = 0;
    int faultState = 0;

    if(VCS_GetFaultState(g_pKeyHandle, axis, &faultState, &p_pErrorCode))
    {
        if(faultState)
        {
            unsigned char nbOfDeviceError;
            if(VCS_GetNbOfDeviceError(g_pKeyHandle, axis, &nbOfDeviceError, &p_pErrorCode) == 0)
                return false;
            if(VCS_ClearFault(g_pKeyHandle, axis, &p_pErrorCode) == 0)
                return false;
            else return true;
        }
    }
    else return false;
}
