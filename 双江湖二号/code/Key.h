/*
 * key.h
 *
 *  Created on: 2023��9��22��
 *      Author: huawei
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#include "zf_common_headfile.h"
#include "Contol.h"

extern uint8 sw_status;          //���뿪��״̬����

extern uint8 LeftGo;              //�����
extern uint8 RightGo;             //�ҳ���
extern uint8 Go;                  //ֱ�ӳ���

//���岦�뿪������
#define SW1     P33_12
#define SW2     P33_13
//���尴������
#define KEY1    P20_6
#define KEY2    P20_7
#define KEY3    P11_2
#define KEY4    P11_3

void DIP_Switch_Status(void);
void key_Init(void);
void key_scan(void);
void Keystroke_Depart(void);
#endif /* CODE_KEY_H_ */
