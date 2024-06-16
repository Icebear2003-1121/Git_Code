 /*
 * key.c
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */



#include "key.h"
//拨码开关状态变量
uint8 sw1_status;
uint8 sw2_status;
uint8 sw3_status;
uint8 sw4_status;

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;
uint8 key5_status = 1;
uint8 key6_status = 1;

uint8 KEY0 = 1;
uint8 KEY1 = 1;
uint8 KEY2 = 1;
uint8 KEY3 = 1;
uint8 KEY4 = 1;
uint8 KEY5 = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;
uint8 key5_last_status;
uint8 key6_last_status;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;
uint8 key5_flag;
uint8 key6_flag;

#define PARAMETER_N   910  //240
#define PARAMETER_N_1 120
uint16 dianya;
uint16 parameter;
uint16 parameter_1;

uint8  keyup_data=0x00;      //按键抬起返回值
uint8  key_tem=0x00;         //长按的按键值与按键扫描程序过渡变量
uint8  key_bak=0x00;         //按键扫描程序过渡变量
uint16 key_time=0x00;        //按键按下之后的时间计数，该值乘以扫描一次按键函数的时间就等于按键按下的时间
uint8  keydown_data=0x00;    //按键按下后就返回的值


void key_Init(void)
{
    //按钮初始化
    //按键初始化
    gpio_init(KEY01,GPI,0,GPI_PULL_UP);
    gpio_init(KEY02,GPI,0,GPI_PULL_UP);
    gpio_init(KEY03,GPI,0,GPI_PULL_UP);
    gpio_init(KEY04,GPI,0,GPI_PULL_UP);
    gpio_init(KEY05,GPI,0,GPI_PULL_UP);
    gpio_init(KEY06,GPI,0,GPI_PULL_UP);
    //拨码开关初始化
    gpio_init(SW1,GPI,0,GPI_PULL_UP);
    gpio_init(SW2,GPI,0,GPI_PULL_UP);
    gpio_init(SW3,GPI,0,GPI_PULL_UP);
    gpio_init(SW4,GPI,0,GPI_PULL_UP);

    //获取拨码开关状态
    sw1_status = gpio_get_level(SW1);
    sw2_status = gpio_get_level(SW2);
    sw3_status = gpio_get_level(SW3);
    sw4_status = gpio_get_level(SW4);
}

void Key_Scan_caidan(void)
{
    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
    uint8 key_num = 0;
#ifdef oled
    oled_show_string(20, 0, "Welcome to XkCar");
    oled_show_string(0, 1, "direction");
    oled_show_string(0, 3, "parameter");
    oled_show_string(0, 5, "SpeedMode");

    oled_show_string(80, 1, "Key1");
    oled_show_string(80, 3, "Key2");
    oled_show_string(80, 5, "Key3");
#endif

#ifdef IPS200
    ips200_show_string(100, 0, "Welcome to XkCar");
    ips200_show_string(10, 30, "direction");
    ips200_show_string(10, 60, "parameter");
    ips200_show_string(10, 90, "SpeedMode");
    ips200_show_string(10, 120, "show");


    ips200_show_string(120, 30, "Key1");
    ips200_show_string(120, 60, "Key2");
    ips200_show_string(120, 90, "Key3");
    ips200_show_string(120, 120, "Key4");
#endif

    while(1)
    {
        key_scan(0);
        if(keyup_data==1)
          key_num = 1;
        if(keyup_data==2)
          key_num = 2;
        if(keyup_data==3)
          key_num = 3;
        if(keyup_data==4)
          key_num = 4;
        if(keyup_data==5)
          key_num = 5;
        if(keyup_data==6)
          key_num = 6;
//        //保存按键状态
//        key1_last = key1;
//        key2_last = key2;
//        key3_last = key3;
//        key4_last = key4;
//        key5_last = key5;
//        key6_last = key6;
//
//        //读取当前按键状态
//        key1 = READ_GPIO(KEY01);
//        key2 = READ_GPIO(KEY02);
//        key3 = READ_GPIO(KEY03);
//        key4 = READ_GPIO(KEY04);
//        key5 = READ_GPIO(KEY05);
//        key6 = READ_GPIO(KEY06);
//
//
//        if((key1 && !key1_last)|| (key1 == 0 &&  key1_last == 0))
//            key_num = 1;
//        if((key2 && !key2_last)|| (key2 == 0 &&  key2_last == 0))
//            key_num = 2;
//        if((key3 && !key3_last)|| (key3 == 0 &&  key3_last == 0))
//            key_num = 3;
//        if((key4 && !key4_last)|| (key4 == 0 &&  key4_last == 0))
//            key_num = 4;
//        if((key5 && !key5_last)|| (key5 == 0 &&  key5_last == 0))
//            key_num = 5;
//        if((key6 && !key6_last)|| (key6 == 0 &&  key6_last == 0))
//            key_num = 6;

//        switch(key_num)
//        {
//            case 6:
//                break;
//            case 5:
//                break;
//            case 4:
//                ips200_clear();
//                Key_Scan_show();
//                break;
//            case 3:
////                oled_clear();
//                ips200_clear();
//                Key_Scan_SpeedMode();
////                key_num=0;
//                break;
//            case 2:
////                oled_clear();
//                ips200_clear();
//                Key_Scan_canshu();
////                key_num=0;
//                break;
//            case 1:
////                oled_clear();
//                ips200_clear();
//                Key_Scan_direction();
////                key_num=0;
//                break;
//        }
        if(key_num==1)
        {
            ips200_clear();
            Key_Scan_direction();
        }
        else if(key_num==2){
            ips200_clear();
            Key_Scan_canshu();
        }
        else if(key_num==3){
            ips200_clear();
            Key_Scan_SpeedMode();
        }
        else if(key_num==4){

            ips200_clear();
            Key_Scan_show();
        }
        else if(key_num==6){
            ips200_clear();//清屏\");
            system_delay_ms(500);  //延时
            break;
        }

//        if(flag.adjust_ok==1)
//       {
//
////        ips200_clear();//清屏\");
////        system_delay_ms(500);  //延时
//        break;
//
//       }
//        ips200_clear();
        key_num=0;
        ips200_show_string(100, 0, "Welcome to XkCar");
        ips200_show_string(10, 30, "direction");
        ips200_show_string(10, 60, "parameter");
        ips200_show_string(10, 90, "SpeedMode");
        ips200_show_string(10, 120, "show");


        ips200_show_string(120, 30, "Key1");
        ips200_show_string(120, 60, "Key2");
        ips200_show_string(120, 90, "Key3");
        ips200_show_string(120, 120, "Key4");

//        if(key_num)
//        {
//            break;
//        }


}
}
//#ifdef IPS200
//void Key_Scan_SpeedMode(void)
//{
//    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
//    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
//    uint8 key_num = 0;
//    while(1)
//    {
//
//
//    }
//}
//#endif
//#ifdef oled
void Key_Scan_SpeedMode(void)
{
    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
    uint8 key_num = 0;
#ifdef oled
    oled_show_string(60, 7, "Mode");
#endif
#ifdef IPS200
//    ips200_show_string(50, 0, "Mode");
#endif
    while(1)
       {

           //保存按键状态
        key_scan(0);
        if(keyup_data==1)
         key_num = 1;
        if(keyup_data==2)
         key_num = 2;
        if(keyup_data==3)
         key_num = 3;
        if(keyup_data==4)
         key_num = 4;
        if(keyup_data==5)
         key_num = 5;
        if(keyup_data==6)
         key_num = 6;

           sw1_status = READ_GPIO(SW1);
           sw2_status = READ_GPIO(SW2);
           sw3_status = READ_GPIO(SW3);
           sw4_status = READ_GPIO(SW4);

           if(!sw1_status && !sw2_status && !sw3_status && !sw4_status)  // 0000 ->MOde0
           {
               SpeedMode = 0;
           }
           if(sw1_status && !sw2_status && !sw3_status && !sw4_status)  // 1000 ->MOde1
           {
               SpeedMode = 1;
           }
           if(!sw1_status && sw2_status && !sw3_status && !sw4_status)  // 0100 ->MOde2
           {
               SpeedMode = 2;
           }
           if(sw1_status && sw2_status && !sw3_status && !sw4_status)  // 1100 ->MOde3
           {
               SpeedMode = 3;
           }
           if(!sw1_status && sw2_status && !sw3_status && sw4_status)  // 0101 ->MOde4
           {
               SpeedMode = 4;
           }
           if(sw1_status && !sw2_status && sw3_status && !sw4_status)  // 1010 ->MOde5 0010 0011
           {
               SpeedMode = 5;
           }
#ifdef IPS200
//           ips200_clear();

           ips200_show_string(10,30,"SpeedMode");
           ips200_show_int(120,30,SpeedMode,2);
//           if(key_num == 5)
//           {
//               flag.adjust_ok=1;
//
//               ips200_clear();//清屏\");
//               break;
//           }
//
           if(key_num == 6)
           {
               flag.adjust_ok=0;
               ips200_clear();//清屏\");
               system_delay_ms(500);  //延时
               break;
           }

#endif
#ifdef oled
           //显示游标
           oled_clear();

           oled_show_string(10, 1, "SpeedMode");
           oled_show_int(80,1,SpeedMode,2);

           if(key_num == 5)
             {
                 flag.adjust_ok=1;

                 oled_clear();//清屏\");
                 break;
             }

           if(key_num == 5)
             {
                 flag.adjust_ok=1;
                 oled_clear();//清屏\");
                 system_delay_ms(500);  //延时
                 break;
             }
#endif
               //键值清零


               key_num = 0;


}
}
void Key_Scan_direction(void)
{
    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
    uint8 key_num = 0;
//    oled_show_string(60, 7, "direction");
//    ips200_show_string(10,30,"barrierDir");
//    ips200_show_int(120,30,flag.barrierDir,2);
//
//    ips200_show_string(10,60,"LeftOut");
//    ips200_show_int(120,60,Xk.LeftOut,2);
//
//    ips200_show_string(10,90,"RightOut");
//    ips200_show_int(120,90,Xk.RightOut,2);
    while(1)
       {

           //保存按键状态
        key_scan(0);
        if(keyup_data==1)
         key_num = 1;
        if(keyup_data==2)
         key_num = 2;
        if(keyup_data==3)
         key_num = 3;
        if(keyup_data==4)
         key_num = 4;
        if(keyup_data==5)
          key_num = 5;
        if(keyup_data==6)
          key_num = 6;

//           sw1_status = READ_GPIO(SW1);
//           sw2_status = READ_GPIO(SW2);
//           sw3_status = READ_GPIO(SW3);
//           sw4_status = READ_GPIO(SW4);
        switch(key_num)
        {
            case 1:flag.barrierDir = 1; break;

            case 2:flag.barrierDir = 0; break;

            case 3:Xk.LeftOut  = 1;
                   Xk.RightOut = 0;break;

            case 4:Xk.LeftOut  = 0;
                   Xk.RightOut = 1;break;
            case 5:Xk.StrGo=1;break;

        }
//           if(key_num==1)//0001
//           {
//               flag.barrierDir = 1;
//           }
////           else //if(!sw1_status&&!sw2_status&&sw3_status&&sw4_status)//0011
////           {
////               flag.barrierDir = 0;
////           }
//           if(key_num==2)//0010
//           {
//               Xk.LeftOut  = 1;
//               Xk.RightOut = 0;
//           }
//           else //if(!sw1_status&&sw2_status&&sw3_status&&sw4_status)//0111
//           {
//               Xk.RightOut = 1;
//               Xk.LeftOut  = 0;
//           }
//           if(sw3_status&&!sw2_status&&!sw1_status&&!sw4_status)
//           {
//               show_tuxiang = 1;
//           }
//           else
//           {
//               show_tuxiang = 0;
//           }
#ifdef IPS200
//           ips200_clear();
           ips200_show_string(10,30,"barrierDir");
           ips200_show_int(120,30,flag.barrierDir,2);

           ips200_show_string(10,60,"LeftOut");
           ips200_show_int(120,60,Xk.LeftOut,2);

           ips200_show_string(10,90,"RightOut");
           ips200_show_int(120,90,Xk.RightOut,2);

           ips200_show_string(10,120,"StrGo");
           ips200_show_int(120,120,Xk.StrGo,2);
//           ips200_show_string(10, 120, "show_tuxiang");
//           ips200_show_float(120,120,show_tuxiang,3,2);

#endif
#ifdef oled                      //显示游标
           oled_clear();

           oled_show_string(10, 1, "barrierDir");
           oled_show_float(80,1,flag.barrierDir,3,2);

           oled_show_string(10, 2, "LeftOut");
           oled_show_float(80,2,Xk.LeftOut,3,2);

           oled_show_string(10, 3, "RightOut");
           oled_show_float(80,3,Xk.RightOut,3,2);

           oled_show_string(10, 4, "StrGo");
           oled_show_float(80,4,Xk.StrGo,3,2);

//           oled_show_string(10, 4, "show_tuxiang");
//           oled_show_float(80,4,show_tuxiang,3,2);

#endif
//           if(key_num == 6)
//             {
//               Key_Scan_SpeedMode();
//               break;
//             }
               //键值清零

               if(key_num == 6)
               {
                   flag.adjust_ok=0;
                   ips200_clear();//清屏\");
                   system_delay_ms(500);  //延时
                   break;
               }
               key_num = 0;

}
}
//#endif

void Key_Scan_canshu(void)
{
    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
    uint8 key_num = 0;

    //显示当前参数
    ips200_show_string(10,   0, "Speedpara");
    ips200_show_int(100, 0, (parameter/30), 2);

    //方向外环pid参数
    ips200_show_string(10,    30, "SpeedMode");
    ips200_show_float(100, 30, SpeedMode, 5, 2);

    ips200_show_string(10,    60, "CurPid_Ki");
    ips200_show_float(100, 60, CurPid.Ki, 3, 2);

    ips200_show_string(10,    90, "CurPid_Kd");
    ips200_show_float(100, 90, CurPid.Kd, 3, 2);

    //方向内环pid参数
    ips200_show_string(10,    120, "In_Kp");
    ips200_show_float(100, 120, JSDPid.Kp, 3, 2);

    ips200_show_string(10,    150, "In_Ki");
    ips200_show_float(100, 150, JSDPid.Ki, 3, 2);

    ips200_show_string(10,    180, "In_Kd");
    ips200_show_float(100, 180, JSDPid.Kd, 3, 2);

    //速度环pid参数
//    ips200_show_string(10,    210, "S_Kp");
//    ips200_show_float(100, 210, SpeedPID.Kp, 3, 2);
//
//    ips200_show_string(10,    240, "S_Ki");
//    ips200_show_float(100, 240, SpeedPID.Ki, 3, 2);
//
//    ips200_show_string(10,    270, "S_Kd");
//    ips200_show_float(100, 270, SpeedPID.Kd, 3, 2);

    //目标速度
    ips200_show_string(10,    300, "speed_base");
    ips200_show_int(100, 300, speed_base,6);

    while(1)
    {
        static unsigned int key6_count = 0;

//        //保存按键状态
//        key1_last = key1;
//        key2_last = key2;
//        key3_last = key3;
//        key4_last = key4;
//        key5_last = key5;
//        key6_last = key6;
//
//              //读取当前按键状态
//        key1 = READ_GPIO(KEY01);
//        key2 = READ_GPIO(KEY02);
//        key3 = READ_GPIO(KEY03);
//        key4 = READ_GPIO(KEY04);
//        key5 = READ_GPIO(KEY05);
//        key6 = READ_GPIO(KEY06);
//
//        if((key1 && !key1_last)|| (key1 == 0 &&  key1_last == 0))
//            key_num = 1;
//        if((key2 && !key2_last)|| (key2 == 0 &&  key2_last == 0))
//            key_num = 2;
//        if(key3 && !key3_last)//|| (key3 == 0 &&  key3_last == 0))
//            key_num = 3;
//        if(key4 && !key4_last)//|| (key4 == 0 &&  key4_last == 0))
//            key_num = 4;
//        if(key5 && !key5_last)//|| (key5 == 0 &&  key5_last == 0))
//            key_num = 5;
//        if(key6 && !key6_last)//|| (key6 == 0 &&  key6_last == 0))
//            key_num = 6;
            key_scan(0);
            if(keyup_data==1)
              key_num = 1;
            if(keyup_data==2)
              key_num = 2;
            if(keyup_data==3)
              key_num = 3;
            if(keyup_data==4)
              key_num = 4;
            if(keyup_data==5)
              key_num = 5;
            if(keyup_data==6)
              key_num = 6;

//        if(key6==0)    key6_count++;//长按

        //根据键值选择对应功能
        switch(key_num)
        {
            case 0://无
                break;
            case 6:
                flag.adjust_ok=1;
                break;
            case 5:
                parameter += 300;
                parameter = (parameter <= PARAMETER_N) ? parameter : 0;
                break;
            case 4:
                parameter -= 30;
                parameter = (parameter >= 0) ? parameter : 0;
                key6_count = 0;
                break;
            case 3:
                parameter += 30;
                parameter = (parameter <= PARAMETER_N) ? parameter : 0;
                break;
            case 2:
                switch(parameter/30)
                {
                    case 1:ElemforeisightSet      +=0.5;     break;
                    case 2:foreisightSet          +=0.5;     break;
                    case 3:BarrierIn              +=0.05;     break;
                    case 4:BarrierOut             +=0.05;     break;
                    case 5:ZebraIn1               +=0.05;     break;
                    case 6:ZebraIn2               +=0.05;     break;
                    case 7:Barriergpt             +=1;     break;
                    case 8:RoadOut_flag    +=1;     break;
                    case 9:                         break;
                    case 10:                        break;

                    case 11: CirSpeed     += 1;       break;
                    case 12: strightSpeed += 1;       break;
                    case 13: ElementSpeed += 1;       break;
                    case 14: GptPidR.Kp+=0.5;       break;
                    case 15: GptPidR.Ki+=0.1;       break;
                    case 16: GptPidR.Kd+=0.1;       break;
                    case 17: Xk.LeftOut+=1;         break;
                    case 18: Xk.RightOut+=1;        break;
                    case 19: Xk.StrGo+=1;           break;
                    case 20:                        break;

                    case 21:CurPid.Kp   += 0.1;     break;
                    case 22:CurPid.Ki   += 0.1;     break;
                    case 23:CurPid.Kd   += 0.1;     break;
                    case 24:JSDPid.Kp   += 0.1;     break;
                    case 25:JSDPid.Ki   += 0.005;   break;
                    case 26:JSDPid.Kd   += 0.1;     break;
                    case 27:Xk.LeftOut  += 1;       break;
                    case 28:Xk.RightOut += 1;       break;
                    case 29:Xk.StrGo    += 1;       break;
                    case 30:                        break;
                }
                break;
            case 1:
                switch(parameter/30)
                {
                    case 1:ElemforeisightSet      -=0.5;     break;
                    case 2:foreisightSet          -=0.5;     break;
                    case 3:BarrierIn              -=0.05;     break;
                    case 4:BarrierOut             -=0.05;     break;
                    case 5:ZebraIn1               -=0.05;     break;
                    case 6:ZebraIn2               -=0.05;     break;
                    case 7:Barriergpt             -=1;     break;
                    case 8:RoadOut_flag    -=1;     break;
                    case 9:                         break;
                    case 10:                        break;

                    case 11: CirSpeed     -= 1;       break;
                    case 12: strightSpeed -= 1;       break;
                    case 13: ElementSpeed -= 1;       break;
                    case 14: GptPidR.Kp-=0.5;       break;
                    case 15: GptPidR.Ki-=0.1;       break;
                    case 16: GptPidR.Kd-=0.1;       break;
                    case 17: Xk.LeftOut-=1;         break;
                    case 18: Xk.RightOut-=1;        break;
                    case 19: Xk.StrGo-=1;           break;
                    case 20:                        break;

                    case 21:CurPid.Kp   -= 0.1;     break;
                    case 22:CurPid.Ki   -= 0.1;     break;
                    case 23:CurPid.Kd   -= 0.1;     break;
                    case 24:JSDPid.Kp   -= 0.1;     break;
                    case 25:JSDPid.Ki   -= 0.005;   break;
                    case 26:JSDPid.Kd   -= 0.1;     break;
                    case 27:Xk.LeftOut  -= 1;       break;
                    case 28:Xk.RightOut -= 1;       break;
                    case 29:Xk.StrGo    -= 1;       break;
                    case 30:                        break;

                }
                break;
        }
        //显示当前参数
        if(key_num)
        {
          if(parameter<=300)
          {
            //显示游标
            ips200_clear();
            ips200_show_string(0, parameter, ">");

            //显示调节后的参数
             //显示当前参数
            // ips200_show_string(10,   0, "para");
             //ips200_show_int(100, 0, (parameter), 6);
             ips200_show_string(10,   0, ">>>>Statege-Select-Set<<<<");


             //方向环pid参数
             ips200_show_string(10,    30, "ElemforeisightSet");
             ips200_show_float(160, 30, ElemforeisightSet, 5, 2);

             ips200_show_string(10,    60, "foreisightSet");
             ips200_show_float(160,  60, foreisightSet, 6,1);

             ips200_show_string(10,    90, "BarrierIn");
             ips200_show_float(160, 90, BarrierIn, 3, 1);

             //速度环pid参数
             ips200_show_string(10,    120, "BarrierOut");
             ips200_show_float(160, 120, BarrierOut, 2, 1);

             ips200_show_string(10,    150, "ZebraIn1");
             ips200_show_float(160, 150, ZebraIn1, 2, 1);

             ips200_show_string(10,    180, "ZebraIn2");
             ips200_show_float(160, 180, ZebraIn2, 2, 1);

             ips200_show_string(10,    210, "Barriergpt");
             ips200_show_int(160, 210, Barriergpt, 3);

             ips200_show_string(10,    240, "RoadOut_flag");
             ips200_show_float(160, 240, RoadOut_flag, 5, 1);

//             ips200_show_string(10,    270, "bizhangset");
//             ips200_show_float(160, 270, bizhangset, 3, 2);
//
             ips200_show_string(10,    300, "Welocme  to Xk SmartCar");
//             ips200_show_int(160, 300, bizhangertime,6);
          }
          else if(parameter>300 && parameter<=600)
          {
            //显示游标
            ips200_clear();
            ips200_show_string(0,  ((parameter-300)), ">");
            //显示调节后的参数
                   //显示当前参数
                   //ips200_show_string(10,   0, "para");
                   //ips200_show_int(100, 0, (parameter), 6);
                   ips200_show_string(10,   0, ">>>>Element-Set<<<<");

//                //   SpeedPID_l.Kp=20;
                   //方向环pid参数
                   ips200_show_string(10,    30, "CirSpeed;");
                   ips200_show_float(140, 30, CirSpeed, 5, 2);

                   ips200_show_string(10,    60, "strightSpeed");
                   ips200_show_float(140, 60, strightSpeed, 3, 2);

                   ips200_show_string(10,    90, "ElementSpeed");
                   ips200_show_float(140, 90, ElementSpeed, 3, 2);

                   //速度环pid参数
                   ips200_show_string(10,    120, "GptPidR.Kp");
                   ips200_show_float(140, 120, GptPidR.Kp, 3, 2);

                   ips200_show_string(10,    150, "GptPidR.Ki");
                   ips200_show_float(140, 150, GptPidR.Ki, 3, 2);

                   ips200_show_string(10,    180, "GptPidR.Kd");
                   ips200_show_float(100, 180, GptPidR.Kd, 3, 2);

//                   ips200_show_string(10,    210, "out_duoyu");
//                   ips200_show_float(100, 210, out_duoyu, 3, 2);
//
//                   ips200_show_string(10,    240, "XXXX");
//                   ips200_show_float(100, 240, 666, 3, 2);
//
//                   ips200_show_string(10,    270, "XXXX");
//                   ips200_show_float(100, 270, SpeedPID.Kd, 3, 2);
//
                   ips200_show_string(10,    300, "Welocme  to Xk SmartCar");
//                   ips200_show_int(100, 300, 666,6);



          }
        else if(parameter>600 && parameter<=900)
        {
          //显示游标
          ips200_clear();
          ips200_show_string(0,  ((parameter-600)), ">");

          //显示调节后的参数
                //显示当前参数
                ips200_show_string(10,   0, ">>>>PID---Set<<<<");
              //  ips200_show_int(100, 0, (parameter), 6);

                //方向环pid参数
                ips200_show_string(10,    30, "CurPid.Kp");
                ips200_show_float(100, 30, CurPid.Kp, 5, 2);

                ips200_show_string(10,    60, "CurPid.Ki");
                ips200_show_float(100, 60, CurPid.Ki, 3, 2);

                ips200_show_string(10,    90, "CurPid.Kd");
                ips200_show_float(100, 90, CurPid.Kd, 3, 2);

                //速度环pid参数
                ips200_show_string(10,    120, "JSDPid.Kp");
                ips200_show_float(100, 120, JSDPid.Kp, 3, 2);

                ips200_show_string(10,    150, "JSDPid.Ki");
                ips200_show_float(100, 150, JSDPid.Ki, 3, 2);

                ips200_show_string(10,    180, "JSDPid.Kd");
                ips200_show_float(100, 180, JSDPid.Kd, 3, 2);

                ips200_show_string(10,    210, "Xk.LeftOut");
                ips200_show_float(100, 210, Xk.LeftOut, 3, 2);

                ips200_show_string(10,    240, "Xk.RightOut");
                ips200_show_float(100, 240, Xk.RightOut, 3, 2);

                ips200_show_string(10,    270, "Xk.StrGo");
                ips200_show_float(100, 270, Xk.StrGo, 3, 2);
//
//                //目标速度
//                ips200_show_string(10,    300, "speed_aim");
                ips200_show_string(10,    300, "Welocme  to Xk SmartCar");
//                ips200_show_int(100, 300, speed_aim,6);

        }


        }
        if (flag.adjust_ok)
          {
            flag.adjust_ok=0;
            parameter = 0;
            ips200_clear();//清屏
            ips200_show_string(0,   0, "parament adjust OK!");
            ips200_show_string(50,  16*2, "\\          //");
            ips200_show_string(50,  16*4, "  =========  ");
            ips200_show_string(50,  16*5, "  L-U-C-K-Y  ");
            ips200_show_string(50,  16*6, "  =========  ");
            ips200_show_string(50,  16*8, "//         \\");
            system_delay_ms(1000);  //延时
            break;
        }

            //键值清零
            key_num = 0;

        //调参完成跳出循环

    }

    //显示速度策略
    ips200_clear();//清屏
    system_delay_ms(20);  //延时
    ips200_clear();  //清屏
}


void Key_Scan_show(void)
{
    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
    uint8 key_num = 0;

    ips200_show_string(10,   0, "elec");
    ips200_show_string(100,   0, "Key1");

    ips200_show_string(10,   30, "tuxiang");
    ips200_show_string(100,   30, "Key2");


    while(1)
    {
        int imgshow,elecshow;


               key_scan(0);
               if(keyup_data==1)
                 key_num = 1;
               if(keyup_data==2)
                 key_num = 2;
//               if(keyup_data==3)
//                 key_num = 3;
//               if(keyup_data==4)
//                 key_num = 4;
//               if(keyup_data==5)
//                 key_num = 5;
               if(keyup_data==6)
                 key_num = 6;

              if(key_num==2)
              {
                  if(!elecshow)
                  {
                      elecshow = 1;
                      ips200_clear();
                  }
                  while(1){

                      Get_Use_Image1(*mt9v03x_image,Image_Use);//图像压缩
                      Get_Bin_Image(0);//得到二值化
                      Bin_Image_Filter();//去噪
                      ImageGetSide(Bin_Image,ImageSide,RoadLostFlag);//得到边界
                      ElementsJudge();//元素判断
                      ImageGetMidLine(ImageSide,MidLine);//得到中线
                      ips200_show_gray_image(0, 0, Bin_Image[0], COLW, COLH,COLW,COLH,0);

                      key_scan(0);
                      if(keyup_data==6)
                        key_num = 6;
                      if(key_num==6)
                      {
                           ips200_clear();//清屏\");
                           key_num=0;
                           break;
                      }


                  }
              }
              else if(key_num==1)
              {
                  if(!imgshow)
                    {
                        imgshow = 1;
                        ips200_clear();
                    }
                  while(1)
                  {
                      ips200_show_string(0, parameter_1, ">");

                      ips200_show_string(10,30, "elec_value[0]");
                      ips200_show_int(150, 30, elec_value[0],4);

                      ips200_show_string(10, 60, "elec_value[1]");
                      ips200_show_int(150, 60,elec_value[1], 4);

                      ips200_show_string(10,90, "elec_data[0]");
                      ips200_show_int(150, 90, elec_data[0], 4);

                      ips200_show_string(10,120, "elec_data[1]");
                      ips200_show_int(150, 120, elec_data[1], 4);

                      key_scan(0);
                      if(keyup_data==6)
                        key_num = 6;
                      if(key_num==6)
                      {
                           ips200_clear();//清屏\");
                           key_num=0;
                           break;
                      }


              }
              }
              else if(key_num == 6)
              {
                  flag.adjust_ok =0;
                  key_num=0;
                  parameter_1=0;
                  ips200_clear();//清屏\");
                  system_delay_ms(500);  //延时
                  break;
              }


              ips200_show_string(10,   0, "elec");
              ips200_show_string(100,   0, "Key1");

              ips200_show_string(10,   30, "tuxiang");
              ips200_show_string(100,   30, "Key2");
        //调参完成跳出循环

        }



    //显示速度策略
    ips200_clear();//清屏
    system_delay_ms(20);  //延时
    ips200_clear();  //清屏

}

void Key_scan_CurPid()
{
//    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
//    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
    uint8 key_num = 0;
    while(1)
    {
        static unsigned int key6_count = 0;
        oled_show_string(0, 1, "CurPid.Kp");
        oled_show_string(0, 3, "CurPid.Ki");
        oled_show_string(0, 5, "CurPid.Kd");
        oled_show_float(80, 1, CurPid.Kp,3,2);
        oled_show_float(80, 3, CurPid.Ki,3,2);
        oled_show_float(80, 5, CurPid.Kd,3,2);
        oled_show_string(0, 7, "para");
        oled_show_int(80,7, (parameter/30), 3);
        //保存按键状态
//       key1_last = key1;
//       key2_last = key2;
//       key3_last = key3;
//       key4_last = key4;
//       key5_last = key5;
//       key6_last = key6;
//
//             //读取当前按键状态
//       key1 = READ_GPIO(KEY01);
//       key2 = READ_GPIO(KEY02);
//       key3 = READ_GPIO(KEY03);
//       key4 = READ_GPIO(KEY04);
//       key5 = READ_GPIO(KEY05);
//       key6 = READ_GPIO(KEY06);

//      if((key1 && !key1_last)|| (key1 == 0 &&  key1_last == 0))
//          key_num = 1;
//      if((key2 && !key2_last)|| (key2 == 0 &&  key2_last == 0))
//          key_num = 2;
//      if((key3 && !key3_last)|| (key3 == 0 &&  key3_last == 0))
//          key_num = 3;
//      if((key4 && !key4_last)|| (key4 == 0 &&  key4_last == 0))
//          key_num = 4;
//      if((key5 && !key5_last)|| (key5 == 0 &&  key5_last == 0))
//          key_num = 5;
//      if((key6 && !key6_last)|| (key6 == 0 &&  key6_last == 0))
//          key_num = 6;
        key_scan(1);
        if(keyup_data==1)
            key_num = 1;
        if(keyup_data==2)
            key_num = 2;
        if(keyup_data==3)
            key_num = 3;
        if(keyup_data==4)
            key_num = 4;
        if(keyup_data==5)
            key_num = 5;
        if(keyup_data==6)
            key_num = 6;


       switch(key_num)
       {
           case 0://无
               break;
           case 5:
               parameter += 300;
               parameter = (parameter <= PARAMETER_N) ? parameter : 0;
               break;
           case 4:
               parameter -= 30;
               parameter = (parameter >= 0) ? parameter : 0;
               key6_count = 0;
               break;
           case 3:
               parameter += 30;
               parameter = (parameter <= PARAMETER_N) ? parameter : 0;
               break;
           case 2:
               switch(parameter/30)
               {
                   case 1:CurPid.Kp   += 0.1;     break;
                   case 2:CurPid.Ki   += 0.1;     break;
                   case 3:CurPid.Kd   += 0.1;     break;

               }
               break;
           case 1:
               switch(parameter/30)
               {   case 1:CurPid.Kp   -= 0.1;     break;
                   case 2:CurPid.Ki   -= 0.1;     break;
                   case 3:CurPid.Kd   -= 0.1;     break;
               }
               break;
       }
       oled_show_float(80, 1, CurPid.Kp,3,2);
      oled_show_float(80, 3, CurPid.Ki,3,2);
      oled_show_float(80, 5, CurPid.Kd,3,2);
    }
}

void key_scan(uint8 mode)
{
//    uint8 key1 = 1,       key2 = 1,       key3 = 1,       key4 = 1,       key5 = 1,       key6 = 1;
//    uint8 key1_last = 1,  key2_last = 1,  key3_last = 1,  key4_last = 1,  key5_last = 1,  key6_last = 1;
//    uint8 key_num = 0;
    //保存按键状态
    key1_last_status = key1_status;
    key2_last_status = key2_status;
    key3_last_status = key3_status;
    key4_last_status = key4_status;
    key5_last_status = key5_status;
    key6_last_status = key6_status;

          //读取当前按键状态
    key1_status = READ_GPIO(KEY01);
    key2_status = READ_GPIO(KEY02);
    key3_status = READ_GPIO(KEY03);
    key4_status = READ_GPIO(KEY04);
    key5_status = READ_GPIO(KEY05);
    key6_status = READ_GPIO(KEY06);

     keyup_data=0;         //键抬起后按键值一次有效

    if(key1_status==0||key2_status==0||key3_status==0||key4_status==0||key5_status==0||key6_status==0)   //有键正按下
    {
        if(key1_status==0)      key_tem=1;
        else if(key2_status==0) key_tem=2;
        else if(key3_status==0) key_tem=3;
        else if(key4_status==0) key_tem=4;
        else if(key5_status==0) key_tem=5;
        else if(key6_status==0) key_tem=6;


        if (key_tem == key_bak)      //有键按下后第一次扫描不处理，与else配合第二次扫描有效，这样实现了去抖动
        {
           key_time++;             //有键按下后执行一次扫描函数，该变量加1
           keydown_data=key_tem;   //按键值赋予keydown_data

           if( (mode==0)&&(key_time>1) )//key_time>1按键值无效，这就是单按，如果mode为1就为连按
            keydown_data=0;
        }
        else                             //去抖动
        {
           key_time=0;
           key_bak=key_tem;
        }

    }
    else       //键抬起
    {
        if(key_time>2)            //按键抬起后返回一次按键值
        {
            keyup_data=key_tem;  //键抬起后按键值赋予keydown_data
        }
        key_bak=0;               //要清零，不然下次执行扫描程序时按键的值跟上次按的值一样，就没有去抖动处理了
        key_time=0;
        keydown_data=0;
  }
}

////蓝牙传输
//void Datasend(void)
//{
//    if(1)
//    {
//        long  send_data[6];
//        send_data[0] = break_speed1;           //10~12ms
//        send_data[1] = speed_aim;  //2ms
//        send_data[2] = Direct_Last;  //2ms
//        send_data[3] = Turn_PWM;
//        send_data[4] = l_speed_now;
//        send_data[5] = r_speed_now;
////        send_data[6] = speed_aim;
//       // send_data[7] = Theory_pwm;
//      //  send_data[8] = Direct_Last;
//
//        Data_Send(UART_User, send_data);
//    }
////    else
////    {
////        long  send_data[9];
////        send_data[0] = offset;
////        send_data[1] = outer_ring_kp;//SD_kp;inner_ring_kp;offset;Theory_pwm;
////        send_data[2] = Turn_PWM;
////        send_data[3] = icm20602_gyro_z;//icm20602_gyro_z;
////        send_data[4] = inner_ring_kp;//l_speed_now;break_speed
////        send_data[5] = inner_ring_ki;
////       // send_data[6] = inner_ring_kd;
////       // send_data[7] = Direct_Last;
////      //  send_data[8] = Theory_pwm;
////
////        Data_Send(UART_User, send_data);//只能传6个，9个会有问题
////    }
//}
