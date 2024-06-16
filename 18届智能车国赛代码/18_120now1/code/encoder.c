/*
 * encoder.c
 *
 *  Created on: 2023年3月20日
 *      Author: 叁召a
 */
#include "encoder.h"

#define gpt_getL encoder_get_count(TIM2_ENCODER);//左轮编码器 + //编码器数据采集选择T2计时器
#define gpt_getR encoder_get_count(TIM5_ENCODER);//右轮编码器 - //编码器数据采集选择T5计时器
int break_speed;
int gptR_Cir=0,gptL_Cir=0,gptR_Rem=0,gptL_Rem=0;
int gpt[2]={0};//采集的编码器数据存放数组

/*---------------------------------------------------------------
 【函    数】gpt_Init
 【功    能】正交编码器采集初始化
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void gpt_Init(void)
{
    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);//正交编码器采集初始化
    encoder_quad_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);//
}
/*---------------------------------------------------------------
 【函    数】gpt_get
 【功    能】编码器数据存储
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void gpt_get(void)
{
    gpt[0] = gpt_getR;//将右轮编码器采集的数据存放在gpt[0]中
    gpt[1] = gpt_getL;//将左轮编码器采集的数据存放在gpt[1]中
    gpt_count();
    encoder_clear_count(TIM2_ENCODER);//编码器计数清零
    encoder_clear_count(TIM5_ENCODER);
    break_speed = (gpt[0] + gpt[1])/2;
}


void gpt_count(void)
{
    if((abs(gpt[0])+gptR_Rem)>100)
      {
          gptR_Cir+=(abs(gpt[0])+gptR_Rem)/100;
          gptR_Rem=(abs(gpt[0])+gptR_Rem)%100;
      }
      else
      {
          gptR_Rem=abs(gpt[0])+gptR_Rem;
      }

      if((abs(gpt[1])+gptL_Rem)>100)
      {
          gptL_Cir+=(abs(gpt[1])+gptL_Rem)/100;
          gptL_Rem=(abs(gpt[1])+gptL_Rem)%100;
      }
      else
      {
          gptL_Rem=abs(gpt[1])+gptL_Rem;
      }
}

void gpt_count_init(void)
{
    gptR_Cir=0;
    gptL_Cir=0;
    gptR_Rem=0;
    gptL_Rem=0;
}



