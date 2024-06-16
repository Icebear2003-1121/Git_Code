/*
 * pid.c
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */

#include "pid.h"
//速度环用的是增量式pid，方向环是位置式pid
struct FPID CurPid;//当前pid
struct FPID ElecPid;
struct FPID GptPidR;
struct FPID GryoPid;
struct FPID ZebraPid;
struct FPID GptPidL;
struct FPID PidRStop;
struct FPID PidLStop;
struct FPID JSDPid;
#define DT 0.01     //采样时间
float dSpeed = 0;   //速度设定值
float Angle_Set=0;
//控制器参数设置
float kp    = 1;
float kd    = 0.1;
float b0    = 1;
float ar    = 1;
float cr    = 10;
float h     = 2;

//ADRC变量初始化
float z1    = 0;
float z2    = 0;
float y     = 0;
float ey    = 0;
float ero   = 0;
float dero  = 0;
float x1    = 0;
float adrc  = 0;

float ADRC_speed_control(float speed)//因为还没想到怎么adrc加上编码器值，输入参数如果在方向环就为中线偏差值，速度环为
{
    float erro, derro, ddSpeed, rdSpeed, f, u;
    erro    = dSpeed - speed;
    derro   = (erro - ey) / DT;
    ddSpeed = ar * dero + cr * erro;

    f       = b0 * (h * derro + ddSpeed);

    rdSpeed = f + kd * z2;
    z1      = z1 + DT * z2;
    z2      = z2 + DT * (rdSpeed - kd * z2);
    y       = speed + z1;

    ero     = y - dSpeed;
    dero    = (ero - ey) / DT;
    ey      = ero - h * b0 * dero;

    x1      = x1 + DT * (y - adrc);
    adrc    = kp * x1 + y;

    u       = adrc;
    return u;
}

void Pid_Init(void)
{
    CurPid.err=0;
    CurPid.err_last=0;
    CurPid.Kp=24;//155 25 180 25
    CurPid.Ki=0;
    CurPid.Kd=19.2;//155 4 180 8
    CurPid.Kout=0;
    CurPid.voltage=0;
    CurPid.integral=0;

    ElecPid.err=0;
    ElecPid.err_last=0;
    ElecPid.Kp=25;//100 13
    ElecPid.Ki=0;
    ElecPid.Kd=10;//6
    ElecPid.Kout=0;
    ElecPid.voltage=0;
    ElecPid.integral=0;

    GryoPid.err=0;
    GryoPid.err_last=0;
    GryoPid.Kp=25;//25
    GryoPid.Ki=0;
    GryoPid.Kd=10;//90
    GryoPid.Kout=0;
    GryoPid.voltage=0;
    GryoPid.integral=0;

    ZebraPid.err=0;
    ZebraPid.err_last=0;
    ZebraPid.Kp=0.5;//25
    ZebraPid.Ki=0;
    ZebraPid.Kd=0;//90
    ZebraPid.Kout=0;
    ZebraPid.voltage=0;
    ZebraPid.integral=0;


    GptPidR.err=0;
    GptPidR.err_last=0;
    GptPidR.err_last_last=0;
    GptPidR.Kp=30;//10 14//70：15.5，3.6
    GptPidR.Ki=7;//1.5 2.8
    GptPidR.Kd=0;
    GptPidR.Kout=0;
    GptPidR.voltage=0;
    GptPidR.integral=0;

    GptPidL.err=0;
    GptPidL.err_last=0;
    GptPidL.err_last_last=0;
    GptPidL.Kp=30;//22
    GptPidL.Ki=7;//14
    GptPidL.Kd=0;
    GptPidL.Kout=0;
    GptPidL.voltage=0;
    GptPidL.integral=0;

    PidRStop.err=0;
    PidRStop.err_last=0;
    PidRStop.err_last_last=0;
    PidRStop.Kp=30;
    PidRStop.Ki=7;
    PidRStop.Kd=0;
    PidRStop.Kout=0;
    PidRStop.voltage=0;
    PidRStop.integral=0;

    PidLStop.err=0;
    PidLStop.err_last=0;
    PidLStop.err_last_last=0;
    PidLStop.Kp=30;
    PidLStop.Ki=7;
    PidLStop.Kd=0;
    PidLStop.Kout=0;
    PidLStop.voltage=0;
    PidLStop.integral=0;


    JSDPid.err=0;
    JSDPid.err_last=0;
    JSDPid.err_last_last=0;
    JSDPid.Kp=1.5;
    JSDPid.Ki=0;
    JSDPid.Kd=0.8;
    JSDPid.Kout=0;
    JSDPid.voltage=0;
    JSDPid.integral=0;
}

//void PIDChang(struct PID a,int p,int i,int d)
//{
//    a.Kp=p;
//    a.Ki=i;
//    a.Kd=d;
//}

void FPIDChang(struct FPID a,float p,float i,float d)
{
    a.Kp=p;
    a.Ki=i;
    a.Kd=d;
}

int calculate_pid(float a)//摄像头pid
{
    int t;

    CurPid.Set      = 0;
    CurPid.Actual   = a;
    CurPid.err      = CurPid.Set - CurPid.Actual;

    CurPid.integral = CurPid.integral + CurPid.err;

    CurPid.voltage = CurPid.Kp*CurPid.err
                + CurPid.Ki*CurPid.integral/1000
                + CurPid.Kd*(CurPid.err-CurPid.err_last)
                + CurPid.Kout;

    CurPid.err_last = CurPid.err;

    t=CurPid.voltage;

    return t;
}

int Gyro_pid(float a,float b)
{
    int t;

    GryoPid.Set      = b;
    GryoPid.Actual   = a;
    GryoPid.err      = GryoPid.Set - GryoPid.Actual;


    GryoPid.integral = GryoPid.integral + GryoPid.err;

    GryoPid.voltage = GryoPid.Kp*GryoPid.err
                + GryoPid.Ki*GryoPid.integral/1000
                + GryoPid.Kd*(GryoPid.err-GryoPid.err_last)
                + GryoPid.Kout;

    GryoPid.err_last = GryoPid.err;

    t=GryoPid.voltage;

    return t;
}

int Zebra_pid(int a,float b)
{
    int t;

    ZebraPid.Set      = b;
    ZebraPid.Actual   = a;
    ZebraPid.err      = ZebraPid.Set - ZebraPid.Actual;


    ZebraPid.integral = ZebraPid.integral + ZebraPid.err;

    ZebraPid.voltage = ZebraPid.Kp*ZebraPid.err
                + ZebraPid.Ki*ZebraPid.integral/1000
                + ZebraPid.Kd*(ZebraPid.err-ZebraPid.err_last)
                + ZebraPid.Kout;

    ZebraPid.err_last = ZebraPid.err;

    t=ZebraPid.voltage;

    return t;
}

int JSD_pid(int a,int b)
{
       int t;
       int temp_speed;
       float c ;
//       c=1.45;
//
//       JSDPid.Kp=1.3;
//       JSDPid.Kd=1.3;




       if(!OpenRoadFlag && !strategic_mode)
       {
           if(SpeedMode == 0 )
           {
               c = 1.4;
           }
           if(SpeedMode == 1)
           {
               c = 0.6;
           }
           if(SpeedMode == 2)
           {
               c = 0.8;
           }
           if(SpeedMode == 3)
           {
               c = 1.5;
           }
           if(SpeedMode == 4 || SpeedMode == 6)
           {
               c = 1.6;
           }
           if(SpeedMode == 5)
           {
               c = 1.7;
           }
//           JSDPid.Kd=0.15;
       }

       if(Speedlow && !OpenRoadFlag)
       {
           if(SpeedMode == 0 )
              {
                  c = 1.9;
              }
              if(SpeedMode == 3 )
              {
                  c = 2;
              }
              if(SpeedMode == 4)
              {
                  c = 2.4;
              }
              if(SpeedMode == 5  || SpeedMode == 6)
              {
                  c = 2.5;
              }
       }
       if(Elements_flag)
       {
           if(SpeedMode == 0 )
              {
                  c = 0.85;
              }
              if(SpeedMode == 1)
              {
                  c = 0.78;
              }
              if(SpeedMode == 2)
              {
                  c = 0.8;
              }
              if(SpeedMode == 3)
              {
                  c = 0.7;
              }
              if(SpeedMode == 4)
              {
                  c = 0.85;
              }
              if(SpeedMode == 5)
                {
                    c = 0.85;
                }
       }
       else
       {
           if(SpeedMode == 0||SpeedMode == 1||SpeedMode == 2||SpeedMode == 3)
           {
               c =1.35;//1.55
           }
           if(SpeedMode == 4)
          {
              c=1.35;
          }
           if(SpeedMode == 5 || SpeedMode == 6)
           {
               c=1.35;
           }
       }

       temp_speed=break_speed;

       Angle_Set=b*temp_speed*c;//temp_speed*0.02;0.25
//       Angle_Set=(Angle_Set>420)?420:Angle_Set;
       JSDPid.integral = JSDPid.integral + (Angle_Set-a);
       t=c*JSDPid.Kp*(Angle_Set-a)
         +JSDPid.Ki*JSDPid.integral/1000
         +JSDPid.Kd*((b*temp_speed-a)-JSDPid.err_last);//yaw_rate;//Aspeed_kd*((Radius*temp_speed-yaw_rate)-last_error_ANG);//单P
       JSDPid.err_last=b*temp_speed-a;

       return t;

}
int elec_pid(float a)
{
    int t;

    ElecPid.Set      = 0;
    ElecPid.Actual   = a;
    ElecPid.err      = ElecPid.Set - ElecPid.Actual;


    ElecPid.integral = ElecPid.integral + ElecPid.err;

    ElecPid.voltage = ElecPid.Kp*ElecPid.err
                + ElecPid.Ki*ElecPid.integral/1000
                + ElecPid.Kd*(ElecPid.err-ElecPid.err_last)
                + ElecPid.Kout;

    ElecPid.err_last = ElecPid.err;

    t=ElecPid.voltage;

    return t;
}

int motorR_pid(int a,int b)
{
    int t;
    GptPidR.Set      = a;
    GptPidR.Actual   = b;
    GptPidR.err      = GptPidR.Set - GptPidR.Actual;

    GptPidR.integral = GptPidR.integral + GptPidR.err;

    GptPidR.voltage = GptPidR.Kp*(GptPidR.err-GptPidR.err_last)
                + GptPidR.Ki*GptPidR.err
                + GptPidR.Kd*(GptPidR.err-2*GptPidR.err_last+GptPidR.err_last_last)
                + GptPidR.Kout
                + GptPidR.voltage;

    GptPidR.err_last_last = GptPidR.err_last;
    GptPidR.err_last = GptPidR.err;
    t=GptPidR.voltage;
//    if(GptPidR.voltage>6000)//2000
//    {
//        GptPidR.voltage=6000;//2000
//    }
//    else if(GptPidR.voltage<-6000)//-2000
//    {
//        GptPidR.voltage=-6000;//-2000
//    }
////
//    if(GptPidR.voltage>0)
//    {
//        gpio_set_level(MOTORR_DIR, 0);//右轮1为正转
//        t=GptPidR.voltage;
//    }
//    else
//    {
//        gpio_set_level(MOTORR_DIR, 1);
//       t=-GptPidR.voltage;
//    }

    return t;
}

int motorL_pid(int a,int b)
{
    int t;

    GptPidL.Set      = a;
    GptPidL.Actual   = b;
    GptPidL.err      = GptPidL.Set - GptPidL.Actual;

    GptPidL.integral = GptPidL.integral + GptPidL.err;

    GptPidL.voltage = GptPidL.Kp*(GptPidL.err-GptPidL.err_last)
                + GptPidL.Ki*GptPidL.err
                + GptPidL.Kd*(GptPidL.err-2*GptPidL.err_last+GptPidL.err_last_last)
                + GptPidL.Kout
                + GptPidL.voltage;

    GptPidL.err_last_last = GptPidL.err_last;
    GptPidL.err_last = GptPidL.err;
    t=GptPidL.voltage;
//    if(GptPidL.voltage>6000)
//    {
//        GptPidL.voltage=6000;
//    }
//    else if(GptPidL.voltage<-6000)
//    {
//        GptPidL.voltage=-6000;
//    }
//
//    if(GptPidL.voltage>0)
//    {
//        gpio_set_level(MOTORL_DIR, 0);//左轮0为正转
//        t=GptPidL.voltage;
//    }
//    else
//    {
//        gpio_set_level(MOTORL_DIR, 1);
//       t=-GptPidL.voltage;
//    }
//    oled_show_int(56,2,t,8);

    return t;
}

int motorRStop_pid(int a,int b)
{
    int t;

    PidRStop.Set      = a;
    PidRStop.Actual   = b;

    PidRStop.err      = PidRStop.Set - PidRStop.Actual;

    PidRStop.integral = PidRStop.integral + PidRStop.err;

    PidRStop.voltage = PidRStop.Kp*(PidRStop.err-PidRStop.err_last)
                + PidRStop.Ki*PidRStop.err
                + PidRStop.Kd*(PidRStop.err-2*PidRStop.err_last+PidRStop.err_last_last)
                + PidRStop.Kout
                + PidRStop.voltage;

    PidRStop.err_last_last = PidRStop.err_last;
    PidRStop.err_last = PidRStop.err;
    if(PidRStop.voltage>6000)
    {
        PidRStop.voltage=6000;
    }
    else if(PidRStop.voltage<-6000)
    {
        PidRStop.voltage=-6000;
    }
//    oled_show_int(16,2,PidRStop.voltage,8);
    if(PidRStop.voltage>0)
    {
        gpio_set_level(MOTORR_DIR, 0);//右轮1为正转
        t=PidRStop.voltage;
//        oled_show_int(16,3,MOTORR_DIR,3);
    }
    else
    {
        gpio_set_level(MOTORR_DIR, 1);
//        gpio_toggle_level(MOTORR_DIR);
        t=-PidRStop.voltage;
//        oled_show_int(56,3,MOTORR_DIR,3);
    }
//    oled_show_int(56,2,t,8);

    return t;
}



int motorLStop_pid(int a,int b)
{
    int t;

    PidLStop.Set      = a;
    PidLStop.Actual   = b;
    PidLStop.err      = PidLStop.Set - PidLStop.Actual;

    PidLStop.integral = PidLStop.integral + PidLStop.err;

    PidLStop.voltage = PidLStop.Kp*(PidLStop.err-PidLStop.err_last)
                + PidLStop.Ki*PidLStop.err
                + PidLStop.Kd*(PidLStop.err-2*PidLStop.err_last+PidLStop.err_last_last)
                + PidLStop.Kout
                + PidLStop.voltage;

    PidLStop.err_last_last = PidLStop.err_last;
    PidLStop.err_last = PidLStop.err;
    if(PidLStop.voltage>6000)
    {
        PidLStop.voltage=6000;
    }
    else if(PidLStop.voltage<-6000)
    {
        PidLStop.voltage=-6000;
    }

    if(PidLStop.voltage>0)
    {
        gpio_set_level(MOTORL_DIR, 0);
        t=PidLStop.voltage;
    }
    else
    {
        gpio_set_level(MOTORL_DIR, 1);
        t=-PidLStop.voltage;
    }

    return t;

    //GptPidR.voltage = fmax(-8000, fmin(GptPidR.voltage, 8000));
}





