/*
 * elec.h
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
 */

#ifndef CODE_ELEC_H_
#define CODE_ELEC_H_


#include "zf_common_headfile.h"

extern int16 elec_value[8],elec_data[8],difference_s,difference_h,difference_zd,elecOut_flag;
/*************************
*��������IMU_Update
*���룺�����ǣ����ٶȼ�ԭʼ���ݣ�short����)
*************************/
void IMU_Update(short gyrox,short gyroy,short gyroz,short accx,short accy,short accz);
void elec_init(void);
void elec_get(void);
void elec_calculate(void);

double find_max(void);
double find_min(void);
float Q_rsqrt(float number);




#endif /* CODE_ELEC_H_ */
