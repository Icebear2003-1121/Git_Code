#include "ZW_Tools.h"
#include "zf_common_headfile.h"
/************************************************************
                                                 ����ͼ��

���������ơ�ZW_Send_Image
����    �ܡ�ͼ���ͺ���
����    ����ZW_Image��ֵ��ͼ�����飨0���0���Ķ�ֵ��ͼ��
���� �� ֵ����
��ʵ    ����ZW_Send_Image(Image);
��ע�������ֻ�ܴ����ֵ��ͼ��0���0��
                        ��.h�ļ��к궨��ߴ��������λ������д�ĳߴ��Ӧ�����޷�ʹ��
                        �۸ú��������ͼ��������±���������λ������

************************************************************/

void ZW_Send_Image(ZW_uint8* ZW_Image)
{
    ZW_uint8 ZW_data;
    ZW_uint16 ZW_i;
    ZW_uint8 ZW_j;

    for(ZW_i = 0; ZW_i < ZW_ImgSize_H*ZW_ImgSize_W; ZW_i += 8)
    {
        ZW_data = 0;
        for(ZW_j = 0; ZW_j < 8; ZW_j++)
        {
            if(*(ZW_Image + ZW_i + ZW_j))       ZW_data |= 0x01 << (7-ZW_j);
        }
        ZW_Putchar(ZW_data);
    }
}

/************************************************************
                                                 ����ͼ��

���������ơ�ZW_Send_Boundary
����    �ܡ����ͱ߽纯��
����    ����ZW_Left_Boundary, ZW_Right_Boundary���ұ߽�һά�������飨�������£�
���� �� ֵ����
��ʵ    ����ZW_Send_Boundary(left_Boundary, right_Boundary);
��ע������ٱ߽����鳤�ȱ�����ͼ��߶�һֱ��
                        ��ÿ�����ұ߽����ҽ���һ�������
                        �����ұ߽����ͬʱ���룬���ɵ�������һ���߽�
                        ��������ֽ���С��1430Byte
                        �����ֽ������㷽����
                        ͼ��ߴ�(���˿�)/8��β������һ���ֽ�����һ���ֽ� + �߽���Ϣ��һ��Byte�洢�����߽����2*ZW_ImgSize_H
                        ����ͼ�����������������߽���Ϣ
                        �޴˺����Ǳ�����ã�����ʹ�ô˺����������ͼ��

************************************************************/
void ZW_Send_Boundary(ZW_uint8* ZW_Left_Boundary, ZW_uint8* ZW_Right_Boundary)
{
    ZW_uint8 ZW_data;
    ZW_uint8 ZW_i,ZW_j;
    if(ZW_Left_Boundary != ZW_NULL && ZW_Right_Boundary != ZW_NULL)
    {
        for(ZW_i = 0; ZW_i < ZW_ImgSize_H; ZW_i++)
        {
            ZW_Putchar(*(ZW_Left_Boundary + ZW_i));
        }
        for(ZW_i = 0; ZW_i < ZW_ImgSize_H; ZW_i++)
        {
            ZW_Putchar(*(ZW_Right_Boundary + ZW_i));
        }
    }
}

void XK_init(void)
{
    uart_init(UART_0,2000000,UART0_TX_P14_0,UART0_RX_P14_1);
}

void VOFA_init(void)
{
    uart_init(UART_0,115200,UART0_TX_P14_0,UART0_RX_P14_1);
}

void XK_uart(void)
{
/*XKͼ������*/
     ImageSideTransform(ImageSide,LeftSide,RightSide);//ͼ����ʾ�߽�
     uart_write_byte(UART_0,0x00);uart_write_byte(UART_0,0xff);uart_write_byte(UART_0,0x01);uart_write_byte(UART_0,0x01);
     ZW_Send_Image(Bin_Image);//��ֵ��
     ZW_Send_Boundary(LeftSide,RightSide);
     uart_write_byte(UART_0,0x01);uart_write_byte(UART_0,0x01);uart_write_byte(UART_0,0xff);uart_write_byte(UART_0,0x00);
}




