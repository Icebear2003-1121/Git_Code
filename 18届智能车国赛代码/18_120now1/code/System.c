/*
 * System.c
 *
 *  Created on: 2023年6月2日
 *      Author: 叁召a
 */

#include "System.h"

/************************************************
函数名：Peripheral_init
功  能：外设初始化
参  数：void
返回值：void
************************************************/
void Peripheral_init(void)
{
#ifdef IPS200
    //ips200初始化
    Screen_Int();
    ips200_show_string(0, 0, "ips200_init_ok");

    //按键拨码开关初始化
    key_Init();
    ips200_show_string(0, 16, "key_init_ok");

    //摄像头初始化
    mt9v03x_init();
    ips200_show_string(0, 30, "mt9v03x_init_ok");

    //电机初始化
    motor_init();
    ips200_show_string(0, 60, "pwm_init_ok");

    //电磁初始化
    elec_init();
    ips200_show_string(0, 90, "elec_init_ok");

    //串口初始化(虽然逐飞自带函数初始化了，但是此处不影响)
//    uart_init(UART_2,3000000,UART2_TX_P10_5,UART2_RX_P10_6);

    //无线转串口模块初始化
//    wireless_uart_init();

    //pid数值初始化
    Pid_Init();

    //控制参数初始化
    SpeedMode_Init();

    //gpio初始化
    gpio_init(P33_9,GPO,0,GPO_PUSH_PULL);

    //编码器初始化
    gpt_Init();
    ips200_show_string(0, 120, "gpt_init_ok");

    //设置定时中断(2ms)  此处注意区分新老程序上赋值的差异
    pit_ms_init(CCU60_CH0, 5);  //5ms
    pit_ms_init(CCU61_CH0, 1);  //1ms,电磁采集

    //陀螺仪初始化
    imu660ra_init();
    ips200_show_string(0, 150, "imu660ra_init_ok");

    //TOF测距初始化
    dl1a_init();
    ips200_show_string(0, 180, "dl1a_init_ok");

    //ADC初始化(用于电压检测)
    ips200_show_string(0, 210, "All_init_ok");
    ips200_show_string(0, 240, "Xkice_init_ok");
    flag.init_ok = 1;               //初始化成功
    system_delay_ms(300);  //延时
    ips200_clear();


#endif

#ifdef oled
       //ips200初始化
       oled_init();
       //按键拨码开关初始化
       key_Init();
       //摄像头初始化
       mt9v03x_init();
       //电机初始化
       motor_init();
       //电磁初始化
       elec_init();
       //串口初始化(虽然逐飞自带函数初始化了，但是此处不影响)
   //    uart_init(UART_2,3000000,UART2_TX_P10_5,UART2_RX_P10_6);
       //无线转串口模块初始化
   //    wireless_uart_init();
       //pid数值初始化
       Pid_Init();
       //gpio初始化
       gpio_init(P33_9,GPO,0,GPO_PUSH_PULL);
       //编码器初始化
       gpt_Init();
       //设置定时中断(2ms)  此处注意区分新老程序上赋值的差异
       pit_ms_init(CCU60_CH0, 5);  //5ms
           pit_ms_init(CCU61_CH0, 1);  //1ms,电磁采集
       //陀螺仪初始化
       imu660ra_init();
       //TOF测距初始化
       dl1a_init();
       //ADC初始化(用于电压检测)
       flag.init_ok = 1;               //初始化成功
#endif
}

/*****************************************
 函数名：Screen_Int
 作用：屏幕初始化，显示软件程序版本信息便于区分
 作者：Sanz
 日期：2023-05-21
 ************************************************/
void Screen_Int(void)
{
       unsigned char temp[20];
       ips200_set_dir(IPS200_PORTAIT_180);
       ips200_init(IPS200_TYPE_PARALLEL8);
       ips200_full(RGB565_WHITE);
       ips200_set_color(RGB565_GREEN, RGB565_BLACK);
       sprintf(temp,"Smart Car 2023-18th Soft V%0.2f",SOFT_VERSION); //打印字符串
       ips200_show_string(0, 300,  temp);
       system_delay_ms(200);
}

/************************************************
函数名：Parameter_init
功  能：参数初始化
参  数：void
返回值：void
************************************************/
void Parameter_init(void)
{
    //参数初始化
//    Parameter_write_eeprom_canshu();
//        Parameter_write_eeprom_canshu_2();
//        Parameter_write_eeprom_canshu_3();
    Key_Scan_caidan();
//    Parameter_read_eeprom_canshu();
//      Parameter_read_eeprom_canshu_2();
//      Parameter_read_eeprom_canshu_3();
//    Key_Scan_canshu();

}
