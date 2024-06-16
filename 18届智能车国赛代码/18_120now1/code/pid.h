/*
 * pid.h
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_


#include "zf_common_headfile.h"

extern struct FPID CurPid;
extern struct FPID ElecPid;
extern struct FPID GptPidR;
extern struct FPID GptPidL;
extern struct FPID PidRStop;
extern struct FPID PidLStop;
extern struct FPID JSDPid;
//void PIDChang(struct PID a,int p,int i,int d);
void FPIDChang(struct FPID a,float p,float i,float d);
void Pid_Init(void);
int calculate_pid(float a);
int elec_pid(float a);
int motorR_pid(int a,int b);
int motorL_pid(int a,int b);
int motorRStop_pid(int a,int b);
int motorLStop_pid(int a,int b);
float ADRC_speed_control(float speed);
int dtmotorL_pid(int a, int b);
int dtmotorR_pid(int a, int b);
int Zebra_pid(int a,float b);
int Gyro_pid(float a,float b);
int JSD_pid(int a,int b);

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
        float GKD;             //������ϵ��
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









#endif /* CODE_PID_H_ */
