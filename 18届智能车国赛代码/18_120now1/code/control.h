/*
 * control.h
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */

#ifndef CODE_CONTROL_H_
#define CODE_CONTROL_H_

#include "zf_common_headfile.h"
#include "encoder.h"
#include "pid.h"
#include "ScanLine.h"
#include "ImageProcess.h"
//==========================================宏定义=====================================================

#define PwmMax 9000  //电机输出限幅
#define Control_jiao
// Control_chuan默认为不加角速度环的串级（仅方向环速度环）
// Control_bing默认为不加角速度环的并级（仅方向环速度环）
// Control_jiao默认为加角速度环的串级
#define Camera //elec电磁循迹，Camera摄像头循迹

//==========================================外部引用===================================================

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

//==========================================函数声明====================================================

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
