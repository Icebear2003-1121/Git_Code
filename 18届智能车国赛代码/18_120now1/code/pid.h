/*
 * pid.h
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
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
        float Set;             //设定值
        float Actual;          //实际值
        float err;             //当前误差
        float err_last;        //上一次误差
        float err_last_last;   //上上一次误差
        float last_derivative; //上次误差与上上次误差之差

        float Kp,Ki,Kd,Kout;
        float voltage;         //计算值
        float integral;        //误差积分值
        float GKD;             //陀螺仪系数
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









#endif /* CODE_PID_H_ */
