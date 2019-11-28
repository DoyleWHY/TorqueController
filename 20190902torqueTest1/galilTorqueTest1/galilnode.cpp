#include "galilnode.h"

using namespace std;

GalilNode::GalilNode()
    : rc(GALIL_EXAMPLE_OK), galilLinked(false), axisStrSet("ABCD"), g(NULL)
{
    for(size_t i = 0; i < MAX_AXIS_NUMBER; ++i)
    {
        currentMode[i] = GALIL_NOMODE;
        velocity[i] = 10000;
        acceleration[i] = 8000000;   //max 1000 0000
        deceleration[i] = 8000000;   //initialize dec and acc
    }
    countsPerRevolution[0] = 10854;
    countsPerRevolution[1] = 10854;
    countsPerRevolution[2] = 10854;
    countsPerRevolution[3] = 68608;

}

GalilNode::~GalilNode()
{
    GClose(g);
}

bool GalilNode::initMotor(uint axis)
{
//    LOGFUNC
    assert(axis <= MAX_AXIS_NUMBER);
    if(!galilLinked)
    {
        try
        {
            cout << "Step1 : Initialize DMC4143 controller\n";
            cout << "  1.1 : connect to DMC4143 controller\n";
            x_e(GVersion(buf, sizeof(buf))); //library version
            cout << "Library versions: " << buf << "\n";

            cout << "Connecting to hardware\n";
            x_e(GOpen("/dev/ttyUSB0 --baud 115200 --subscribe ALL", &g));
            x_e(GInfo(g, buf, sizeof(buf))); //grab connection string
            cout << buf << '\n';
            cout << "*********************************" << endl;

            x_e(GCmd(g, "ST"));  //Stop the previous running DMCcode
            galilLinked = true;
            cout << "  1.2 : Auto detection\n";
            x_e(GCmd(g, "XQ")); //Program execute.
            cout << "*********************************" << endl;

        }//try
        catch (GReturn gr) //for x_e() function
        {
            if (gr == GALIL_EXAMPLE_ERROR)
                cout << "ERROR: Example code failed\n";
            else
            {
                cout << "Function returned " << gr << '\n';
                GError(gr, buf, sizeof(buf));
                cout << buf << '\n';
                GSize size = sizeof(buf);

                if (g)
                {
                    GUtility(g, G_UTIL_ERROR_CONTEXT, buf, &size);
                    cout << buf << '\n'; //further context
                }

            }
            rc = GALIL_EXAMPLE_ERROR;
            if (g) GClose(g); g = 0; //close g
        }
        catch (std::exception& e)
        {
            std::cerr << "Unexpected std::exception... Kaboom. " << e.what() << std::endl;
            rc = GALIL_EXAMPLE_ERROR;
            if (g) GClose(g); g = 0; //close g
        }
        catch (...)
        {
            cout << "Unexpected error... Kaboom." << endl;
            rc = GALIL_EXAMPLE_ERROR;
            if (g) GClose(g); g = 0; //close g
        }
    }
}

bool GalilNode::setenablestate(uint axis)
{
    assert(axis <= MAX_AXIS_NUMBER);
    sprintf(buf, "XQ");
    x_e(GCmd(g, buf));
    cout << "Motion start!" << endl;
    return true;
}

bool GalilNode::disableMotor(uint axis)
{
//    LOGFUNC
    assert(axis <= MAX_AXIS_NUMBER);
    sprintf(buf, "ST;MO");
    x_e(GCmd(g, buf));
    cout << "Motion stop!" << endl;
    return true;
}

bool GalilNode::getAnalogInput(uint axis)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    sprintf(buf, "Torque={F1.5}");
    x_e(GCmdD(g, buf, &analogValue));
    return true;
}

bool GalilNode::getMotorCurrent(uint axis, double &pos)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    sprintf(buf, "TT%c", axisStrSet[axis-1]);
    x_e(GCmdD(g, buf, &pos));
    return true;
}

bool GalilNode::setMotorJogSpeed(uint axis, long speed)
{
    assert(axis <= MAX_AXIS_NUMBER);
    sprintf(buf, "Jspeed=%ld", speed);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::movePositionAbsolutely(uint axis, long pos)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    currentMode[axis-1] = GALIL_POSITION;
    sprintf(buf, "PA%c=%ld;BG%c", axisStrSet[axis-1], pos, axisStrSet[axis-1]);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::movePositionRelatively(uint axis, long pos)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    currentMode[axis-1] = GALIL_POSITION;
    sprintf(buf, "PR%c=%ld;BG%c", axisStrSet[axis-1], pos, axisStrSet[axis-1]);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::movePositionTracking(uint axis, long pos)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    if(currentMode[axis-1] != GALIL_PT)
    {
        sprintf(buf, "IT%c=0.3;PT%c=1", axisStrSet[axis-1], axisStrSet[axis-1]); //Need modified
        x_e(GCmd(g, buf));
        currentMode[axis-1] = GALIL_PT;
    }
    sprintf(buf, "PA%c=%ld", axisStrSet[axis-1], pos);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::moveAllPositionTracking(long *pos)
{
    sprintf(buf, "PA %ld,%ld,,%ld", pos[0], pos[1], pos[2]);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::moveVelocity(uint axis, long v)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    currentMode[axis-1] = GALIL_JOG;
    sprintf(buf, "JG%c=%ld;BG%c", axisStrSet[axis-1], v, axisStrSet[axis-1]);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::getMotorPosition(uint axis, int &pos)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    sprintf(buf, "TP%c", axisStrSet[axis-1]);
    x_e(GCmdI(g, buf, &pos));
    return true;
}

bool GalilNode::setMotorVelocity(uint axis, long rpm)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    long countsPerSecond = static_cast<long>(countsPerRevolution[axis-1]*(rpm));
    sprintf(buf, "SP%c=%ld", axisStrSet[axis-1], countsPerSecond);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::accuracyTest(float *position)
{
    sprintf(buf, "PEnd[0]=%f;PEnd[1]=%f;PEnd[2]=%f", position[0],position[1],position[2]);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::definePosition(uint axis, int pos)
{
//    LOGFUNC
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    sprintf(buf, "DP%c=%d", axisStrSet[axis-1], pos);
    x_e(GCmd(g, buf));
    return true;
}

bool GalilNode::printPositon(uint axis)
{
    assert((axis <= MAX_AXIS_NUMBER) && (axis >= 1));
    x_e(GCmdT(g, "RP", buf, sizeof(buf), &trimmed));
    cout << "Position: " << trimmed << '\n';
}


void GalilNode::x_e(GReturn rc)
{
        if (rc != G_NO_ERROR)
                throw rc;
}

void GalilNode::check(GReturn rc)
{
        if (rc != G_NO_ERROR)
        {
                printf("ERROR: %d", rc);
                if (g)
                        GClose(g);
                exit (rc);
        }
}
