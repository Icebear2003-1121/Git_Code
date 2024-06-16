/*
 * Gyro.h
 *
 *  Created on: 2023年5月13日
 *      Author: 叁召a
 */

#ifndef CODE_GYRO_H_
#define CODE_GYRO_H_


#include "zf_common_headfile.h"

//#define PI 3.1415f
#define delta_T 0.001f //1ms

extern float X,Y,Z;
extern float acc_x,acc_y,acc_z;
extern float gyro_x,gyro_y,gyro_z;
extern float Q0,Q1,Q2,Q3;//记录四元数 初始化1 0 0 0
extern float I_ex, I_ey, I_ez;  // 误差积分
extern float icm_kp;    // 加速度计的收敛速率比例增益
extern float icm_ki;   // 陀螺仪收敛速率的积分增益
extern float pitch, roll,yaw;
extern uint8 GyroOn;
extern uint8 GyroINT;

void gyroOffsetInit(void);
float myRsqrt(float num);
void ImuGetValues(void);
void ImuAHRSupdate(void);
void ReadGyro(void);
void ClearGyro(void);
void GyroResolve(void);



#endif /* CODE_GYRO_H_ */
