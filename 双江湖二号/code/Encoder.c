#include "Encoder.h"

#define gpt_getL -encoder_get_count(TIM2_ENCODER);//×óÂÖ±àÂëÆ÷ -
#define gpt_getR encoder_get_count(TIM5_ENCODER);//ÓÒÂÖ±àÂëÆ÷ +


int gptR_Cir=0,gptL_Cir=0,gptR_Rem=0,gptL_Rem=0;
int gpt[2]={0};

void gpt_Init(void)
{
    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH2_P33_6, TIM2_ENCODER_CH1_P33_7);
    encoder_quad_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);
}

void gpt_get(void)
{
    gpt[0] = gpt_getL;
    gpt[1] = gpt_getR;

    gpt_count();

    encoder_clear_count(TIM2_ENCODER);
    encoder_clear_count(TIM5_ENCODER);
}

void gpt_count(void)
{
    if((abs(gpt[1])+gptR_Rem)>100)
    {
        gptR_Cir+=(abs(gpt[1])+gptR_Rem)/100;
        gptR_Rem=(abs(gpt[1])+gptR_Rem)%100;
    }
    else
    {
        gptR_Rem=abs(gpt[1])+gptR_Rem;
    }

    if((abs(gpt[0])+gptL_Rem)>100)
    {
        gptL_Cir+=(abs(gpt[0])+gptL_Rem)/100;
        gptL_Rem=(abs(gpt[0])+gptL_Rem)%100;
    }
    else
    {
        gptL_Rem=abs(gpt[0])+gptL_Rem;
    }
}

void gpt_count_init(void)
{
    gptR_Cir=0;
    gptL_Cir=0;
    gptR_Rem=0;
    gptL_Rem=0;
}
