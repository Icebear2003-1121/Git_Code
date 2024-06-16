
#ifndef _pid_h
#define _pid_h

#include "zf_common_headfile.h"
//#include "motor.h"
//#include "control.h"
extern struct FPID SteerPid;
extern struct FPID GptPidR;
extern struct FPID GptPidL;

void Pid_Init(void);

int calculate_pid(float a);
int motorR_pid(int a,int b);
int motorL_pid(int a,int b);

struct FPID{
        float Set;             //设定值
        float Actual;          //实际值
        float err;             //当前误差
        float err_last;        //上一次误差
        float err_last_last;   //上上一次误差
        float last_derivative; //上次误差与上上次误差之差

        float Kp,Ki,Kd,Kout;
        float voltage;         //计算值
        float integral;        //误差积分值
};

struct PID{
        int Set;             //设定值
        int Actual;          //实际值
        int err;             //当前误差
        int err_last;        //上一次误差
        int err_last_last;   //上上一次误差
        int last_derivative; //上次误差与上上次误差之差

        int Kp,Ki,Kd,Kout;
        int voltage;         //计算值
        int integral;        //误差积分值
};

#endif
