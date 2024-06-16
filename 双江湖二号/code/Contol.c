/*
 * Contol.c
 *
 *  Created on: 2023年9月17日
 *      Author: huawei
 */

#include "Contol.h"


float gDir_Mid=0;//单次偏差
float gDir_MidFilter=0;//总偏差
float gDir_Midlast=0;//上次偏差
int angle=650;

int curvature_mid;                          //动态前瞻值

int speed_out=0;
uint8 Wdisten=155;//后轮两轮的轮距
float ack[5]={1.25,3.6,4.2};//阿克曼系数1.25

uint8 ZebraStop=0;//斑马线停车标志位
int speed_baselast=0;
int speed_basenow=0;
int speed_base=120;//基础速度
uint8 SpeedFactor=7;
int SetSpeed=190;


uint8 Car=0;

uint8 BlackNum=0;
uint8 BlackNumLast=0;
uint8 BlackNumAll=0;
uint8 DecsionEndLine=0;

int curvature[11][3]=
{
    {32,34,26},
    {30,32,34},
    {28,30,32},
    {26,28,30},
    {23,25,27},
    {22,24,26},
    {21,23,25},
    {20,22,24},
    {16,18,20},
    {15,16,17},
    {12,14,16},
};

void bias_curvature(void)
{
    gDir_MidFilter=one_curvature((MidLine[curvature[0][0]]+MidLine[curvature[0][1]]+MidLine[curvature[0][2]])/3,256);
}

void bias_curvature2(void)
{
    curvature_mid = Dynamic_foreisight();
//    ips114_show_uint(0, 0, curvature_mid, 3);
    if(RampFlag!=1||RampFlag!=2)
    {
      Anchorsite(curvature,curvature_mid,2,1);
    }
    gDir_Mid=one_curvature((MidLine[curvature[1][0]]+MidLine[curvature[1][1]]*3+MidLine[curvature[1][2]]*6)/10,256);
    gDir_MidFilter=gDir_Mid*0.8+gDir_Midlast*0.2;
    gDir_Midlast=gDir_Mid;
}

void steer_allcontrol(void)
{
    if(!Elements_flag)
    {
     if(fabs(gDir_MidFilter*1500)<0.7&&BlackNumAll>37)
     {
       SteerPid.Kp=36-(3-fabs(gDir_MidFilter*1500))*4.0;
       SteerPid.Kd=43;
     }
     else
     {
       SteerPid.Kp=36;
       SteerPid.Kd=32;
     }
    }
     angle=calculate_pid(gDir_MidFilter*1500);
     steer_control(angle);

}

void outgoing(void)
{
    speed_out = ( Wdisten * ack[1]/2) * speed_base * gDir_MidFilter;
    if(speed_out>50)
    {
        speed_out=50;
    }
    if(speed_out<-50)
    {
        speed_out=-50;
    }
}

/*
左负右正
*/
float one_curvature(int x1, int y1)
{
    float K;
    int l = x1-(LCDW-1)/2;
    int h = y1-(LCDH-1);
    K = (float)2 * l / (l*l+h*h);
    return K;
}

void gDir_Filter(void)
{
    static float MidDir[5];
    MidDir[4]=MidDir[3];
    MidDir[3]=MidDir[2];
    MidDir[2]=MidDir[1];
    MidDir[1]=MidDir[0];
    MidDir[0]=gDir_Mid;

    gDir_MidFilter=(MidDir[0]+MidDir[1]+MidDir[2]+MidDir[3]+MidDir[4])/5;
}

void SpeedDecision(void)
{
    if(!Elements_flag)//不在元素内 速度决策
    {
            BlackNum=whitehang(Bin_Image,&DecsionEndLine);
            BlackNumAll=0.8*BlackNum+0.2*BlackNumLast;
            BlackNumLast=BlackNum;
            if(BlackNumAll>40)
            {
                BlackNumAll=40;
            }
            speed_base=SetSpeed-abs(BlackNumAll-40)*SpeedFactor;
//            if(BlackNumAll>27)
//            {
//              SteerPid.Kp=38-(BlackNumAll-27)*0.6;
//            }
//            else
//            {
//              SteerPid.Kp=38;
//            }

//            if(BlackNumAll<32)
//            {
//                 SteerPid.Kp=53;//39
//                 SteerPid.Kd=20;//15
//                 Anchorsite(curvature,30,2,2);
//            }
            //            else if(BlackNumAll<30)
            //            {
            //                SteerPid.Kp=53;//39
            //                SteerPid.Kd=20;//15
            //                Anchorsite(curvature,26,2,2);
            //            }
            //            else if(BlackNumAll<=33)
            //            {
            //                SteerPid.Kp=53;//39
            //                SteerPid.Kd=20;//15
            //                Anchorsite(curvature,30,2,2);
            //            }
            //            else
            //            {
            //                SteerPid.Kp=40;
            //                SteerPid.Kd=19;
            //                Anchorsite(curvature,30,2,2);
            //            }
            if(speed_base<155)
            {
                speed_base=155;
            }
    }
}

//void SpeedDecision(void)
//{
//    if(!Elements_flag)//不在元素内 速度决策
//    {
//        BlackNum=whitehang(Bin_Image,&DecsionEndLine);
//        BlackNumAll=0.7*BlackNum+0.3*BlackNumLast;
//        BlackNumLast=BlackNum;
//        speed_basenow=SetSpeed-abs(BlackNumAll-40)*SpeedFactor;
//        speed_base=speed_basenow*0.8+speed_baselast*0.2;
//        speed_baselast=speed_basenow;
//        if(speed_base<120)
//        {
//               speed_base=120;
//        }
//    }
//}

int Dynamic_foreisight()
{
    static float distance_basic;//动态前瞻值//35
    if(RoundFlag>0&&RoundFlag<12&&RoundFlag!=10)
    {
        distance_basic=32;
    }
    else
    {
        distance_basic=36;
    }
    static float target_x = 50.0;
    static float target_y = 58.0;
    float distance;      //实际距离
    float Xdistance;     //X方向上的距离平方
    float Ydistance;     //Y方向上的距离平方
    int y;
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
    return 22;
}




