/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          zf_device_virtual_oscilloscope
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.4
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_device_virtual_oscilloscope.h"


uint8 virtual_scope_data[10];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      CRC校验（内部使用，用户无需关心）
//  @param      Buf             需要进行CRC计算的数据地址
//  @param      CRC_CNT         需要进行CRC计算的数据个数
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint16 CRC_CHECK(uint8 *Buf, uint8 CRC_CNT)
{
    uint16 CRC_Temp;
    uint8 i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      虚拟示波器数据转换函数
//  @param      data1       要发送的第一个数据
//  @param      data2       要发送的第二个数据
//  @param      data3       要发送的第三个数据
//  @param      data4       要发送的第四个数据
//  @param      *dat        转换之后存放数据的地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void data_conversion(int16 data1, int16 data2, int16 data3, int16 data4, uint8 *dat)
{
    uint16 CRC16 = 0;

    dat[0] = (uint8)((uint16)data1&0xff);
    dat[1] = (uint8)((uint16)data1>>8);

    dat[2] = (uint8)((uint16)data2&0xff);
    dat[3] = (uint8)((uint16)data2>>8);

    dat[4] = (uint8)((uint16)data3&0xff);
    dat[5] = (uint8)((uint16)data3>>8);

    dat[6] = (uint8)((uint16)data4&0xff);
    dat[7] = (uint8)((uint16)data4>>8);

    CRC16  = CRC_CHECK(dat,8);
    dat[8] = (uint8)(CRC16&0xff);
    dat[9] = (uint8)(CRC16>>8);

    //uart_putbuff(UART_0,dat,10);  //数据转换完成后，使用串口发送将数组的内容发送出去
}
