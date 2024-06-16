/*
 * encoder.h
 *
 *  Created on: 2023Äê9ÔÂ19ÈÕ
 *      Author: huawei
 */

#ifndef CODE_ENCODER_H_
#define CODE_ENCODER_H_
#include "zf_common_headfile.h"

extern int Gptout;
extern int gpt[2];
extern int gptR_Cir,gptL_Cir,gptR_Rem,gptL_Rem;

void gpt_Init(void);
void gpt_get(void);
void gpt_count(void);
void gpt_count_init(void);


#endif /* CODE_ENCODER_H_ */
