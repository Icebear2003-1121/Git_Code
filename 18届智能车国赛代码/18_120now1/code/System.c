/*
 * System.c
 *
 *  Created on: 2023��6��2��
 *      Author: ����a
 */

#include "System.h"

/************************************************
��������Peripheral_init
��  �ܣ������ʼ��
��  ����void
����ֵ��void
************************************************/
void Peripheral_init(void)
{
#ifdef IPS200
    //ips200��ʼ��
    Screen_Int();
    ips200_show_string(0, 0, "ips200_init_ok");

    //�������뿪�س�ʼ��
    key_Init();
    ips200_show_string(0, 16, "key_init_ok");

    //����ͷ��ʼ��
    mt9v03x_init();
    ips200_show_string(0, 30, "mt9v03x_init_ok");

    //�����ʼ��
    motor_init();
    ips200_show_string(0, 60, "pwm_init_ok");

    //��ų�ʼ��
    elec_init();
    ips200_show_string(0, 90, "elec_init_ok");

    //���ڳ�ʼ��(��Ȼ����Դ�������ʼ���ˣ����Ǵ˴���Ӱ��)
//    uart_init(UART_2,3000000,UART2_TX_P10_5,UART2_RX_P10_6);

    //����ת����ģ���ʼ��
//    wireless_uart_init();

    //pid��ֵ��ʼ��
    Pid_Init();

    //���Ʋ�����ʼ��
    SpeedMode_Init();

    //gpio��ʼ��
    gpio_init(P33_9,GPO,0,GPO_PUSH_PULL);

    //��������ʼ��
    gpt_Init();
    ips200_show_string(0, 120, "gpt_init_ok");

    //���ö�ʱ�ж�(2ms)  �˴�ע���������ϳ����ϸ�ֵ�Ĳ���
    pit_ms_init(CCU60_CH0, 5);  //5ms
    pit_ms_init(CCU61_CH0, 1);  //1ms,��Ųɼ�

    //�����ǳ�ʼ��
    imu660ra_init();
    ips200_show_string(0, 150, "imu660ra_init_ok");

    //TOF����ʼ��
    dl1a_init();
    ips200_show_string(0, 180, "dl1a_init_ok");

    //ADC��ʼ��(���ڵ�ѹ���)
    ips200_show_string(0, 210, "All_init_ok");
    ips200_show_string(0, 240, "Xkice_init_ok");
    flag.init_ok = 1;               //��ʼ���ɹ�
    system_delay_ms(300);  //��ʱ
    ips200_clear();


#endif

#ifdef oled
       //ips200��ʼ��
       oled_init();
       //�������뿪�س�ʼ��
       key_Init();
       //����ͷ��ʼ��
       mt9v03x_init();
       //�����ʼ��
       motor_init();
       //��ų�ʼ��
       elec_init();
       //���ڳ�ʼ��(��Ȼ����Դ�������ʼ���ˣ����Ǵ˴���Ӱ��)
   //    uart_init(UART_2,3000000,UART2_TX_P10_5,UART2_RX_P10_6);
       //����ת����ģ���ʼ��
   //    wireless_uart_init();
       //pid��ֵ��ʼ��
       Pid_Init();
       //gpio��ʼ��
       gpio_init(P33_9,GPO,0,GPO_PUSH_PULL);
       //��������ʼ��
       gpt_Init();
       //���ö�ʱ�ж�(2ms)  �˴�ע���������ϳ����ϸ�ֵ�Ĳ���
       pit_ms_init(CCU60_CH0, 5);  //5ms
           pit_ms_init(CCU61_CH0, 1);  //1ms,��Ųɼ�
       //�����ǳ�ʼ��
       imu660ra_init();
       //TOF����ʼ��
       dl1a_init();
       //ADC��ʼ��(���ڵ�ѹ���)
       flag.init_ok = 1;               //��ʼ���ɹ�
#endif
}

/*****************************************
 ��������Screen_Int
 ���ã���Ļ��ʼ������ʾ�������汾��Ϣ��������
 ���ߣ�Sanz
 ���ڣ�2023-05-21
 ************************************************/
void Screen_Int(void)
{
       unsigned char temp[20];
       ips200_set_dir(IPS200_PORTAIT_180);
       ips200_init(IPS200_TYPE_PARALLEL8);
       ips200_full(RGB565_WHITE);
       ips200_set_color(RGB565_GREEN, RGB565_BLACK);
       sprintf(temp,"Smart Car 2023-18th Soft V%0.2f",SOFT_VERSION); //��ӡ�ַ���
       ips200_show_string(0, 300,  temp);
       system_delay_ms(200);
}

/************************************************
��������Parameter_init
��  �ܣ�������ʼ��
��  ����void
����ֵ��void
************************************************/
void Parameter_init(void)
{
    //������ʼ��
//    Parameter_write_eeprom_canshu();
//        Parameter_write_eeprom_canshu_2();
//        Parameter_write_eeprom_canshu_3();
    Key_Scan_caidan();
//    Parameter_read_eeprom_canshu();
//      Parameter_read_eeprom_canshu_2();
//      Parameter_read_eeprom_canshu_3();
//    Key_Scan_canshu();

}
