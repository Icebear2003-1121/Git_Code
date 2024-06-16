/*
 * FuzzyPID.h
 *
 *  Created on: 2023��7��4��
 *      Author: tuweizong
 */

#ifndef FUZZYPID_H_
#define FUZZYPID_H_
#include "zf_Common_headfile.h"

#define NB -3 //��������ֵ
#define NM -2
#define NS -1
#define ZO 0
#define PS 1
#define PM 2
#define PB 3

void Get_grad_membership(float erro, float erro_c);
float Quantization(float maximum, float minimum, float x);
float Inverse_quantization(float maximum, float minimum, float qvalues);
void GetSumGrad();
void GetOUT();
void FuzzyPIDcontroller(float erro , float e_max, float e_min, float ec_max, float ec_min, float dkp_max, float dkp_min, float dki_max,float dki_min,float dkd_max,float dkd_min);

extern float e_max;  //������ֵ
extern float e_min;  //�����Сֵ
extern float ec_max;  //���仯���ֵ
extern float ec_min;  //���仯��Сֵ
extern float dkp_max, dkp_min;//pid�����仯��Χ
extern float dki_max, dki_min;
extern float dkd_max, dkd_min;
extern float dkp,dki,dkd;//pid�仯��

#endif /* FUZZYPID_H_ */
