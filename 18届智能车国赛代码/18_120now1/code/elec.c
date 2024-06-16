/*
 * elec.c
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */

#include "elec.h"



#define ADC_CH1         ADC0_CH0_A0
#define ADC_CH2         ADC0_CH1_A1
#define ADC_CH3         ADC0_CH2_A2
#define ADC_CH4         ADC0_CH3_A3

int16 elec_value[8],elec_data[8],difference_s,difference_h,difference_zd,elecOut_flag=0;
float data_yaw,data_roll,data_pitch;
short gyrox,gyroy,gyroz;
short accx,accy,accz;

#define RtA   57.295779f //弧度->角度
#define AtR      0.0174533f //角度->弧度
#define Acc_G   0.0011963f
#define Gyro_G   0.0610351f
#define Gyro_Gr  0.0010653f

#define Kp 18.0f
#define Ki 0.008f
#define halfT 0.008f

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float exInt = 0, eyInt = 0, ezInt = 0;
float Angle[3]={0};//最终角度


void IMU_Update(short gyrox,short gyroy,short gyroz,short accx,short accy,short accz)
{
 float ax=accx,ay=accy,az=accz;
 float gx=gyrox,gy=gyroy,gz=gyroz;
   float norm;
   float vx, vy, vz;
   float ex, ey, ez;
   float q0q0 = q0*q0;
   float q0q1 = q0*q1;
   float q0q2 = q0*q2;
   float q1q1 = q1*q1;
   float q1q3 = q1*q3;
   float q2q2 = q2*q2;
   float q2q3 = q2*q3;
   float q3q3 = q3*q3;
//   oled_show_float(105,2,q3q3,2,3);

// if(ax*ay*az==0)//此时任意方向角加速度为0
//   return;
  gx *= Gyro_Gr;
 gy *= Gyro_Gr;
 gz *= Gyro_Gr;

   norm = myRsqrt(ax*ax + ay*ay + az*az);
   ax = ax /norm;
   ay = ay / norm;
   az = az / norm;

   // estimated direction of gravity and flux (v and w)
   vx = 2*(q1q3 - q0q2);
   vy = 2*(q0q1 + q2q3);
   vz = q0q0 - q1q1 - q2q2 + q3q3 ;

   // error is sum of cross product between reference direction of fields and direction   measured by sensors
   ex = (ay*vz - az*vy) ;
   ey = (az*vx - ax*vz) ;
   ez = (ax*vy - ay*vx) ;

   exInt = exInt + ex * Ki;
   eyInt = eyInt + ey * Ki;
   ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
   gx = gx + Kp*ex + exInt;
   gy = gy + Kp*ey + eyInt;
   gz = gz + Kp*ez + ezInt;

   // integrate quaternion rate and normalise
   q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
   q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
   q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
   q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

   // normalise quaternion
   norm = myRsqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

   q0 = q0 / norm;
   q1 = q1 / norm;
   q2 = q2 / norm;
   q3 = q3 / norm;

  data_yaw  = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* RtA;    // yaw
   data_pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* RtA ; //pitch
   data_roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* RtA;   //roll
}




void elec_init(void)
{
    //adc_init(ADC1_CH0_A0, ADC_8BIT);
    adc_init(ADC_CH1,ADC_12BIT);    // 初始化 ADC 已经对应通道引脚 10位精度
    adc_init(ADC_CH2,ADC_12BIT);    // 初始化 ADC 已经对应通道引脚 10位精度
    adc_init(ADC_CH3,ADC_12BIT);    // 初始化 ADC 已经对应通道引脚 10位精度
    adc_init(ADC_CH4,ADC_12BIT);    // 初始化 ADC 已经对应通道引脚 10位精度


}
void elec_get(void)
{
//    elec_value[0] = adc_mean_filter_convert(ADC_CH1,12);
//    elec_value[1] = adc_mean_filter_convert(ADC_CH2,12);
//    elec_value[2] = adc_mean_filter_convert(ADC_CH3,12);
//    elec_value[3] = adc_mean_filter_convert(ADC_CH4,12);

//    elec_value[0]= adc_mean_filter_convert(ADC_CH1,12);
////    elec_value[1] = adc_mean_filter_convert(ADC_CH4,12);
//    elec_value[1] = adc_mean_filter_convert(ADC_CH3,12);
    elec_value[0] = adc_mean_filter_convert(ADC_CH2,12);
    elec_value[1] = adc_mean_filter_convert(ADC_CH3,12);
//
//    oled_show_int(0, 3, elec_value[0], 4);//左横
//    oled_show_int(0, 4, elec_value[1], 4);//右横
//    oled_show_int(0, 5, elec_value[2], 4);//
//    oled_show_int(0, 6, elec_value[3], 4);


//    elec_data[0] = elec_value[0]/12/4095.0*1000;
//    elec_data[1] = elec_value[1]/12/4095.0*1000;//4095
//    elec_data[2] = elec_value[2]/12/4095.0*1000;
//    elec_data[3] = elec_value[3]/12/4095.0*1000;

    elec_data[0] = elec_value[0]/12/4000.0*1000;
    elec_data[1] = elec_value[1]/12/4000.0*1000;//4095
//    elec_data[1] = elec_value[1]/12/4000.0*1000;//4095
//    elec_data[2] = elec_value[2]/12/4000.0*1000;
 //   elec_data[3] = elec_value[3]/12/4000.0*1000;//4095
//
//    oled_show_int(30, 3, gpt[0], 4);
//    oled_show_int(30, 4, gpt[1], 4);
//    oled_show_int(30, 3, elec_data[0], 4);
//    oled_show_int(30, 4, elec_data[1], 4);

    double temp,a,d,b,c;
  //  temp = find_max();
   // oled_show_int(0, 4,temp, 4);

    for (int i = 0; i < 2; i++)//i<4
    {
       // elec_data[i] = (elec_data[i]/temp) * 100;
        elec_data[i] = elec_data[i] > 100 ? 100 : elec_data[i];
        elec_data[i] = elec_data[i] < 1 ? 1 : elec_data[i];
    }
//    a = elec_data[0];
//    b = elec_data[1];
//    c = elec_data[2];
//    d = elec_data[3];

//    b = elec_data[1];
//    c = elec_data[2];

    a = elec_data[0];
    d = elec_data[1];
//   oled_show_int(0, 6, b, 4);
//   oled_show_int(0, 7, c, 4);

    if(a+d<3)
    {
        elecOut_flag=1;
    }

    difference_s = ((b - c) / (b + c)) * 100; //ba
    difference_h = ((1/Q_rsqrt(a) - 1/Q_rsqrt(d)) / (a + d)) * 100;
//    difference_h = ((a - d) / (a + d)) * 100;
    difference_zd = (((b + a) - (d + c)) / ((b + a) + (d + c))) * 100;
//   oled_show_int(0, 7  , difference_h ,4);
//        oled_show_int(0, 3, elec_value[1], 4);
//        oled_show_int(0, 5, ((!RoadOut_flag && !elecOut_flag) || (RoadOut_flag && !elecOut_flag)) ,4);
//        oled_show_int(0, 6, RoadOut_flag, 4);
//        oled_show_int(0, 7, elecOut_flag, 4);
//        oled_show_int(30, 4, OpenRoadFlag, 4);
}

//void elec_calculate(void)
//{
//    gDir_Mid=difference_h;
//}
//-------------------------------------------------------------------------------------------------------------------
//  @brief    差比和的1.5次方函数
//  @param      data1  数据1
//  @param      data2  数据2
//  @param      multiple  放大倍数
//  @return     void
//  Sample usage:          ADC_collect();
//-------------------------------------------------------------------------------------------------------------------
double Cha_bi_he0(int16 data1, int16 data2, uint8 multiple)
{
    int16 cha;
    int16 he;
    double result;

    cha = data1-data2;
    he = data1+data2;
    result = (multiple*cha)/(1.0*he*sqrt(he));

    return result;
}

double find_max(void)
{
    double temp = 0;
    for (int i = 0; i < 5; i++)
    {
        if (temp < elec_data[i])//temp < elec_data[i]
        {
            temp = elec_data[i];
        }
    }
    return temp;
}

double find_min(void)
{
    double temp = 100000;
    for (int i = 1; i < 5; i++)
    {
        if (temp > elec_data[i])
        {
            temp = elec_data[i];
        }
    }
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       快速平方根倒数算法
// @param       float number
// @return      y
// Sample usage:                Q_rsqrt(9);
//-------------------------------------------------------------------------------------------------------------------
float Q_rsqrt(float number)
{
 long i;
 float x2,y;
 const float threehalfs = 1.5F;
 x2= number * 0.5F;
 y= number;
 i= * (long * ) &y;
 i= 0x5f3759df-(i>>1);
 y= * (float * ) &i;
 //上述代码给牛顿迭代法提供了一个较好的初始值
 y=y * (threehalfs - (x2*y*y));  //牛顿迭代一次
 y=y * (threehalfs - (x2*y*y));  //  可持续迭代
 return y;
}
