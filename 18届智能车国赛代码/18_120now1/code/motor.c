/*
 * motor.c
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
 */



#include "motor.h"

void motor_init(void)
{
    gpio_init(P21_4, GPO, 0, GPO_PUSH_PULL);
    gpio_init(P21_2, GPO, 0, GPO_PUSH_PULL);
    //��ʼ��PWM����
    pwm_init(MOTORR_PWM, 17000, 0);
    pwm_init(MOTORL_PWM, 17000, 0);

}

void motor_control(int32 duty_m1, int32 duty_m2)
{
    //��ռ�ձ��޷�
    if(duty_m1 > PWM_DUTY_MAX)
        duty_m1 = PWM_DUTY_MAX;
    else if(duty_m1 < 0)
        duty_m1 = 0;

    if(duty_m2 > PWM_DUTY_MAX)
        duty_m2 = PWM_DUTY_MAX;
    else if(duty_m2 < 0)
        duty_m2 = 0;

    pwm_set_duty(MOTORR_PWM, duty_m1);
    pwm_set_duty(MOTORL_PWM, duty_m2);


}


