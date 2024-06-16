
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
        float Set;             //�趨ֵ
        float Actual;          //ʵ��ֵ
        float err;             //��ǰ���
        float err_last;        //��һ�����
        float err_last_last;   //����һ�����
        float last_derivative; //�ϴ���������ϴ����֮��

        float Kp,Ki,Kd,Kout;
        float voltage;         //����ֵ
        float integral;        //������ֵ
};

struct PID{
        int Set;             //�趨ֵ
        int Actual;          //ʵ��ֵ
        int err;             //��ǰ���
        int err_last;        //��һ�����
        int err_last_last;   //����һ�����
        int last_derivative; //�ϴ���������ϴ����֮��

        int Kp,Ki,Kd,Kout;
        int voltage;         //����ֵ
        int integral;        //������ֵ
};

#endif
