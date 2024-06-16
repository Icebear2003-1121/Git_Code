/*
 * motor.h
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_



#include "zf_common_headfile.h"

#define MOTORR_PWM   ATOM0_CH1_P21_3    //定义1电机PWM引脚
#define MOTORL_PWM   ATOM0_CH3_P21_5   //定义2电机PWM引脚

#define MOTORR_DIR   P21_2
#define MOTORL_DIR   P21_4

void motor_init(void);
void motor_control(int32 duty_m1, int32 duty_m2);






#endif /* CODE_MOTOR_H_ */
