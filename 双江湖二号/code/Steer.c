#include "steer.h"



void steer_init(void)
{
    //初始化PWM引脚
    pwm_init(S_MOTOR_PIN, 50, 613);   //初始化舵机


}

void steer_control(int32 duty_m1)
{
    //对占空比限幅
    if(duty_m1 > PWM_DUTY_MAX)
        duty_m1 = PWM_DUTY_MAX;
    else if(duty_m1 < 0)
        duty_m1 = 0;

    pwm_set_duty(S_MOTOR_PIN, duty_m1);
}

