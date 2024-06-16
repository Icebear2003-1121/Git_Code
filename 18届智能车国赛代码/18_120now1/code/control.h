/*
 * control.h
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
 */

#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_

#include "zf_common_headfile.h"
#include "encoder.h"
#include "pid.h"
#include "ScanLine.h"
#include "ImageProcess.h"
//==========================================�궨��=====================================================

#define PwmMax 9000  //�������޷�
#define Control_jiao
// Control_chuanĬ��Ϊ���ӽ��ٶȻ��Ĵ������������ٶȻ���
// Control_bingĬ��Ϊ���ӽ��ٶȻ��Ĳ������������ٶȻ���
// Control_jiaoĬ��Ϊ�ӽ��ٶȻ��Ĵ���
#define Camera //elec���ѭ����Camera����ͷѭ��

//==========================================�ⲿ����===================================================

extern float gDir_Mid;
extern int motorR_Set,motorL_Set;
extern int speed_base;
extern int speed_out;
extern uint8 ZebraStop;
extern int curvature[][3];
extern int curvatureUp[][3];
extern uint8 Speedlow;
extern int strategic_mode;
extern int curvature_mid;
extern float ElemforeisightSet;
extern float BarrierIn;
extern float BarrierOut;
extern float foreisightSet;
extern int tofdistance;
extern float ZebraIn1;
extern float ZebraIn2;
extern int Barriergpt;
extern int strightSpeed;
extern int ElementSpeed;
extern int CirSpeed;

//==========================================��������====================================================

void SpeedMode_Init(void);
int Dynamic_foreisight();
void Anchorsite(int a[][3],int cur,int step,uint8 mode);
void bias_curvature(void);
void SpeedControl(unsigned char mode);
void motor_allcontrol(void);
void DirectionControl(void);
float one_curvature(int x1, int y1);
void curvatureAll(uint8 type);




#endif /* CODE_CONTROL_H_ */
