#ifndef _Motor_h
#define _Motor_h
#include "zf_common_headfile.h"
#include "Pid.h"
#include "Encoder.h"
extern int32 PWML;
extern int32 PWMR;
#define MOTOR1_PWM   ATOM0_CH3_P21_5    //定义1电机PWM引脚
#define MOTOR2_PWM   ATOM0_CH1_P21_3   //定义2电机PWM引脚

#define MOTOR1_DIR   P21_4
#define MOTOR2_DIR   P21_2

void motor_init(void);
void Speed_Control(int32 duty_m1, int32 duty_m2);
void motor_control(int32 duty_m1, int32 duty_m2);
void oled_showgpt();
#endif
