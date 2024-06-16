#include "Motor.h"
int32 PWML;
int32 PWMR;
/*---------------------------------------------------------------
                   �����ʼ��
            �����Զ���24����35��С
                  21_4 21_5��
                  21_2 21_3��
 ----------------------------------------------------------------*/
void motor_init(void)
{
    gpio_init(P21_4, GPO, 1, GPO_PUSH_PULL);
    gpio_init(P21_2, GPO, 0, GPO_PUSH_PULL);
    //��ʼ��PWM����
    pwm_init(MOTOR1_PWM, 17000, 0);
    pwm_init(MOTOR2_PWM, 17000, 0);

}

void motor_control(int32 duty_m1, int32 duty_m2)//m1��m2��
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

    pwm_set_duty(MOTOR1_PWM, duty_m2);
    pwm_set_duty(MOTOR2_PWM, duty_m1);
}
/*---------------------------------------------------------------
                   ����ջ�
         ��������
                ��     028       ��      337
                ��     009       ��      336
 ----------------------------------------------------------------*/
void Speed_Control(int32 duty_m1, int32 duty_m2)
{
    PWML=motorL_pid(duty_m1,gpt[0]);
    PWMR=motorR_pid(duty_m2,gpt[1]);
    motor_control( PWML,PWMR);
}

void oled_showgpt()
{
    oled_show_int(0, 0,gpt[0] , 3);
    oled_show_int(5, 0,gpt[1] , 3);
}
