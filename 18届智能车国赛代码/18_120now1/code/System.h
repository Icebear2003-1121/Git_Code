/*
 * System.h
 *
 *  Created on: 2023��6��2��
 *      Author: ����a
 */

#ifndef CODE_SYSTEM_H_
#define CODE_SYSTEM_H_

#define SOFT_VERSION    7.18
#define IPS200   //IPS200����ips200��Ļ ��oled����oled��Ļ
#include "zf_common_headfile.h"

void Peripheral_init(void);                               //�����ʼ��
void Screen_Int(void);                                    //��Ļ��ʼ��
void Parameter_init(void);                                //�˵���ʼ��


#endif /* CODE_SYSTEM_H_ */
