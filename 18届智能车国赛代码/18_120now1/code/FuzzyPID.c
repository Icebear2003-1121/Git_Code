/*
 * FuzzyPID.c
 *
 *  Created on: 2023年7月4日
 *      Author: tuweizong
 */
#include "FuzzyPID.h"
#include "math.h"

const int  num_area = 8; //划分区域个数
float e_max=20;  //误差最大值
float e_min=0;  //误差最小值
float ec_max=5;  //误差变化最大值
float ec_min=0;  //误差变化最小值
float dkp_max, dkp_min;//pid调整变化范围
float dki_max, dki_min;
float dkd_max, dkd_min;
float dkp=0,dki=0,dkd=0;//pid变化量

float e_membership_values[7] = {-3,-2,-1,0,1,2,3}; //输入e的隶属值
float ec_membership_values[7] = { -3,-2,-1,0,1,2,3 };//输入de/dt的隶属值
float kp_menbership_values[7] = { -3,-2,-1,0,1,2,3 };//输出增量kp的隶属值
float ki_menbership_values[7] = { -3,-2,-1,0,1,2,3 }; //输出增量ki的隶属值
float kd_menbership_values[7] = { -3,-2,-1,0,1,2,3 };  //输出增量kd的隶属值
float fuzzyoutput_menbership_values[7] = { -3,-2,-1,0,1,2,3 };

float qdetail_kp=0;               //增量kp对应论域中的值
float qdetail_ki=0;               //增量ki对应论域中的值
float qdetail_kd=0;               //增量kd对应论域中的值
float detail_kp;                //输出增量kp
float detail_ki;                //输出增量ki
float detail_kd;                //输出增量kd
float qerro;                    //输入e对应论域中的值
float qerro_c;                  //输入de/dt对应论域中的值
float e_gradmembership[2];      //输入e的隶属度
float ec_gradmembership[2];     //输入de/dt的隶属度
int e_grad_index[2];            //输入e隶属度在规则表的索引
int ec_grad_index[2];           //输入de/dt隶属度在规则表的索引
float gradSums[7] = {0,0,0,0,0,0,0};
float KpgradSums[7] = { 0,0,0,0,0,0,0 };   //输出增量kp总的隶属度
float KigradSums[7] = { 0,0,0,0,0,0,0 };   //输出增量ki总的隶属度
float KdgradSums[7] = { 0,0,0,0,0,0,0 };   //输出增量kd总的隶属度

int  Kp_rule_list[7][7] = { {PB,PB,PM,PM,PS,ZO,ZO},     //kp规则表
                            {PB,PB,PM,PS,PS,ZO,NS},
                            {PM,PM,PM,PS,ZO,NS,NS},
                            {PM,PM,PS,ZO,NS,NM,NM},
                            {PS,PS,ZO,NS,NS,NM,NM},
                            {PS,ZO,NS,NM,NM,NM,NB},
                            {ZO,ZO,NM,NM,NM,NB,NB} };

int  Ki_rule_list[7][7] = { {NB,NB,NM,NM,NS,ZO,ZO},     //ki规则表
                            {NB,NB,NM,NS,NS,ZO,ZO},
                            {NB,NM,NS,NS,ZO,PS,PS},
                            {NM,NM,NS,ZO,PS,PM,PM},
                            {NM,NS,ZO,PS,PS,PM,PB},
                            {ZO,ZO,PS,PS,PM,PB,PB},
                            {ZO,ZO,PS,PM,PM,PB,PB} };

int  Kd_rule_list[7][7] = { {PS,NS,NB,NB,NB,NM,PS},    //kd规则表
                            {PS,NS,NB,NM,NM,NS,ZO},
                            {ZO,NS,NM,NM,NS,NS,ZO},
                            {ZO,NS,NS,NS,NS,NS,ZO},
                            {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                            {PB,NS,PS,PS,PS,PS,PB},
                            {PB,PM,PM,PM,PS,PS,PB} };

//输入e与de/dt隶属度计算函数//
void Get_grad_membership(float erro,float erro_c)
{
    if (erro > e_membership_values[0] && erro < e_membership_values[6])
    {
        for (int i = 0; i < num_area - 2; i++)
        {
            if (erro >= e_membership_values[i] && erro <= e_membership_values[i + 1])
            {
                e_gradmembership[0] = 1+(erro - e_membership_values[i + 1]) / (e_membership_values[i + 1] - e_membership_values[i]);//对左边的隶属度
                e_gradmembership[1] = -(erro - e_membership_values[i + 1]) / (e_membership_values[i + 1] - e_membership_values[i]);//对右边的隶属度
                e_grad_index[0] = i;//左
                e_grad_index[1] = i + 1;//右
                break;
            }
        }
    }
    else
    {
        if (erro <= e_membership_values[0])
        {
            e_gradmembership[0] = 1;
            e_gradmembership[1] = 0;
            e_grad_index[0] = 0;
            e_grad_index[1] = -1;
        }
        else if (erro >= e_membership_values[6])
        {
            e_gradmembership[0] = 1;
            e_gradmembership[1] = 0;
            e_grad_index[0] = 6;
            e_grad_index[1] = -1;
        }
    }

    if (erro_c > ec_membership_values[0] && erro_c < ec_membership_values[6])
    {
        for (int i = 0; i < num_area - 2; i++)
        {
            if (erro_c >= ec_membership_values[i] && erro_c <= ec_membership_values[i + 1])
            {
                ec_gradmembership[0] =  1 + (erro_c - ec_membership_values[i + 1]) / (ec_membership_values[i + 1] - ec_membership_values[i]);//对左边的隶属度
                ec_gradmembership[1] = -(erro_c - ec_membership_values[i + 1]) / (ec_membership_values[i + 1] - ec_membership_values[i]);//对右边的隶属度
                ec_grad_index[0] = i;//左
                ec_grad_index[1] = i + 1;//右
                break;
            }
        }
    }
    else
    {
        if (erro_c <= ec_membership_values[0])
        {
            ec_gradmembership[0] = 0;
            ec_gradmembership[1] = 1;
            ec_grad_index[0] = -1;
            ec_grad_index[1] = 0;
        }
        else if (erro_c >= ec_membership_values[6])
        {
            ec_gradmembership[0] = 1;
            ec_gradmembership[1] = 0;
            ec_grad_index[0] = 6;
            ec_grad_index[1] = -1;
        }
    }
}

//获取输出增量kp,ki,kd的总隶属度//
void GetSumGrad()
{
    for (int i = 0; i < num_area - 1; i++)
    {
        KpgradSums[i] = 0;
        KigradSums[i] = 0;
        KdgradSums[i] = 0;
    }
  for (int i=0;i<2;i++)
  {
      if (e_grad_index[i] == -1)
      {
       continue;
      }
      for (int j = 0; j < 2; j++)
      {
          if (ec_grad_index[j] != -1)
          {
              int indexKp = Kp_rule_list[e_grad_index[i]][ec_grad_index[j]] + 3;
              int indexKi = Ki_rule_list[e_grad_index[i]][ec_grad_index[j]] + 3;
              int indexKd = Kd_rule_list[e_grad_index[i]][ec_grad_index[j]] + 3;
              //gradSums[index] = gradSums[index] + (e_gradmembership[i] * ec_gradmembership[j])* Kp_rule_list[e_grad_index[i]][ec_grad_index[j]];
              KpgradSums[indexKp]= KpgradSums[indexKp] + (e_gradmembership[i] * ec_gradmembership[j]);
              KigradSums[indexKi] = KigradSums[indexKi] + (e_gradmembership[i] * ec_gradmembership[j]);
              KdgradSums[indexKd] = KdgradSums[indexKd] + (e_gradmembership[i] * ec_gradmembership[j]);
          }
          else
          {
            continue;
          }

      }
  }
}

//计算输出增量kp,kd,ki对应论域值//
void GetOUT()
{
    for (int i = 0; i < num_area - 1; i++)
    {
        qdetail_kp += kp_menbership_values[i] * KpgradSums[i];
        qdetail_ki += ki_menbership_values[i] * KigradSums[i];
        qdetail_kd += kd_menbership_values[i] * KdgradSums[i];
    }
}

//区间映射函数///
float Quantization(float maximum,float minimum,float x)//实际映射至论域
{
    float qvalues= (6.0 *(x-minimum)/(maximum - minimum))-3;
    //float qvalues=6.0*()
    return qvalues;

    //qvalues[1] = 3.0 * ecerro / (maximum - minimum);
}

//反区间映射函数
float Inverse_quantization(float maximum, float minimum, float qvalues)//论域映射至实际
{
    float x = ((maximum - minimum) *(qvalues + 3)/6) + minimum;
    return x;
}

//模糊PID控制实现函数//
//FuzzyPIDcontroller(gDir_Mid*1000, 10, 0, 5, 0, 35, 20, 10, 0, 20, 10);
void FuzzyPIDcontroller(float erro , float e_max, float e_min, float ec_max, float ec_min, float dkp_max, float dkp_min, float dki_max,float dki_min,float dkd_max,float dkd_min)
{
    static float erro_pre=0;
    static float erro_ppre=0;
    static float errosum=0;
    static float erro_c=0;

    erro_c = erro - erro_pre;
    qerro =  -Quantization(e_max, e_min, fabs(erro));
    qerro_c = -Quantization(ec_max, ec_min, fabs(erro_c));
    Get_grad_membership(qerro, qerro_c);
    GetSumGrad();
    GetOUT();
    detail_kp = Inverse_quantization(dkp_max, dkp_min, qdetail_kp);
    detail_ki = Inverse_quantization(dki_max, dki_min, qdetail_ki);
    detail_kd = Inverse_quantization(dkd_max, dkd_min, qdetail_kd);
    qdetail_kd = 0;
    qdetail_ki = 0;
    qdetail_kp = 0;
    dkp = detail_kp;
    dki = detail_ki;
    dkd = detail_kd;
//    if (kp < 0)
//        kp = 0;
//    if (ki < 0)
//        ki = 0;
//    if (kd < 0)
//        kd = 0;
    detail_kp = 0;
    detail_ki = 0;
    detail_kd = 0;
    erro_ppre=erro_pre;
    erro_pre=erro;
    errosum+=erro;
}





