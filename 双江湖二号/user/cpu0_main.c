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
* 文件名称          cpu0_main
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
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设


// **************************** 代码区域 ****************************
/***********************外设资料*****************************
 舵机接口P33_9

 电机接口
 gpio右  P21_4
 gpio左  P21_2
 pwm右   P21_5
 pwm左   P21_3

 编码器接口
 左 P02_8 P00_9
 右 P33_7 P33_6

 XK图传
 TX14_0   RX14_1

************************************************************/

void Init(void)
{
    camera_Init();//初始化相机
    mt9v03x_init();//初始化摄像头
    mt9v03x_set_exposure_time(ExposureTime);//设置曝光时间
    while(!mt9v03x_finish_flag);
//    oled_init();//初始化OLED显示屏
    ips114_init();
    steer_init();
    Pid_Init();
    motor_init();
    gpt_Init();
    key_Init();
    dl1a_init();//初始化TOF传感器
    gpio_init(BEEP, GPO, 0, GPO_PUSH_PULL);
//    gpt_count_init();
    VOFA_init();
//    XK_init();
}

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    Init();

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕

    while(TRUE)
    {
        gpt_get();
        if(LeftGo)//左出库
        {
            Depart(1);
            if(Car)
            break;
        }
        else if(RightGo)//右出库
        {
            Depart(2);
            if(Car)
            break;
        }
        else if(Go)//直接发车
        {
            Car=1;
            break;
        }
        else
        {
            key_scan();//按键扫描
            Keystroke_Depart();
        }
    }
    pit_init(CCU60_CH0, 5000);//电机控制
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
                Test_CAMERA();//图像处理
//        printf("%d,%d,%d,%d\n",gpt[0],gpt[1],gptL_Cir,gptR_Cir);
//        pwm_set_duty(S_MOTOR_PIN,613);
//        motor_control(2000, 2000);
//        VOFA_gpt();
//        oled_show_int(0, 0, 1, 3);//测试oled是否有效
//        gpt_get();
//                ips114_show_int(0, 0, gpt[0], 3);//测试oled是否有效
//                ips114_show_int(0, 20, gpt[1], 3);//测试oled是否有效
//                ips114_show_float(0, 40, gDir_MidFilter*1500, 3,3);
//                ips114_show_int(0, 100, BlackNumAll, 3);
        // 此处编写需要循环执行的代码
    }
}

#pragma section all restore


// **************************** 代码区域 ****************************
