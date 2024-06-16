/*
 * key.h
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#include "zf_common_headfile.h"


//定义按键引脚
#define KEY01    P22_3
#define KEY02    P22_2
#define KEY03    P22_1
#define KEY04    P22_0
#define KEY05    P23_1
#define KEY06    P32_4
//定义拨码开关引脚
#define SW1     P33_11
#define SW2     P33_10
#define SW3     P33_13
#define SW4     P33_12

//按键值定义
#define KEY0_DATA     1
#define KEY1_DATA     2
#define KEY2_DATA     3
#define KEY3_DATA     4
#define KEY4_DATA     5
#define KEY5_DATA     6

//拨码开关状态变量
extern uint8 sw1_status;
extern uint8 sw2_status;
extern uint8 sw3_status;
extern uint8 sw4_status;

//开关状态变量
extern uint8 key1_status;
extern uint8 key2_status;
extern uint8 key3_status;
extern uint8 key4_status;
extern uint8 key5_status;
extern uint8 key6_status;

extern uint8 KEY0;
extern uint8 KEY1;
extern uint8 KEY2;
extern uint8 KEY3;
extern uint8 KEY4;
extern uint8 KEY5;

//上一次开关状态变量
extern uint8 key1_last_status;
extern uint8 key2_last_status;
extern uint8 key3_last_status;
extern uint8 key4_last_status;
extern uint8 key5_last_status;
extern uint8 key6_last_status;

//开关标志位
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;
extern uint8 key5_flag;
extern uint8 key6_flag;

extern uint8  keydown_data;      //按键按下后就返回的值
extern uint8  keyup_data;        //按键抬起返回值
extern uint16 key_time;          //按键按下之后的时间计数，该值乘以扫描一次按键函数的时间就等于按键按下的时间
extern uint8  key_tem;           //长按的按键值与按键扫描程序过渡变量
extern uint8  key_bak;           //按键扫描程序过渡变量



void key_Init(void);
void key(void);
void Key_Scan_canshu(void);
void Key_Scan_show(void);
void Key_Scan_direction(void);
void Key_Scan_caidan(void);
void Key_Scan_SpeedMode(void);
void Key_scan_CurPid();
void key_scan(uint8 mode);
//***************函数宏定义****(下面这些函数请修改宏定义为对应的GPIO库函数操作)***********
#define KEY_INT(key_x)           (gpio_init((key_x),GPI, 1, GPI_PULL_UP))//配置为上拉电阻
#define SWITCH_INT(switch_x)     (gpio_init((switch_x),GPI, 1, GPI_PULL_UP))//配置为上拉电阻
#define READ_GPIO(Pin_X)         (gpio_get_level((Pin_X)))

#define GPIO_INT(PIN_X,MODE,DATE)    gpio_init((PIN_X),GPO, (DATE), (MODE))
#define PIN_Write(PIN_X, DATE)       gpio_set_level((PIN_X), (DATE))

#endif /* CODE_KEY_H_ */

