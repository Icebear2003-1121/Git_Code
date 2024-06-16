/*
 * ZW_Tools.h
 *
 *  Created on: 2023年4月10日
 *      Author: 叁召a
 */

#ifndef CODE_ZW_TOOLS_H_
#define CODE_ZW_TOOLS_H_

/*本函数基于逐飞驱动库进行开发需要包含逐飞UART函数头文件可根据自己的库自行更改*/
#include "zf_driver_uart.h"
#include "zf_common_headfile.h"

/*图像尺寸大小        切记必须与上位机填写的尺寸对应否则无法使用*/
/*图像尺寸大小        不得超过 长×宽<=10000*/
#define     ZW_ImgSize_H        COLH
#define     ZW_ImgSize_W        COLW

/*空数据定义*/
#define     ZW_NULL                 0

/*UART数据发宏定义    需要用户更改为自己的UART发送函数*/
#define     ZW_Putchar(ZW_data)     uart_write_byte(UART_2,ZW_data)

/*数据类型*/
typedef         unsigned char       ZW_uint8;
typedef         unsigned short      ZW_uint16;

/*函数声明*/
void ZW_Send_Image(ZW_uint8* ZW_Image);
void ZW_Send_Boundary(ZW_uint8* ZW_Left_Boundary, ZW_uint8* ZW_Right_Boundary);







#endif /* CODE_ZW_TOOLS_H_ */
