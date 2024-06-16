/*
 * ZW_Tools.h
 *
 *  Created on: 2023��4��10��
 *      Author: ����a
 */

#ifndef CODE_ZW_TOOLS_H_
#define CODE_ZW_TOOLS_H_

/*���������������������п�����Ҫ�������UART����ͷ�ļ��ɸ����Լ��Ŀ����и���*/
#include "zf_driver_uart.h"
#include "zf_common_headfile.h"

/*ͼ��ߴ��С        �мǱ�������λ����д�ĳߴ��Ӧ�����޷�ʹ��*/
/*ͼ��ߴ��С        ���ó��� ������<=10000*/
#define     ZW_ImgSize_H        COLH
#define     ZW_ImgSize_W        COLW

/*�����ݶ���*/
#define     ZW_NULL                 0

/*UART���ݷ��궨��    ��Ҫ�û�����Ϊ�Լ���UART���ͺ���*/
#define     ZW_Putchar(ZW_data)     uart_write_byte(UART_2,ZW_data)

/*��������*/
typedef         unsigned char       ZW_uint8;
typedef         unsigned short      ZW_uint16;

/*��������*/
void ZW_Send_Image(ZW_uint8* ZW_Image);
void ZW_Send_Boundary(ZW_uint8* ZW_Left_Boundary, ZW_uint8* ZW_Right_Boundary);







#endif /* CODE_ZW_TOOLS_H_ */
