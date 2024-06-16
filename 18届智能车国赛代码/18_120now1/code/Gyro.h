/*
 * Gyro.h
 *
 *  Created on: 2023��5��13��
 *      Author: ����a
 */

#ifndef CODE_GYRO_H_
#define CODE_GYRO_H_


#include "zf_common_headfile.h"

//#define PI 3.1415f
#define delta_T 0.001f //1ms

extern float X,Y,Z;
extern float acc_x,acc_y,acc_z;
extern float gyro_x,gyro_y,gyro_z;
extern float Q0,Q1,Q2,Q3;//��¼��Ԫ�� ��ʼ��1 0 0 0
extern float I_ex, I_ey, I_ez;  // ������
extern float icm_kp;    // ���ٶȼƵ��������ʱ�������
extern float icm_ki;   // �������������ʵĻ�������
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
