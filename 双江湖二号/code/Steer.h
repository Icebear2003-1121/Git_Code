/*
 * Steer.h
 *
 *  Created on: 2023��9��17��
 *      Author: huawei
 */

#ifndef CODE_STEER_H_
#define CODE_STEER_H_
#include "zf_common_headfile.h"
#define S_MOTOR_PIN   ATOM1_CH1_P33_9       //����������
void steer_init(void);
void steer_control(int32 duty_m1);




#endif /* CODE_STEER_H_ */
