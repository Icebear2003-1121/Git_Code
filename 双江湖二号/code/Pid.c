#include "pid.h"

struct FPID SteerPid;
struct FPID GptPidR;
struct FPID GptPidL;
void Pid_Init(void)
{
        SteerPid.err=0;
        SteerPid.err_last=0;
        SteerPid.Kp=38;
        SteerPid.Ki=0;
        SteerPid.Kd=27;
        SteerPid.Kout=0;
        SteerPid.voltage=0;
        SteerPid.integral=0;

        GptPidR.err=0;
        GptPidR.err_last=0;
        GptPidR.err_last_last=0;
        GptPidR.Kp=30;
        GptPidR.Ki=7;
        GptPidR.Kd=0;
        GptPidR.Kout=0;
        GptPidR.voltage=0;
        GptPidR.integral=0;

        GptPidL.err=0;
        GptPidL.err_last=0;
        GptPidL.err_last_last=0;
        GptPidL.Kp=30;
        GptPidL.Ki=7;
        GptPidL.Kd=0;
        GptPidL.Kout=0;
        GptPidL.voltage=0;
        GptPidL.integral=0;

}

int calculate_pid(float a)
{
    int t;

    SteerPid.Set      = 0;
    SteerPid.Actual   = a;
    SteerPid.err      = SteerPid.Set - SteerPid.Actual;


    SteerPid.integral = SteerPid.integral + SteerPid.err;

    SteerPid.voltage = SteerPid.Kp*SteerPid.err
                + SteerPid.Ki*SteerPid.integral/2000
                + SteerPid.Kd*(SteerPid.err-SteerPid.err_last)
                + SteerPid.Kout;

    SteerPid.err_last = SteerPid.err;

    t=613+SteerPid.voltage;

    if(t>680)
    {
        t = 680;
    }
    else if(t<540)
    {
        t = 540;
    }

    return t;
}

int motorR_pid(int a,int b)
{
    int t;
    GptPidR.Set      = a;
    GptPidR.Actual   = b;
    GptPidR.err      = GptPidR.Set - GptPidR.Actual;

    GptPidR.integral = GptPidR.integral + GptPidR.err;

    GptPidR.voltage = GptPidR.Kp*(GptPidR.err-GptPidR.err_last)
                + GptPidR.Ki*GptPidR.err
                + GptPidR.Kd*(GptPidR.err-2*GptPidR.err_last+GptPidR.err_last_last)
                + GptPidR.Kout
                + GptPidR.voltage;

    GptPidR.err_last_last = GptPidR.err_last;
    GptPidR.err_last = GptPidR.err;
//    t=GptPidR.voltage;


    if(GptPidR.voltage>7000)
    {
        GptPidR.voltage=7000;
    }
    else if(GptPidR.voltage<-7000)
    {
        GptPidR.voltage=-7000;
    }

    if(GptPidR.voltage>0)
    {
        gpio_set_level(MOTOR1_DIR, 1);
        t=GptPidR.voltage;
    }
    else
    {
        gpio_set_level(MOTOR1_DIR, 0);
       t=-GptPidR.voltage;
    }

    return t;
}

int motorL_pid(int a,int b)
{
    int t;

    GptPidL.Set      = a;
    GptPidL.Actual   = b;
    GptPidL.err      = GptPidL.Set - GptPidL.Actual;

    GptPidL.integral = GptPidL.integral + GptPidL.err;

    GptPidL.voltage = GptPidL.Kp*(GptPidL.err-GptPidL.err_last)
                + GptPidL.Ki*GptPidL.err
                + GptPidL.Kd*(GptPidL.err-2*GptPidL.err_last+GptPidL.err_last_last)
                + GptPidL.Kout
                + GptPidL.voltage;

    GptPidL.err_last_last = GptPidL.err_last;
    GptPidL.err_last = GptPidL.err;
//    t=GptPidL.voltage;
    if(GptPidL.voltage>7000)
    {
        GptPidL.voltage=7000;
    }
    else if(GptPidL.voltage<-7000)
    {
        GptPidL.voltage=-7000;
    }

    if(GptPidL.voltage>0)
    {
        gpio_set_level(MOTOR2_DIR, 0);
        t=GptPidL.voltage;
    }
    else
    {
        gpio_set_level(MOTOR2_DIR, 1);
       t=-GptPidL.voltage;
    }
    return t;
}





