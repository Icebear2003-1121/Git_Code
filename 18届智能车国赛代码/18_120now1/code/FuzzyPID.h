/*
 * FuzzyPID.h
 *
 *  Created on: 2023年7月4日
 *      Author: tuweizong
 */

#ifndef FUZZYPID_H_
#define FUZZYPID_H_
#include "zf_Common_headfile.h"

#define NB -3 //论域隶属值
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

extern float e_max;  //误差最大值
extern float e_min;  //误差最小值
extern float ec_max;  //误差变化最大值
extern float ec_min;  //误差变化最小值
extern float dkp_max, dkp_min;//pid调整变化范围
extern float dki_max, dki_min;
extern float dkd_max, dkd_min;
extern float dkp,dki,dkd;//pid变化量

#endif /* FUZZYPID_H_ */
