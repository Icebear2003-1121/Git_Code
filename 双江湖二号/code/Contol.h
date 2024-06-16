/*
 * Contol.h
 *
 *  Created on: 2023年9月17日
 *      Author: huawei
 */

#ifndef CODE_CONTOL_H_
#define CODE_CONTOL_H_
#include "zf_common_headfile.h"
#include "steer.h"
#include "camera.h"
#include "pid.h"

void bias_curvature(void);
void steer_allcontrol(void);
float one_curvature(int x1, int y1);
void gDir_Filter(void);
void outgoing(void);
void bias_curvature2(void);
void SpeedDecision(void);
int Dynamic_foreisight(void);


extern int angle;
extern uint8 Car;
extern int speed_base;//基础速度
extern int SetSpeed;
extern uint8 ZebraStop;//斑马线停车标志位
extern uint8 BlackNumAll;
extern int speed_out;
extern float gDir_MidFilter;//总偏差
extern int curvature[11][3];

#endif /* CODE_CONTOL_H_ */
