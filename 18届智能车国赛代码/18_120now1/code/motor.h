/*
 * motor.h
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_



#include "zf_common_headfile.h"

#define MOTORR_PWM   ATOM0_CH1_P21_3    //����1���PWM����
#define MOTORL_PWM   ATOM0_CH3_P21_5   //����2���PWM����

#define MOTORR_DIR   P21_2
#define MOTORL_DIR   P21_4

void motor_init(void);
void motor_control(int32 duty_m1, int32 duty_m2);






#endif /* CODE_MOTOR_H_ */
