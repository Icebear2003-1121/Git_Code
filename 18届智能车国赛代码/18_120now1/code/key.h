/*
 * key.h
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#include "zf_common_headfile.h"


//���尴������
#define KEY01    P22_3
#define KEY02    P22_2
#define KEY03    P22_1
#define KEY04    P22_0
#define KEY05    P23_1
#define KEY06    P32_4
//���岦�뿪������
#define SW1     P33_11
#define SW2     P33_10
#define SW3     P33_13
#define SW4     P33_12

//����ֵ����
#define KEY0_DATA     1
#define KEY1_DATA     2
#define KEY2_DATA     3
#define KEY3_DATA     4
#define KEY4_DATA     5
#define KEY5_DATA     6

//���뿪��״̬����
extern uint8 sw1_status;
extern uint8 sw2_status;
extern uint8 sw3_status;
extern uint8 sw4_status;

//����״̬����
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

//��һ�ο���״̬����
extern uint8 key1_last_status;
extern uint8 key2_last_status;
extern uint8 key3_last_status;
extern uint8 key4_last_status;
extern uint8 key5_last_status;
extern uint8 key6_last_status;

//���ر�־λ
extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;
extern uint8 key5_flag;
extern uint8 key6_flag;

extern uint8  keydown_data;      //�������º�ͷ��ص�ֵ
extern uint8  keyup_data;        //����̧�𷵻�ֵ
extern uint16 key_time;          //��������֮���ʱ���������ֵ����ɨ��һ�ΰ���������ʱ��͵��ڰ������µ�ʱ��
extern uint8  key_tem;           //�����İ���ֵ�밴��ɨ�������ɱ���
extern uint8  key_bak;           //����ɨ�������ɱ���



void key_Init(void);
void key(void);
void Key_Scan_canshu(void);
void Key_Scan_show(void);
void Key_Scan_direction(void);
void Key_Scan_caidan(void);
void Key_Scan_SpeedMode(void);
void Key_scan_CurPid();
void key_scan(uint8 mode);
//***************�����궨��****(������Щ�������޸ĺ궨��Ϊ��Ӧ��GPIO�⺯������)***********
#define KEY_INT(key_x)           (gpio_init((key_x),GPI, 1, GPI_PULL_UP))//����Ϊ��������
#define SWITCH_INT(switch_x)     (gpio_init((switch_x),GPI, 1, GPI_PULL_UP))//����Ϊ��������
#define READ_GPIO(Pin_X)         (gpio_get_level((Pin_X)))

#define GPIO_INT(PIN_X,MODE,DATE)    gpio_init((PIN_X),GPO, (DATE), (MODE))
#define PIN_Write(PIN_X, DATE)       gpio_set_level((PIN_X), (DATE))

#endif /* CODE_KEY_H_ */

