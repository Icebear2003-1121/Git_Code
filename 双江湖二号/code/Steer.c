#include "steer.h"



void steer_init(void)
{
    //��ʼ��PWM����
    pwm_init(S_MOTOR_PIN, 50, 613);   //��ʼ�����


}

void steer_control(int32 duty_m1)
{
    //��ռ�ձ��޷�
    if(duty_m1 > PWM_DUTY_MAX)
        duty_m1 = PWM_DUTY_MAX;
    else if(duty_m1 < 0)
        duty_m1 = 0;

    pwm_set_duty(S_MOTOR_PIN, duty_m1);
}

