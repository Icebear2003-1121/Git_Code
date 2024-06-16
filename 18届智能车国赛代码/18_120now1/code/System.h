/*
 * System.h
 *
 *  Created on: 2023年6月2日
 *      Author: 叁召a
 */

#ifndef CODE_SYSTEM_H_
#define CODE_SYSTEM_H_

#define SOFT_VERSION    7.18
#define IPS200   //IPS200调用ips200屏幕 ，oled调用oled屏幕
#include "zf_common_headfile.h"

void Peripheral_init(void);                               //外设初始化
void Screen_Int(void);                                    //屏幕初始化
void Parameter_init(void);                                //菜单初始化


#endif /* CODE_SYSTEM_H_ */
