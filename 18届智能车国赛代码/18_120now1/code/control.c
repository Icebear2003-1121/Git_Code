/*
 * control.c
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
 */

#include "control.h"
extern uint8 Elements_flag;


float foreisightSet = 0;                      //ǰհ�趨����ֵ
float ElemforeisightSet = 0;                  //Բ��ǰհ�趨����ֵ
int foreisightOutSet = 0;                   //��ʵǰհ�趨����ֵ
int curvature_mid;                          //��̬ǰհֵ
int SteerVelocity_pwm = 0;                  //�����������Pwm
int Direct_Last = 0;                        //����˲���һ�η������Pwmֵ
int Direct_Last_Last = 0;                   //����˲�����һ�η������Pwmֵ
int AucPwm = 0;                             //���򻷲��ٶȻ����Pwmֵ
int Lspeed_base=0;                          //�����趨�����ٶ�
int Rspeed_base=0;                          //�������趨�����ٶ�
int speed_base = 130;                       //�趨�����ٶ�
int strategic_mode = 0;                     //�����ã��ٶȿ��Ʒ�ʽ(0����ͷ 1���)
int speed_out=0;                            //����
int Gyroz_transition;                       //ƫ����ʵ������ֵ
int offset_1=0;                             //����ͷ����õ���ƫ��
float gDir_MidSpeedUp=0;                    //ֱ������ʱ�õ���ƫ��
float offset=0;                             //����ͷ����õ���ƫ��
float gDir_Mid=0;                           //��������ƫ��
float gyro_p=1;                             //�Ƕ�ǰ��ϵ��
float CurpSet = 0;                          //����p�趨����ֵ
float CuriSet = 0;                          //����p�趨����ֵ
float CurdSet = 0;                          //����p�趨����ֵ
float BarrierIn = 0;                        //��·�ϽǶ�
float BarrierOut = 0;                       //��·�ϽǶ�
float ZebraIn1 = 0;                          //�������Ƕ�
float ZebraIn2 = 0;                          //�������Ƕ�
uint8 ZebraStop=0;                          //������ͣ����־λ
uint8 Speedlow=0;                           //������ٱ�־λ
uint8 SpeedUp=0;                            //���ٱ�־λ
int motorR_Set=0,motorL_Set=0;              //������ռ�ձ�
int Barriergpt = 0;
int tofdistance = 0;
int CirSpeed = 0;
int strightSpeed = 0;
int ElementSpeed = 0;

void SpeedMode_Init(void)
{
    foreisightSet = 0;
    ElemforeisightSet = -2;
    foreisightOutSet = 0;
    CurpSet = 0;
    CuriSet = 0;
    CurdSet = 0;
    BarrierIn = 1.95;
    BarrierOut = -0.9;
    ZebraIn1 =  2.9;
    ZebraIn2 =  5.1;
    tofdistance = 16;
    Barriergpt = 40;
    CirSpeed = 150;
    strightSpeed = 170;
    ElementSpeed = 150;
}
int curvature[10][3]= //ǰհ����
{
    {24,26,28},    /*320 [0][0],[0][1],[0][2]*/
    {23,25,27},
    {22,24,26},

    {20,22,24},    /*420 [1][0],[1][1],[1][2]*/
    {16,18,20},    /*480 [1][0],[1][1],[1][2]*/
    {15,16,17},
    {12,14,16},
};

int curvatureUp[10][3]=
{
    {24,26,28},    /*320 [0][0],[0][1],[0][2]*/
    {23,25,27},
    {22,24,26},

    {20,22,24},    /*420 [1][0],[1][1],[1][2]*/
    {16,18,20},    /*480 [1][0],[1][1],[1][2]*/
    {15,16,17},
    {12,14,16},
};
//  @brief     ����ǰհ��
//  @param      type  0��MT9V03X_W/2  1��MT9V03X_W/2 -30  2��MT9V03X_W/2 +30
//  @return     void
//  Sample usage:    QianZhang(1);
//-------------------------------------------------------------------------------------------------------------------
uint8 curvature_m;
uint8 curvature_l;
uint8 curvature_r;
void curvatureAll(uint8 type)
{
    if(type == 0)
    {
        curvature_m = 0;
        for(uint8 y = COLH -1; y > 1; y--)
        {
            if(Bin_Image[y][COLW/2] != 0)
            {
                curvature_m++;
            }
            else
            {
              break;
            }
        }
    }
    else if(type == 1)
    {
        curvature_l = 0;
        for(uint8 y = COLH -1; y > 1; y--)
        {
            if(Bin_Image[y][COLW/2 -30] != 0)
            {
                curvature_l++;
            }
            else
            {
              break;
            }
        }
    }
    else if(type == 2)
    {
        curvature_r = 0;
        for(uint8 y = COLH -1; y > 1; y--)
        {
            if(Bin_Image[y][COLW/2 +30] != 0)
            {
                curvature_r++;
            }
            else
            {
              break;
            }
        }
    }
}
/*
��̬ǰհ����

*/
int Dynamic_foreisight()
{
    static float distance_basic = 32.0;
    static float target_x = 50.0;
    static float target_y = 58.0;
    float distance;      //ʵ�ʾ���
    float Xdistance;     //X�����ϵľ���ƽ��
    float Ydistance;     //Y�����ϵľ���ƽ��
    int y;

    if(!Elements_flag )
    {
        if(SpeedMode == 0 )
          {
             distance_basic = 30;
          }
          if(SpeedMode == 1)
          {
              distance_basic = 20;
          }
          if(SpeedMode == 2)
          {
              distance_basic = 26;
          }
          if(SpeedMode == 3)
          {
              distance_basic = 35;
          }
          if(SpeedMode == 4)
          {
              distance_basic = 34;
          }
          if(SpeedMode == 5)
          {
              distance_basic = 34;
          }

    }
    else
    {
        if(SpeedMode == 0 )
          {
             distance_basic = 32;
          }
          if(SpeedMode == 1)
          {
              distance_basic = 20;
          }
          if(SpeedMode == 2)
          {
              distance_basic = 26;
          }
          if(SpeedMode == 3)
          {
              distance_basic = 34;
          }
          if(SpeedMode == 4)
         {
              distance_basic = 34;

         }
          if(SpeedMode == 5)
         {
             distance_basic = 35;
         }
    }
    for(y = 55;y > 0;y--)
    {
        Xdistance = (target_x - MidLine[y]) * (target_x - MidLine[y]);
        Ydistance = (target_y - y) * (target_y - y);
        distance = 1/Q_rsqrt((Xdistance + Ydistance));
        if(distance >= distance_basic)
        {
            if(y>40)y=38;
            return y;
        }
    }
    return 20;
}
/*
 ��ǰհ����
a:ǰհ����
cur:�м�������
step:�����������
mode:�ٶȾ����ǵ��ٶ�ģʽ
 ��
Anchorsite(curvature,25,2,1);
Anchorsite(curvatureUp,22,2,3);
*/
void Anchorsite(int a[][3],int cur,int step,uint8 mode)
{
    a[mode][0]=cur-step;
    a[mode][1]=cur;
    a[mode][2]=cur+step;
}

/*---------------------------------------------------------------
 ����    ����bias_curvature
 ����    �ܡ�����ǰհ��ָerror
 ����    ����
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void bias_curvature(void)
{
    if(!SpeedUp)//��ǰת��˵��ǰհ̫С��Ҳ���Ǿ��복Զ�ˣ���֮
    {
        curvature_mid = Dynamic_foreisight();
        if(!Elements_flag)
        {
        Anchorsite(curvature,curvature_mid+foreisightSet,2,2);
        gDir_Mid=one_curvature((MidLine[curvature[2][0]]+MidLine[curvature[2][1]]+MidLine[curvature[2][2]])/3,200);
        }
        else
        {
            Anchorsite(curvature,curvature_mid+ElemforeisightSet,2,2);
            gDir_Mid=one_curvature((MidLine[curvature[2][0]]+MidLine[curvature[2][1]]*3+MidLine[curvature[2][2]]*6)/10,200);
        }
//        gDir_Mid=one_curvature((MidLine[curvature[2][0]]+MidLine[curvature[2][1]]+MidLine[curvature[2][2]])/3,200);//155 2
        //gDir_Mid=one_curvature((MidLine[curvature[2][0]]+MidLine[curvature[2][1]]*3+MidLine[curvature[2][2]]*6)/10,200);
        gDir_MidSpeedUp=one_curvature((MidLine[curvatureUp[5][0]]+MidLine[curvatureUp[5][1]]+MidLine[curvatureUp[5][2]])/3,200);
    }
    else
    {
        gDir_Mid=one_curvature((MidLine[curvature[3][0]]+MidLine[curvature[3][1]]+MidLine[curvature[3][2]])/3,200.0);
        gDir_MidSpeedUp=one_curvature((MidLine[curvatureUp[4][0]]+MidLine[curvatureUp[4][1]]+MidLine[curvatureUp[4][2]])/3,200);
    }
}


void SpeedControl(unsigned char mode)
{
    if(mode ==0)
    {
    if(!Elements_flag )//Ԫ���ٶȿ���
    {
        if(!road_type.Ramp)
        {
            if(OpenRoadFlag)
            {
                speed_base = 130;
                FPIDChang(GptPidR,30,5,0);
                FPIDChang(GptPidL,30,5,0);
                FPIDChang(JSDPid,1.3,0,1.5);
            }
            else if(gDir_Mid * 1000 < -0.5)
            {
                Speedlow = 1;
                speed_base=135;
                FPIDChang(CurPid,23,0,15.2);
                FPIDChang(GptPidL,30,5,0);
                FPIDChang(GptPidR,30,5,0);
                FPIDChang(JSDPid,1.6,0,2.2);
            }
            else if(gDir_Mid * 1000 > 0.5)
            {
                Speedlow = 1;
                speed_base=135;
                FPIDChang(CurPid,23,0,15.2);
                 FPIDChang(GptPidL,30,5,0);
                 FPIDChang(GptPidR,30,5,0);
                 FPIDChang(JSDPid,1.6,0,2.2);
            }
            else if(dl1a_distance_mm<1400)
            {
                speed_base = 100;
            }
            else if(gDir_Mid * 1000 > -0.5 && gDir_Mid * 1000 < 0.5)
            {
                speed_base=160;
                FPIDChang(CurPid,15,0,19.2);
                FPIDChang(GptPidL,30,5,0);
                FPIDChang(GptPidR,30,5,0);
                FPIDChang(JSDPid,1.6,0,2);
                Speedlow = 0;
            }
//            else if(ZebraFlag)
//            {
//                speed_base = -50;
//            }
        }
    }
    else if(Elements_flag)
    {
        speed_base=150;
        FPIDChang(CurPid,19,0,15.2);
        FPIDChang(GptPidL,30,5,0);
        FPIDChang(GptPidR,30,5,0);
        FPIDChang(JSDPid,2.3,0,1.7);
    }
    }
    if(mode ==1)
       {
           if(!road_type.Ramp)
           {
           curvature_mid = Dynamic_foreisight();
           Anchorsite(curvature,curvature_mid,2,2);
           speed_base = 120;
           FPIDChang(CurPid,15,0,19.2);
           FPIDChang(GptPidL,30,7,0);
           FPIDChang(GptPidR,30,7,0);
           FPIDChang(JSDPid,0.8,0,1.14);
           if(OpenRoadFlag)
               {
                   speed_base = 120;
                   FPIDChang(ElecPid,17,0,6);
                   FPIDChang(GptPidR,30,7,0);
                   FPIDChang(GptPidL,30,7,0);
                   FPIDChang(JSDPid,1.3,0,1.5);
               }
           if(Elements_flag)
           {
               curvature_mid = Dynamic_foreisight();
               Anchorsite(curvature,curvature_mid-4,2,2);
               FPIDChang(JSDPid,1.7,0,0.7);
           }
       }
       }
    if(mode ==2)
        {
        if(!road_type.Ramp)
                {
                    speed_base = 130;
                    curvature_mid = 31;
                    Anchorsite(curvature,curvature_mid,1,2);
                    FPIDChang(CurPid,15,0,19.2);
                    FPIDChang(GptPidL,10,2.5,0);
                    FPIDChang(GptPidR,10,2.5,0);
                    FPIDChang(JSDPid,0.5,0,1.3);
                    if(OpenRoadFlag)
                    {
                        speed_base = 100;
                        FPIDChang(GptPidL,10,2.5,0);
                        FPIDChang(GptPidR,10,2.5,0);
                        FPIDChang(JSDPid,1.3,0,1.5);
                    }
//                    else if(dl1a_distance_mm<1400)
//                    {
//                        speed_base = 120;
//                    }
                }
        if(Elements_flag)
                   {
                        curvature_mid = Dynamic_foreisight();
                        Anchorsite(curvature,32,2,2);
                        FPIDChang(CurPid,15,0,19.2);
                        FPIDChang(GptPidL,10,2.5,0);
                        FPIDChang(GptPidR,10,2.5,0);
                        FPIDChang(JSDPid,1.3,0,1.4);
                   }
        if(ZebraFlag)
                {
                    speed_base = 80;
                }
        }
    if(mode ==3)
    {
    if(!Elements_flag )//Ԫ���ٶȿ���
    {
        if(!road_type.Ramp)
        {
             if(OpenRoadFlag)
           {
               FPIDChang(GptPidR,30,5,0);
               FPIDChang(GptPidL,30,5,0);
               FPIDChang(JSDPid,1.5,0,1.5);
           }
            else if(gDir_Mid * 1000 < -0.5)
            {
                Speedlow = 1;
                speed_base=150;
                FPIDChang(CurPid,23,0,15.2);
                FPIDChang(GptPidL,30,5,0);
                FPIDChang(GptPidR,30,5,0);
                FPIDChang(JSDPid,1.6,0,2.2);
            }
            else if(gDir_Mid * 1000 > 0.5)
            {
                Speedlow = 1;
                speed_base=150;
                FPIDChang(CurPid,23,0,15.2);
                 FPIDChang(GptPidL,30,5,0);
                 FPIDChang(GptPidR,30,5,0);
                 FPIDChang(JSDPid,1.6,0,2.2);
            }

            else if(dl1a_distance_mm<1400)
            {
                speed_base = 100;
            }
            else if(gDir_Mid * 1000 > -0.5 && gDir_Mid * 1000 < 0.5)
            {
                speed_base=160;
                FPIDChang(CurPid,23,0,19.2);
                FPIDChang(GptPidL,30,5,0);
                FPIDChang(GptPidR,30,5,0);
                FPIDChang(JSDPid,1.6,0,2);
                Speedlow = 0;
            }
        }
    }
    else if(Elements_flag)
    {
        speed_base=150;
        FPIDChang(CurPid,19,0,25.2);
        FPIDChang(GptPidL,30,5,0);
        FPIDChang(GptPidR,30,5,0);
        FPIDChang(JSDPid,1.3,0,1.5);
    }

}
    if(mode ==4)
        {
        BarrierIn = 1.95;
        BarrierOut = -0.9;
        if(!Elements_flag )//Ԫ���ٶȿ���
        {
            if(!road_type.Ramp)
            {
//                if(ZebraFlag)
//                {
//                    speed_base = 10;
//                }
                if(OpenRoadFlag)
            {
                    speed_base = 130;
                    FPIDChang(GptPidR,30,5,0);
                    FPIDChang(GptPidL,30,5,0);
                    FPIDChang(JSDPid,1.3,0,1.5);
            }
                else if(dl1a_distance_mm<1400)
                {
                    speed_base = 100;
                }
            else if(gDir_Mid * 1000 < -0.5)
                {
                    Speedlow = 1;
                    speed_base=150;
                    FPIDChang(CurPid,33,0,15.2);
                    FPIDChang(GptPidL,30,5,0);
                    FPIDChang(GptPidR,30,5,0);
                    FPIDChang(JSDPid,1.6,0,2.7);
                }
                else if(gDir_Mid * 1000 > 0.5)
                {
                    Speedlow = 1;
                    speed_base=150;
                    FPIDChang(CurPid,33,0,15.2);
                     FPIDChang(GptPidL,30,5,0);
                     FPIDChang(GptPidR,30,5,0);
                     FPIDChang(JSDPid,1.6,0,2.7);
                }
                else if(gDir_Mid * 1000 >= -0.5 && gDir_Mid * 1000 <= 0.5)
                {
                    speed_base=170;
                    FPIDChang(CurPid,15,0,19.2);
                    FPIDChang(GptPidL,30,5,0);
                    FPIDChang(GptPidR,30,5,0);
                    FPIDChang(JSDPid,1.6,0,2);
                    Speedlow = 0;
                }

            }
        }
        else if(Elements_flag)
        {
            speed_base=150;
            FPIDChang(CurPid,19,0,25.2);
            FPIDChang(GptPidL,30,5,0);
            FPIDChang(GptPidR,30,5,0);
            FPIDChang(JSDPid,1.7,0,1.2);
        }

    }
    if(mode ==5)
           {
           if(!Elements_flag )//Ԫ���ٶȿ���
           {
               if(!road_type.Ramp)
               {
                   if(OpenRoadFlag)
               {
                   FPIDChang(GptPidR,30,5,0);
                   FPIDChang(GptPidL,30,5,0);
                   FPIDChang(JSDPid,1.3,0,1.5);
               }
                   else if(gDir_Mid * 1000 < -0.5)
                   {
                       Speedlow = 1;
                       speed_base=160;
                       FPIDChang(CurPid,23,0,17.2);
                       FPIDChang(GptPidL,30,5,0);
                       FPIDChang(GptPidR,30,5,0);
                       FPIDChang(JSDPid,1.8,0,2.7);
                   }
                   else if(gDir_Mid * 1000 > 0.5)
                   {
                       Speedlow = 1;
                       speed_base=160;
                       FPIDChang(CurPid,23,0,17.2);
                        FPIDChang(GptPidL,30,5,0);
                        FPIDChang(GptPidR,30,5,0);
                        FPIDChang(JSDPid,1.8,0,2.7);
                   }

                   else if(dl1a_distance_mm<1400)
                   {
                       speed_base = 100;
                   }
                   else if(gDir_Mid * 1000 > -0.5 && gDir_Mid * 1000 < 0.5)
                   {
                       speed_base=180;
                       FPIDChang(CurPid,15,0,20.2);
                       FPIDChang(GptPidL,30,5,0);
                       FPIDChang(GptPidR,30,5,0);
                       FPIDChang(JSDPid,1.7,0,2.2);
                       Speedlow = 0;
                   }
               }
           }
           else if(Elements_flag)
           {
               speed_base=160;
               FPIDChang(CurPid,19,0,25.2);
               FPIDChang(GptPidL,30,5,0);
               FPIDChang(GptPidR,30,5,0);
               FPIDChang(JSDPid,1.7,0,1.2);
           }

       }
    if(mode ==6)
               {
               if(!Elements_flag )//Ԫ���ٶȿ���
               {
                   if(!road_type.Ramp)
                   {
                       if(OpenRoadFlag)
                   {
                       speed_base = 130;
                       FPIDChang(GptPidR,30,5,0);
                       FPIDChang(GptPidL,30,5,0);
                       FPIDChang(JSDPid,1.3,0,1.5);
                   }
                       else if(gDir_Mid * 1000 < -0.5)
                       {
                           Speedlow = 1;
                           speed_base=CirSpeed;
                           FPIDChang(CurPid,23,0,17.2);
                           FPIDChang(GptPidL,30,5,0);
                           FPIDChang(GptPidR,30,5,0);
                           FPIDChang(JSDPid,1.8,0,2.7);
                       }
                       else if(gDir_Mid * 1000 > 0.5)
                       {
                           Speedlow = 1;
                           speed_base=CirSpeed;
                           FPIDChang(CurPid,23,0,17.2);
                            FPIDChang(GptPidL,30,5,0);
                            FPIDChang(GptPidR,30,5,0);
                            FPIDChang(JSDPid,1.8,0,2.7);
                       }

                       else if(dl1a_distance_mm<1400)
                       {
                           speed_base = 100;
                       }
                       else if(gDir_Mid * 1000 > -0.5 && gDir_Mid * 1000 < 0.5)
                       {
                           speed_base=strightSpeed;
                           FPIDChang(CurPid,15,0,20.2);
                           FPIDChang(GptPidL,30,5,0);
                           FPIDChang(GptPidR,30,5,0);
                           FPIDChang(JSDPid,1.7,0,2.2);
                           Speedlow = 0;
                       }
                   }
               }
               else if(Elements_flag)
               {
                   speed_base=ElementSpeed;
                   FPIDChang(CurPid,19,0,25.2);
                   FPIDChang(GptPidL,30,5,0);
                   FPIDChang(GptPidR,30,5,0);
                   FPIDChang(JSDPid,1.7,0,1.2);
               }

           }
}

void motor_allcontrol(void)
{
    gpt_get();
    if(flag.start && !RoadOut_flag)//flag.start && !RoadOut_flag  flag.start&& (!RoadOut_flag || !flag.stop)
    {
#ifdef Control_bing
//     ����
        motorR_Set=motorR_pid(speed_base,(gpt[1]+gpt[0])/2)+speed_out;
        motorL_Set=motorL_pid(speed_base,(gpt[1]+gpt[0])/2)-speed_out;
        if(motorR_Set>7000)
               {
                   motorR_Set=7000;
               }
               else if(motorR_Set<-7000)
               {
                   motorR_Set=-7000;
               }

               if(motorL_Set>7000)
               {
                   motorL_Set=7000;
               }
               else if(motorL_Set<-7000)
               {
                   motorL_Set=-7000;
               }

               if(motorR_Set>0)
               {
                   gpio_set_level(MOTORR_DIR, 0);
               }
               else
               {
                   gpio_set_level(MOTORR_DIR, 1);
                   motorR_Set=-motorR_Set;
               }

               if(motorL_Set>0)
               {
                   gpio_set_level(MOTORL_DIR, 0);
               }
               else
               {
                  gpio_set_level(MOTORL_DIR, 1);
                  motorL_Set=-motorL_Set;
               }
        motor_control(motorR_Set,motorL_Set);
#endif

#ifdef Control_chuan
//     ����
        motorR_Set=motorR_pid(speed_base+speed_out,gpt[0]);
        motorL_Set=motorL_pid(speed_base-speed_out,gpt[1]);
        motor_control(motorR_Set,motorL_Set);
#endif

#ifdef Control_jiao
        /*���ٶȻ�*/
        Gyroz_transition = imu660ra_gyro_transition(imu660ra_gyro_z)*1;               //��ԭʼֵת��Ϊʵ������ֵ
        SteerVelocity_pwm = JSD_pid(imu660ra_gyro_z,speed_out);         //���ٶȻ�pid
        AucPwm = Direct_Last_Last * 0.04 + Direct_Last * 0.1 + SteerVelocity_pwm * 0.86;
        motorR_Set=motorR_pid(speed_base,(gpt[0]+gpt[1])/2)+AucPwm;
        motorL_Set=motorL_pid(speed_base,(gpt[0]+gpt[1])/2)-AucPwm ;
        Direct_Last_Last = Direct_Last;
        Direct_Last = AucPwm;
        if(motorR_Set>PwmMax)
          {
              motorR_Set=PwmMax;
          }
          else if(motorR_Set<-PwmMax)
          {
              motorR_Set=-PwmMax;
          }

          if(motorL_Set>PwmMax)
          {
              motorL_Set=PwmMax;
          }
          else if(motorL_Set<-PwmMax)
          {
              motorL_Set=-PwmMax;
          }


       if(motorR_Set>0)
       {
           gpio_set_level(MOTORR_DIR, 0);
       }
       else
       {
           gpio_set_level(MOTORR_DIR, 1);
           motorR_Set=-motorR_Set;
       }

       if(motorL_Set>0)
       {
           gpio_set_level(MOTORL_DIR, 0);
       }
       else
       {
          gpio_set_level(MOTORL_DIR, 1);
          motorL_Set=-motorL_Set;
       }
       motor_control(motorR_Set,motorL_Set);

#endif

    }
    else if(RoadOut_flag)//!RoadOut_flag  flag.stop
    {
//        ips200_show_string(150, 150, "Out");
        motor_control(motorRStop_pid(0,gpt[0]),motorLStop_pid(0,gpt[1]));//��ͬ��ֱ��ֹͣ�ٶȸ�0
    }
}

/*
 ���ټ���
 */
void DirectionControl(void)
{
    if(!OpenRoadFlag)//!OpenRoadFlag
    {//�����gDir_Mid * n,�����n�ǵ�������һ��ϵ��
   if(!strategic_mode)
   {
        speed_out = calculate_pid(gDir_Mid*1000);
   }
   else
        speed_out =elec_pid(difference_h*0.26);
    }
    else if (OpenRoadFlag)
    {
        speed_out=elec_pid(difference_h*0.26);
    }
    if(BarrierFlag)
        {
            switch(BarrierFlag)
            {
                {
                            case 3:
                                speed_out=Gyro_pid(yaw,BarrierIn);
                                break;
                            case 5:
                                speed_out=0;//Gyro_pid(yaw,BarrierIn);
                                break;
                            case 7:
                                speed_out=Gyro_pid(yaw,BarrierOut);
                                break;
                            case 9:
                                speed_out=calculate_pid(gDir_Mid*1000);
                                break;

                            case 4:
                                speed_out=Gyro_pid(yaw,-BarrierIn);
                                break;
                            case 6:
                                speed_out=0;//Gyro_pid(yaw,-BarrierIn);
                                break;
                            case 8:
                                speed_out=Gyro_pid(yaw,-BarrierOut);
                                break;
                            case 10:
                                speed_out=calculate_pid(gDir_Mid*1000);
                                break;
                        }

            }
        }

        if(ZebraFlag)
        {

                if(SpeedMode == 0 || SpeedMode == 3|| SpeedMode == 4 || SpeedMode == 5 || SpeedMode == 6)
                {
                    switch(ZebraFlag)
                                {
                case 21:
                    speed_out=Gyro_pid(yaw,ZebraIn2);
                    break;
                case 25:
                    speed_out=0;//Gyro_pid(yaw,BarrierIn);
                    break;
                case 22:
                    speed_out=Gyro_pid(yaw,-ZebraIn2);
                    break;
                case 26:
                    speed_out=0;
                    break;
                }
                }
                else
                {
                    switch(ZebraFlag)
                                {
                case 21:
                    speed_out=Gyro_pid(yaw,ZebraIn1);
                    break;
                case 25:
                    speed_out=0;//Gyro_pid(yaw,BarrierIn);
                    break;
                case 22:
                    speed_out=Gyro_pid(yaw,-ZebraIn1);
                    break;
                case 26:
                    speed_out=0;
                    break;
                                }
                }
            }
        }

/*
���ʼ���
������
*/
float one_curvature(int x1, int y1)
{
    float K;
    int l = x1-(COLW-1)/2;
    int h = y1-(COLH-1);

    K = (float)2 * l / (l*l+h*h);
    return K;
}

/*
 �޷�����
 */
int range_protect(int duty,int min,int max)
{
 if(duty >= max)
 {
     return max;
 }
 if(duty <= min)
 {
     return min;
 }
 else
 {
     return duty;
 }
}
/*
  ƫ�����
 */
//void Calculate_Offset()
//{
//    offset_1 = 0;
//    for (uint8 y = COLH - 1;y >= 10;y--)
//    {
//
//
//    }
//



//}
//void gDir_Filter(void)
//{
//    static float MidDir[5];
//    MidDir[4]=MidDir[3];
//    MidDir[3]=MidDir[2];
//    MidDir[2]=MidDir[1];
//    MidDir[1]=MidDir[0];
//    MidDir[0]=gDir_Mid;
//
//    gDir_MidFilter=(MidDir[0]+MidDir[1]+MidDir[2]+MidDir[3]+MidDir[4])/5;
//}



