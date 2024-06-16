/*
 * key.h
 *
 *  Created on: 2023年9月22日
 *      Author: huawei
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#include "zf_common_headfile.h"
#include "Contol.h"

extern uint8 sw_status;          //拨码开关状态变量

extern uint8 LeftGo;              //左出库
extern uint8 RightGo;             //右出库
extern uint8 Go;                  //直接出发

//定义拨码开关引脚
#define SW1     P33_12
#define SW2     P33_13
//定义按键引脚
#define KEY1    P20_6
#define KEY2    P20_7
#define KEY3    P11_2
#define KEY4    P11_3

void DIP_Switch_Status(void);
void key_Init(void);
void key_scan(void);
void Keystroke_Depart(void);
#endif /* CODE_KEY_H_ */
