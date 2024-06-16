/*
 * encoder.c
 *
 *  Created on: 2023��3��20��
 *      Author: ����a
 */
#include "encoder.h"

#define gpt_getL encoder_get_count(TIM2_ENCODER);//���ֱ����� + //���������ݲɼ�ѡ��T2��ʱ��
#define gpt_getR encoder_get_count(TIM5_ENCODER);//���ֱ����� - //���������ݲɼ�ѡ��T5��ʱ��
int break_speed;
int gptR_Cir=0,gptL_Cir=0,gptR_Rem=0,gptL_Rem=0;
int gpt[2]={0};//�ɼ��ı��������ݴ������

/*---------------------------------------------------------------
 ����    ����gpt_Init
 ����    �ܡ������������ɼ���ʼ��
 ����    ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void gpt_Init(void)
{
    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);//�����������ɼ���ʼ��
    encoder_quad_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);//
}
/*---------------------------------------------------------------
 ����    ����gpt_get
 ����    �ܡ����������ݴ洢
 ����    ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void gpt_get(void)
{
    gpt[0] = gpt_getR;//�����ֱ������ɼ������ݴ����gpt[0]��
    gpt[1] = gpt_getL;//�����ֱ������ɼ������ݴ����gpt[1]��
    gpt_count();
    encoder_clear_count(TIM2_ENCODER);//��������������
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



