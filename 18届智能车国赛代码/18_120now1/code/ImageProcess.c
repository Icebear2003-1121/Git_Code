
/*
 * ImageProcess.c
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */




#include "ImageProcess.h"

uint8 LeftLostStart,LeftLostEnd,RightLostStart,RightLostEnd;
float parameterA=0, parameterB=0;
uint16 Garage_T;  //出库计时
uint16 Barrier_T;  //横断计时
//下面是元素判断标志
static uint8 g_ucFlagRound=0;
static uint8 g_ucFlagCross=0;
static uint8 g_ucFlagOpening=0;
static uint8 g_ucFlagBarrier=0;
static uint8 g_ucFlagZebra=0;
static uint8 g_ucFlagRamp=0;
static uint8 g_ucFlagZebraFlag=0;
int Opennum = 0;
int16 cross_s = 0;
int Round14flag = 0;
uint8 Elements_flag=0;
uint8 RoundFlag = 0;
uint8 OpenRoadFlag = 0;
uint8 BarrierFlag = 0;
uint8 ZebraFlag = 0;
uint8 RampFlag = 0;
uint8 CrossFlag = 0;
uint16 Avegray = 0;
uint16 Barrier_T = 0;
int16 tof_gyroz=0;
uint16 DirTof;

/**********元素处理结构体**********/
struct XkElement road_type = {
       .Ramp                    = 0,
       .barrier                 = 0,
       .straight                = 0,
       .bend                    = 0,
       .Cross                   = 0,
       .LeftCirque              = 0,
       .RightCirque             = 0,
       .LeftOut                 = 0,
       .RightOut                = 0,
       .Leftin                  = 0,
       .Rightin                 = 0,
       .Open_road               = 0,
};


struct XkElement annulus = {
        .Ramp                    = 0,
        .barrier                 = 0,
        .straight                = 0,
        .bend                    = 0,
        .Cross                   = 0,
        .LeftCirque              = 0,
        .RightCirque             = 0,
        .LeftOut                 = 0,
        .RightOut                = 0,
        .Leftin                  = 0,
        .Rightin                 = 0,
        .Open_road               = 0,
};


struct XkElement jiaodu = {
        .Ramp                    = 0,
        .barrier                 = 0,
        .straight                = 0,
        .bend                    = 0,
        .Cross                   = 0,
        .LeftCirque              = 0,
        .RightCirque             = 0,
        .LeftOut                 = 0,
        .RightOut                = 0,
        .Leftin                  = 0,
        .Rightin                 = 0,
        .Open_road               = 0,
};



struct XkElement stage = {
        .Ramp                    = 0,
        .barrier                 = 0,
        .straight                = 0,
        .bend                    = 0,
        .Cross                   = 0,
        .LeftCirque              = 0,
        .RightCirque             = 0,
        .LeftOut                 = 0,
        .RightOut                = 0,
        .Leftin                  = 0,
        .Rightin                 = 0,
        .Open_road               = 0,
};



struct XkElement speed = {
        .Ramp                    = 0,
        .barrier                 = 0,
        .straight                = 0,
        .bend                    = 0,
        .Cross                   = 0,
        .LeftCirque              = 0,
        .RightCirque             = 0,
        .LeftOut                 = 0,
        .RightOut                = 0,
        .Leftin                  = 0,
        .Rightin                 = 0,
        .Open_road               = 0,
};



struct XkElement show = {
        .Ramp                    = 0,
        .barrier                 = 0,
        .straight                = 0,
        .bend                    = 0,
        .Cross                   = 0,
        .LeftCirque              = 0,
        .RightCirque             = 0,
        .LeftOut                 = 0,
        .RightOut                = 0,
        .Leftin                  = 0,
        .Rightin                 = 0,
        .Open_road               = 0,
};
//结构体定义
struct FLAG flag =
{
        .init_ok         = 0,
        .start           = 0,
        .strategy1       = 0,
        .strategy2       = 0,
        .strategy3       = 0,
        .strategy4       = 0,
        .adjust_ok       = 0,
        .t_ms            = 0,
        .msg_collect     = 0,
        .speed           = 0,
        .direction       = 0,
        .open_check_ku   = 1,
        .stop            = 0,
        .image_show      = 0
};
struct XkElement Xk =
{
       .Ramp                    = 0,
       .barrier                 = 0,
       .straight                = 0,
       .bend                    = 0,
       .Cross                   = 0,
       .LeftCirque              = 0,
       .RightCirque             = 0,
       .LeftOut                 = 0,
       .RightOut                = 0,
       .Leftin                  = 0,
       .Rightin                 = 0,
       .Open_road               = 0,
};
/**********元素处理结构体**********/
/*
 标志位检测部分
 */
//void FlagDetection(void)
//{
//    if(Xk->LRound_flag == 1)
//    {
//
//
//    }
//
//
//
//}
//-----------------------------------------------------------------------------------------
//  @brief      元素识别
//  @param      void
//  @return     void
//  Sample usage:    ElementsJudge();
//-----------------------------------------------------------------------------------------
int ElementsJudge()
{
    /* 获取tof测距读数 */
    dl1a_get_distance();

        if(ZebraJudge(Bin_Image,LCDW/2,55,15,10)/*||ZebraJudge(Bin_Image,LCDW/2+5,55,15,10)||ZebraJudge(Bin_Image,LCDW/2-6,55,15,10)*/)
            {
                if(g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagZebraFlag==0)
                {
                    if(SideIsNoLost(RoadLostFlag,45,20,1,5)&&!SideIsNoLost(RoadLostFlag,COLH-1,40,2,5))//左车库
                    {
                        RoundFlag=0;
                        g_ucFlagRound=0;
                        g_ucFlagOpening=0;
                        OpenRoadFlag=0;
                        g_ucFlagZebra=21;
                        g_ucFlagZebraFlag=1;
                        printf("Zebra\n");
                    }

                    if(SideIsNoLost(RoadLostFlag,45,20,2,5)&&!SideIsNoLost(RoadLostFlag,COLH-1,40,1,5))//右车库
                    {
                        RoundFlag=0;
                        g_ucFlagRound=0;
                        g_ucFlagOpening=0;
                        OpenRoadFlag=0;
                        g_ucFlagZebra=22;
                        g_ucFlagZebraFlag=1;
                        printf("Zebra\n");
                    }
                }

                if(g_ucFlagRound==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagZebraFlag==1)
                {
                    if(SideIsNoLost(RoadLostFlag,45,20,1,5)&&!SideIsNoLost(RoadLostFlag,COLH-1,40,2,5))//左车库
                    {
                        RoundFlag=0;
                        g_ucFlagRound=0;
                        g_ucFlagOpening=0;
                        OpenRoadFlag=0;
                        g_ucFlagZebra=21;
                        g_ucFlagZebraFlag=1;
                        printf("Zebra\n");
                    }

                    if(SideIsNoLost(RoadLostFlag,45,20,2,5)&&!SideIsNoLost(RoadLostFlag,COLH-1,40,1,5))//右车库
                    {
                        RoundFlag=0;
                        g_ucFlagRound=0;
                        g_ucFlagOpening=0;
                        OpenRoadFlag=0;
                        g_ucFlagZebra=22;
                        g_ucFlagZebraFlag=1;
                        printf("Zebra\n");
                    }
                }
            }

            if(g_ucFlagZebra)
            {
                speed_base = 0;
//                oled_show_string(105,0,"Z");
                if(!ZebraFlag)
                {
                GyroOn=1;
                ClearGyro();
                gpt_count_init();
                }
                ZebraProcess(ImageSide,&g_ucFlagZebra);
                ZebraFlag=g_ucFlagZebra;
//                oled_show_int(104,1,g_ucFlagZebra,4);
                return 1;


            }
//
//    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* 圆环判断start */
    /* 左圆环 */
//            oled_show_int(0,0,RoundaboutLine(ImageSide,30,20,2,53/*57*/,3),3);
//            oled_show_int(0,1,RoundaboutLine(ImageSide,COLH-1,50,2,74,1),3);
//            oled_show_int(0,2,SideIsNoLost(RoadLostFlag,45,20,1,5),3);
//            oled_show_int(0,3,!SideIsNoLost(RoadLostFlag,50,15,2,0),3);
//            oled_show_int(0,4,!ForkaboutGetArc(ImageSide,60,15,2,3),3);
//            oled_show_int(0,5,RoadSideMono(ImageSide,50,20,2)==2,3);

        if(RoundaboutLine(ImageSide,25,15,2,53,1)&&!SideIsNoLost(RoadLostFlag,59,50,1,3)&&SideIsNoLost(RoadLostFlag,40,20,1,4)&&!SideIsNoLost(RoadLostFlag,50,15,2,0)&&!Tututu(2)&&!ForkaboutGetArc(ImageSide,60,15,2,3)&&RoadSideMono(ImageSide,50,20,2)==2&&!SideIsNoLost(RoadLostFlag,12,8,2,1))
        {
            if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
                       {
//                    road_type.LeftCirque=1;
                    g_ucFlagRound = 1;//road_type.LeftCirque;
//                    oled_show_string(105,0,"L");
//                BeepON;
                       }


        }
     /* 右圆环 */
        if(RoundaboutLine(ImageSide,25,15,1,46,1)&&RoundaboutLine(ImageSide,LCDH-1,50,1,25,1)&&SideIsNoLost(RoadLostFlag,40,25,2,5)&&!SideIsNoLost(RoadLostFlag,50,15,1,0)&&!ForkaboutGetArc(ImageSide,60,15,1,3)&&RoadSideMono(ImageSide,50,20,1)==1)
        {
            if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
                       {
//                                road_type.RightCirque=2;
                                g_ucFlagRound = 2;//road_type.RightCirque;
//                                oled_show_string(105,0,"R");
                       }
        }
    /* 圆环判断end */

    if(road_type.LeftCirque || road_type.RightCirque)
    {
        Elements_flag=1;
    }
    else
    {
        Elements_flag=0;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

//    oled_show_int(105,7,g_ucFlagBarrier,1);
//    oled_show_int(105,4,g_ucFlagBarrier,1);
//    oled_show_int(105,5,UpdownSideGet(Bin_Image,20,COLH-2, 80,20,UpSide),1);

    /* 路障判断start */
    if(dl1a_distance_mm<1400&&UpdownSideGet(Bin_Image,20,COLH-2, 80,tofdistance,UpSide)&&!SideIsNoLost(RoadLostFlag,58,20,1,1)&&!SideIsNoLost(RoadLostFlag,58,20,2,1))
       {
            if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
           {
               if(flag.barrierDir==1)
               {
                   GyroOn=1;
                   g_ucFlagBarrier=3;
                   ClearGyro();
                   gpt_count_init();
                   printf("barrier\n");
//                   oled_show_string(105,7,"B");
               }
               else if(flag.barrierDir==0)
               {
                   GyroOn=1;
                   g_ucFlagBarrier=4;
                   ClearGyro();
                   gpt_count_init();
                   printf("barrier\n");
//                   oled_show_string(105,7,"B");
               }

           }
       }
    /* 路障判断end */
    /* 坡道判断start */
        if(dl1a_distance_mm<400&&!UpdownSideGet(Bin_Image,20,COLH-2, 80,tofdistance,UpSide)&&UpdownSideGet(Bin_Image,20,COLH-2, 80,0,UpSide)&&!SideIsNoLost(RoadLostFlag,45,15,1,1)&&!SideIsNoLost(RoadLostFlag,45,15,2,1))
        {
            if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
            {
                g_ucFlagRamp=1;
                road_type.Ramp = 1;
                GyroOn=1;
                ClearGyro();
                BeepON;
                printf("ramp\n");
//                oled_show_string(105,0,"P");
            }
        }
    /* 坡道判断end */

    /* 十字判断start */
    /*正入十字判断*/
//    if(cross_left[0] =cross_left[1] =cross_right[0] =cross_right[1] =1)
//    {
//        g_ucFlagCross=1;
//    }
    if(SideIsNoLost(RoadLostFlag,40,20,1,10)&&SideIsNoLost(RoadLostFlag,40,20,2,10))//正入十字有用，具体点通过上位机导入十字前图像得到
    {
        road_type.Cross=1;
//        BeepON;
    }
    /* 十字判断end */
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /* 断路判断start */
    if(UpdownSideGet(Bin_Image,20,COLH-2, 80,20,UpSide)&&RoadSideUpMonoBTS(UpSide,30,40)==1&&RoadSideUpMonoSTB(UpSide,60,70)==1)//Bin_Image,30,COLH-2, 70,20,UpSide)&&RoadSideUpMonoBTS(UpSide,30,40)==1&&RoadSideUpMonoSTB(UpSide,60,70)==1
       {
           if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
           {
               g_ucFlagOpening=1;
//               BeepON;
               printf("open\n");
//               oled_show_string(105,0,"O");
               ips200_show_string(150, 0, "O");
           }
       }
    /* 断路判断end */
//    ////////////////////////////////////////路障判断start//////////////////////////////////////////
//    if(dl1a_distance_mm<1400&&UpdownSideGet(Bin_Image,30,COLH-2, 80,20,UpSide)&&!SideIsNoLost(RoadLostFlag,58,30,1,1)&&!SideIsNoLost(RoadLostFlag,45,20,2,1))
//    {
//        if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
//        {
//            //speed_base=80;
//            BeepON;
//            GyroOn=1;
//            g_ucFlagBarrier=1;
//            ClearGyro();
//            gpt_count_init();
//            printf("barrier\n");
//            oled_show_string(105,7,"B");
//        }
//    }
/////////////////////////////////////////////路障判断end////////////////////////////////////////
    if(g_ucFlagRound)
           {
               Elements_flag=1;
           }
           else
           {
               Elements_flag=0;
           }

           if(g_ucFlagRound)
           {
               RoundProcess(ImageSide,&g_ucFlagRound);
               RoundFlag=g_ucFlagRound;
               return 1;
           }

           if(g_ucFlagRamp)
           {
               RampProcess(&g_ucFlagRamp);
               RampFlag=g_ucFlagRamp;
               return 1;
           }

           if(g_ucFlagOpening)
           {
               OpeningProcess(ImageSide,&g_ucFlagOpening);
               OpenRoadFlag=g_ucFlagOpening;
               return 1;
           }

           if(g_ucFlagBarrier)
           {
               BarrierProcess(ImageSide,&g_ucFlagBarrier);
               BarrierFlag=g_ucFlagBarrier;
               return 1;
           }
           return 1;
   }
//-------------------------------------------------------------------------------------------------------------------
//  @brief      元素处理
//  @param      void
//  @return     void
//  Sample usage:    Element_Handle();
//-------------------------------------------------------------------------------------------------------------------
int  Element_Handle()
{
//    ElementsJudge();
    if(g_ucFlagRound)
       {
           Elements_flag=1;
       }
       else
       {
           Elements_flag=0;
       }

       if(g_ucFlagRound)
       {
           RoundProcess(ImageSide,&g_ucFlagRound);
           RoundFlag=g_ucFlagRound;
           return 1;
       }

       if(g_ucFlagRamp)
       {
           RampProcess(&g_ucFlagRamp);
           RampFlag=g_ucFlagRamp;
           return 1;
       }

       if(g_ucFlagOpening)
       {
           OpeningProcess(ImageSide,&g_ucFlagOpening);
           OpenRoadFlag=g_ucFlagOpening;
           return 1;
       }

       if(g_ucFlagBarrier)
       {
           BarrierProcess(ImageSide,&g_ucFlagBarrier);
           BarrierFlag=g_ucFlagBarrier;
           return 1;
       }
       return 1;
}
/*
赛道封闭
imageInput:放入二值化图像
startX     :起始点列数
endX       :结束点列数
endX一定要大于startX
imageOut：放入上边线数组
MaxY：判断拐点最大Y值
return：大于y值为1，小于y值为0
Trcak_closure(Bin_Image[COLH][COLW],20,80,UpSide[COLW])
*/
uint8 Track_closure(uint8 imageInput[COLH][COLW],uint8 startX,uint8 endX,int imageOut[COLW],uint8 MaxY) //传入图像边界or二值化
{
    uint8 x,y;

       for(x = startX;x<endX+1;x++)
       {
           for(y=COLH-1;y>0;y--)
           {
               imageOut[x]=0;
               if(imageInput[x][y])
               {
                   imageOut[x]=y;
                   if (y<MaxY)
                   {
                       return 0;
                   }
                   break;
               }
           }
           if(imageOut[x]==0)//||(x>startX&&abs(imageOut[x]-imageOut[x-1])>4))
           {
               return 0;
           }

       }
       return 1;
   }
/*赛道封闭
imageInput:放入二值化图像
startX     :起始点列数
endX       :结束点列数
endX一定要大于startX
imageOut：放入上边线数组
MaxY：判断拐点最大Y值
return：大于y值为1，小于y值为0
Trcak_closure(Bin_Image[COLH][COLW],20,80,UpSide[COLW])
*/
//修改：时间：05.17   修改人：Ice Bear
uint8 Track_closure1(uint8 imageInput[COLH][COLW],uint8 startX,uint8 endX,uint8 imageOut[COLW],int MaxY) //传入图像边界or二值化
{
    uint8 x,y,i;
    int temp;
       for(x = startX;x<endX+1;x++)
       {
           for(y=COLH-1;y>0;y--)
           {
               imageOut[x]=0;
               if(imageInput[x][y])
               {
                   imageOut[x]=y;
                   break;
               }
           }
           if(x>(startX+15))    //这里的15可以自行根据图像而改变，在这里的意义是限制判断开始条件，即当扫到一定的列数之后在进行断路判断
           {
               for(i = x; i>x-10;i--)   //这里的10可以自行根据图像而改变，当这里改变之后，相应的下面的10也要改变
               {
                   temp += imageOut[i];
                }
                if((temp/10) == MaxY)
                {
                   return 0;
                }
           }
       }
       return 1;
   }
/*
判断两点距离
imageOut:放入保存丢边信息的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
index:距离，大于index+1
distance:距离
count:大于count返回1,小于等于count返回0
*/
uint8 PointsDistance(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 index,uint8 distance,uint8 count)
{
    uint8 y,num=0;
    switch(status)
    {
        case 1:
            for(y=imageStart;y>imageEnd;y--)
            {
                  if(imageOut[y-distance][0]-imageOut[y][0]>index)
                  {
                      num++;
                      if (num > count)
                      {
                          return 1;
                      }
                  }
            }
            break;
        case 2:
            for(y=imageStart;y>imageEnd;y--)
            {
                if(imageOut[y][1]-imageOut[y-distance][1]>index)
                {
                    num++;
                    if (num > count)
                    {
                        return 1;
                    }
                }
            }
            break;
    }
    return 0;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      判断边线是否突变
//  @param      type        1：左圆环  2：右圆环
//  @return     0：不突变  1：突变
//  Sample usage:    Tututu(1);
//-------------------------------------------------------------------------------------------------------------------
uint8 Tututu(uint8 type)
{
    if (type ==1)
    {
        for(uint8 y1 = COLH -1; y1 >= 20; y1 = y1-4)
        {
            if(fabs(ImageSide[y1][0] -ImageSide[y1-1][0]) < 8 && (ImageSide[y1][0] -ImageSide[y1-10][0]) >8)
            {
                return 1;
            }
        }
    }
    else if (type ==2)
    {
        for(uint8 y1 =COLH -1; y1>=20; y1=y1-4)
        {
            if(fabs(ImageSide[y1][1] -ImageSide[y1-1][1])<4 && (ImageSide[y1][1] -ImageSide[y1-10][1]) <-8)
            {
                return 1;
            }
        }
    }
    return 0;
}
/*
纵向扫白点数
imageInput:放入二值化图像
startX     :起始点列数
startY     :起始点行数
endX       :结束点列数
endY       :结束点行数
endY一定要大于startY
返回为白点数
*/
int WhitePointNumUP(uint8 imageInput[COLH][COLW],uint8 startX,uint8 startY,uint8 endX,uint8 endY)
{
    uint8 x,y;
    int num=0;

    for(x=startX;x<endX+1;x++)
    {
        for(y=startY;y>endY;y--)
        {
            if(imageInput[y][x])
            {
                num++;
            }
        }
    }

    return num;
}

/*
横向扫白点数
imageInput:放入二值化图像
startX     :起始点列数
startY     :起始点行数
endX       :结束点列数
endY       :结束点行数
endY一定要大于startY
返回为白点数
*/
int WhitePointNum(uint8 imageInput[COLH][COLW],uint8 startX,uint8 startY,uint8 endX,uint8 endY)
{
    uint8 x,y;
    int num=0;
    for(y=startY;y>endY-1;y--)
    {
        for(x=startX;x<endX+1;x++)
        {
            if(imageInput[y][x])
            {
                num++;
            }
        }
    }

    return num;
}

//直道检测函数
uint8 sudu_yingzi = 0;    //速度因子
void Check_Zhidao()
{
    uint8 inc = 0, dec = 0;
    sudu_yingzi = 0;
    for(uint8 y = COLH-1; y > 1; y--)
    {
        if((ImageSide[y][0] <= ImageSide[y-1][0]) && ImageSide[y][0] != 0)   //两边不丢线才计算直道长度
        {
            inc++;
        }
        if((ImageSide[y][1] >=ImageSide[y-1][1]) && ImageSide[y][1] != COLW -1)
        {
            dec++;
        }
    }

    if(inc >= dec)
    {
        sudu_yingzi = dec;
    }
    else
    {
        sudu_yingzi = inc;
    }
}

/*
判断边界是否连续丢线
imageOut:放入保存丢边信息的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
index:连续丢失多少行，大于index返回1,小于等于index返回0
*/
uint8 SideIsNoLost(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 index)
{
    uint8 y,num=0;
    switch(status)
    {
        case 1:
          for(y=imageStart;y>imageEnd;y--)
          {
              if(imageOut[y][0]==0)
              {
                  num++;
                  if (num > index)
                  {
                      return 1;
                  }
              }
              else
              {
                  num=0;
              }
          }
          break;
        case 2:
          for(y=imageStart;y>imageEnd;y--)
          {
              if(imageOut[y][1]==0)
              {
                  num++;
                  if (num > index)
                  {
                      return 1;
                  }
              }
              else
              {
                  num=0;
              }
          }
          break;
    }
    return 0;
}

/*
判断边界是否超过某列
imageOut:放入保存边界的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
num:左边界大于某列,右边界小于某列
index:左边界大于某列且大于index列则返回:0,右边界小于某列且大于index列则返回:0,否则返回:1
*/
uint8 RoundaboutLine(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index)
{
    int i=0;
    uint8 n=0;
    switch(status)
        {
          case 1:
              for(i = imageStart; i > imageEnd; i--)
              {
                  if(imageOut[i][0]>=num)
                  {
                      n++;
                  }
                  if(n>=index)
                  {
                      return 0;
                  }
              }
              break;

          case 2:
              for(i = imageStart; i > imageEnd; i--)
              {
                  if(imageOut[i][1]<=num)
                  {
                      n++;
                  }
                  if(n>=index)
                  {
                      return 0;
                  }
              }
              break;
        }
    return 1;
}
/*
计算一块区域的平均灰度值
imageOut:放入灰度图像
startX     :起始点列数
startY     :起始点行数
endX       :结束点列数
endY       :结束点行数
endY一定要大于startY
GrayThreshold 设定灰度阈值
*/
uint8 AveGray(uint8 imageOut[COLH][COLW],int startX, int startY, int endX, int endY,uint16 GrayThreshold)
{
    uint8 x,y;
    int sumgray=0;
    for(y=startY;y<=endY;y++)
    {
        for(x=startX;x<=endX;x++)
        {
            sumgray += imageOut[x][y];
        }
    }
    Avegray=sumgray/((endY-startY)*(endX-startX));

    if(sumgray/((endY-startY)*(endX-startX))>GrayThreshold)
    {
        return 1;
    }
}
//计算前瞻量(中线)
uint8 qianzhang = 0;
uint8 qianzhang_l = 0;
uint8 qianzhang_r = 0;
void QianZhang(uint8 type)
{
    if(type == 0)
    {
        qianzhang = 0;
        for(uint8 y = COLH -1; y > 1; y--)
        {
            if(Bin_Image[y][COLW/2] != 0)
            {
                qianzhang++;
            }
            else
            {
              break;
            }
        }
    }
    else if(type == 1)
    {
        qianzhang_l = 0;
        for(uint8 y = COLH -1; y > 1; y--)
        {
            if(Bin_Image[y][COLW/2 -30] != 0)
            {
                qianzhang_l++;
            }
            else
            {
              break;
            }
        }
    }
    else if(type == 2)
    {
        qianzhang_r = 0;
        for(uint8 y = COLH -1; y > 1; y--)
        {
            if(Bin_Image[y][COLW/2 +30] != 0)
            {
                qianzhang_r++;
            }
            else
            {
              break;
            }
        }
    }
}

/*
判断边界单调性
imageIn:放入保存边界的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
返回:不单调or错误:0,单调递增:1,单调递减:2
*/
uint8 RoadSideMono(int imageIn[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status)
{
    uint8 i = 0, num = 0;
    switch(status)
    {
        case 1:
            for(i = imageStart; i > imageEnd+1; i--)
            {
                if(imageIn[i][0] <= imageIn[i-1][0])
                    num++;
                else
                    num = 0;
                if (num >= (imageStart-imageEnd)*5/8)
                    return 1;
            }

            for(i = imageStart; i > imageEnd+1; i--)
            {
                if(imageIn[i][0] >= imageIn[i-1][0])
                    num++;
                else
                    num = 0;
                if (num >= (imageStart-imageEnd)*5/8)
                    return 2;
            }
            break;

        case 2:
            for(i = imageStart; i > imageEnd+1; i--)
            {
                if(imageIn[i][1] <= imageIn[i-1][1])
                    num++;
                else
                    num = 0;
                if (num >= (imageStart-imageEnd)*5/8)
                    return 1;
            }

            for(i = imageStart; i > imageEnd+1; i--)
            {
                if(imageIn[i][1] >= imageIn[i-1][1])
                    num++;
                else
                    num = 0;
                if (num >= (imageStart-imageEnd)*5/8)
                    return 2;
            }
            break;
    }
    return 0;
}

/*
判断边界是否有拐点
imageOut:放入保存边界的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
num:判断拐点的条件,连续单调增和连续单调减的行数
index:拐点在第几行
返回:无拐点:0,有拐点:1
*/
uint8 RoundaboutGetArc(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8* index)
{
    int i = 0;
    uint8 inc = 0, dec = 0, n = 0;
    switch(status)
    {
      case 1:
        for(i = imageStart; i > imageEnd-1; i--)
        {
            if(imageOut[i][0] == imageOut[i+1][0]){
                n++;
                continue;
            }
            if(imageOut[i][0] > imageOut[i+1][0])
            {
                inc++;
                inc+=n;
                n=0;
            }
            else
            {
                dec++;
                dec+=n;
                n=0;
            }

            /* 有弧线 */
            if(inc > num && dec > num)
            {
                *index = i + num;
                return 1;
            }
        }

        break;

      case 2:
        for(i = imageStart; i > imageEnd-1; i--)
        {
            if(imageOut[i][1] == imageOut[i+1][1])
            {
                n++;
                continue;
            }
            if(imageOut[i][1] > imageOut[i+1][1])
            {
                inc++;
                inc+=n;
                n = 0;
            }
            else
            {
                dec++;
                dec+=n;
                n=0;
            }

            /* 有弧线 */
            if(inc > num && dec > num)
            {
                *index = i + num;
                return 1;
            }
        }

        break;
    }

    return 0;
}

/*
关于边界列间距的函数
imageOut:放入保存边界的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
num:边界列间距判断
index:边界列间距大于num且大于index列则返回:0,否则返回:1
*/
uint8 SideColumnSpace(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index)
{
    int i=0;
    uint8 n=0;
    switch(status)
        {
          case 1:
              for(i = imageStart; i > imageEnd-1; i--)
              {
                  if(abs(imageOut[i][0]-imageOut[i-1][0])>num)
                  {
                      n++;
                  }
                  if(n>index)
                  {
                      return 0;
                  }
              }
              break;

          case 2:
              for(i = imageStart; i > imageEnd-1; i--)
              {
                  if(abs(imageOut[i][1]-imageOut[i-1][1])>num)
                  {
                      n++;
                  }
                  if(n>index)
                  {
                      return 0;
                  }
              }
              break;
        }
    return 1;
}

/*
关于边界列间距的函数(连续)
imageOut:放入保存边界的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
num:边界列间距判断
index:边界列间距大于num且大于index列则返回:0,否则返回:1
*/
uint8 SideColumnSpaceContinue(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index)
{
    int i=0;
    uint8 n=0;
    switch(status)
        {
          case 1:
              for(i = imageStart; i > imageEnd-1; i--)
              {
                  if(abs(imageOut[i][0]-imageOut[i-1][0])>num)
                  {
                      n++;
                      if(n>index)
                       {
                          return 0;
                       }
                  }
                  else
                  {
                      n=0;
                  }
              }
              break;

          case 2:
              for(i = imageStart; i > imageEnd-1; i--)
              {
                  if(abs(imageOut[i][1]-imageOut[i-1][1])>num)
                  {
                      n++;
                      if(n>index)
                      {
                          return 0;
                      }
                  }
                  else
                  {
                      n=0;
                  }
              }
              break;
        }
    return 1;
}

/*
判断前方是否为斑马线
imageOut:二值化后的图像数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:斑马线判断条件,连续跳变的白点列数
返回:无斑马线:0,有斑马线:1
*/
uint8 ZebraJudge(uint8 imageOut[COLH][COLW],uint8 center,uint8 imageStart,uint8 imageEnd,uint8 status)//Bin_Image 30 10 12
{
    uint8 num=0,index=0,flag=0;
    uint8 y,x;
    uint8 imageFlag[COLW];
    uint8 imagePoint[COLW];
    for(y=imageStart;y>imageEnd;y--)
    {
        num=0;
        index=0;
        for(x=center+10;x>center-11;x--)//LCDW/2
        {
            if(imageOut[y][x]==1)
            {
                imageFlag[x]=1;
            }
            else
            {
                imageFlag[x]=0;
            }
        }
        for(x=center+10;x>center-11;x--)
        {
            if(imageFlag[x]==1)
            {
                imagePoint[index]=x;
                index++;
                if(index>0)
                {
                    if((imagePoint[index-1]-imagePoint[index])<3)
                    {
                        num++;
                    }
                }
            }
        }
        if(num>status/*&&num<18*/)
        {
            num=0;
            flag++;
            //return 1;
        }
        else
        {
            flag=0;
        }
        if(flag>5)
        {
            flag=0;
            return 1;
        }
    }
    return 0;
}

/*
判断前方是否有拐点（连续）
imageOut:二值化后的图像数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
num:判断拐点的条件,连续单调增和连续单调减的行数
返回:无拐点:0,有拐点:1
*/
uint8 ForkaboutGetArc(int imageOut[COLH][2], uint8 imageStart, uint8 imageEnd, uint8 status, uint8 num)
{
    int i = 0;
    uint8 inc = 0, dec = 0, n = 0, flag = 0;
    switch (status)
    {
    case 1:
        for (i = imageStart; i > imageEnd - 1; i--)
        {
            if (imageOut[i][0] == imageOut[i + 1][0]) {
                n++;
                continue;
            }
            if (imageOut[i][0] > imageOut[i + 1][0])
            {
                dec = 0;
                inc++;
                inc += n;
                n = 0;
                if (inc > num)
                {
                    flag = 1;
                }
            }
            else
            {
                inc = 0;
                if (flag)
                {
                    dec++;
                    dec += n;
                    n = 0;
                    if (dec > num)
                    {
                        return 1;
                    }
                }
            }
        }

        break;

    case 2:
        for (i = imageStart; i > imageEnd - 1; i--)
        {
            if (imageOut[i][1] == imageOut[i + 1][1])
            {
                n++;
                continue;
            }
            if (imageOut[i][1] < imageOut[i + 1][1])
            {
                dec = 0;
                inc++;
                inc += n;
                n = 0;
                if (inc > num)
                {
                    flag = 1;
                }
            }
            else
            {
                inc = 0;
                if (flag)
                {
                    dec++;
                    dec += n;
                    n = 0;
                    if (dec > num)
                    {
                        return 1;
                    }
                }
            }
        }

        break;
    }

    return 0;
}

/*
补线处理
imageSide  :边线
status     :左边线补线:1,右边线补线:2
startX     :起始点列数
startY     :起始点行数
endX       :结束点列数
endY       :结束点行数
endY一定要大于startY
*/
void ImageAddingLine(int imageOut[COLH][2], uint8 status, int startX, int startY, int endX, int endY)
{
    int i = 0;

    /* 直线 x = ky + b*/
    float k = 0.0f, b = 0.0f;
    switch(status)
    {
      case 1://左补线
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(i = startY; i < endY; i++)
            {
                imageOut[i][0] = (int)(k * i + b);
            }
            break;
        }

      case 2://右补线
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(i = startY; i < endY; i++)
            {
                imageOut[i][1] = (int)(k * i + b);
            }
            break;
        }

    }
}

uint8 ImageAddingLineCompare(int imageOut[COLH][2],uint8 status,int startX,int startY,int endX,int endY,uint8 num)
{
    int i = 0;
    int imageAddLine[COLH][2];
    uint8 n=0;

    /* 直线 x = ky + b*/
    float k = 0.0f, b = 0.0f;
    switch(status)
    {
      case 1://左补线
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(i = startY; i < endY; i++)
            {
                imageAddLine[i][0] = (int)(k * i + b);
                if(imageOut[i][0]>imageAddLine[i][0])
                {
                    n++;
                }
                if(n>=num)
                {
                    return 0;
                }
            }
            break;
        }

      case 2://右补线
        {
            k = (float)((float)endX - (float)startX) / (float)((float)endY - (float)startY);
            b = (float)startX - (float)startY * k;

            for(i = startY; i < endY; i++)
            {
                imageOut[i][1] = (int)(k * i + b);
                if(imageOut[i][1]<imageAddLine[i][1])
                {
                    n++;
                }
                if(n>=num)
                {
                    return 0;
                }
            }
            break;
        }

    }
    return 1;
}

/*
判断边界单调性小到大
imageIn:放入上边线数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart<imageEnd
返回:不单调or错误:0,单调递增:1,单调递减:2
*/
uint8 RoadSideUpMonoSTB(int imageIn[COLW],uint8 imageStart,uint8 imageEnd)
{
    uint8 i = 0, num = 0;
    uint8 count=0;

    for(i = imageStart; i < imageEnd+1; i++)
    {
        if(imageIn[i] < imageIn[i+1])
        {
            num++;
            num+=count;
            count=0;
        }
        else if(imageIn[i] > imageIn[i+1])
        {
            num = 0;
            count=0;
        }
        else if(imageIn[i] == imageIn[i+1])
        {
            count++;
        }
        if (num >= (imageEnd-imageStart+1)*5/8)
            return 1;
    }



    for(i = imageStart; i < imageEnd+1; i++)
    {
        if(imageIn[i] > imageIn[i+1])
        {
            num++;
            num+=count;
            count=0;
        }
        else if(imageIn[i] < imageIn[i+1])
        {
            num = 0;
            count=0;
        }
        else if(imageIn[i] == imageIn[i+1])
        {
            count++;
        }
        if (num >= (imageEnd-imageStart+1)*5/8)
            return 2;
    }

    return 0;
}
/*
扫上线
imageInput:放入二值化图像
startX     :起始点列数
startY     :起始点行数
endX       :结束点列数
endY       :结束点行数
endY一定要大于startY
imageOut：放入上边线数组
*/
uint8 UpdownSideGet(uint8 imageInput[COLH][COLW],uint8 startX, uint8 startY, uint8 endX, uint8 endY,int imageOut[COLW])
{
    uint8 x,y;

    for(x = startX;x<endX+1;x++)
    {
        for(y=startY;y>endY;y--)
        {
            imageOut[x]=0;
            if(imageInput[y][x])
            {
                imageOut[x]=y;
                break;
            }
        }
        if(imageOut[x]==0||(x>startX&&abs(imageOut[x]-imageOut[x-1])>4))
        {
            return 0;
        }
    }
    return 1;
}
/*
判断目标范围内是否为全白行或者全白列
imageInput:放入二值化图像
status    :0为判断行，1为判断列
X         :起始点行/列数
white_num       :sum>white_num为1，反之为0
*/
uint8 White_Judgment(uint8 imageInput[COLH][COLW],uint8 status, uint8 X,uint8 white_num)
{
    uint8 sum;
    uint8 x,y;
    switch(status)
    {
        case 0:for(y=X;y<=COLW-1;y++)
        {
                sum += imageInput[x][y];
        }
        if(sum>white_num)return 1;
        else return 0;
        break;
        case 1:for(x=X;x<=COLH-1;x++)
        {
            sum += imageInput[x][y];
        }
        if(sum>white_num)return 1;
        else return 0;
                break;
    }

}
/*
判断边界单调性大到小
imageIn:放入上边线数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart<imageEnd
返回:不单调or错误:0,单调递增:1,单调递减:2
*/
uint8 RoadSideUpMonoBTS(int imageIn[COLW],uint8 imageStart,uint8 imageEnd)
{
    uint8 i = 0, num = 0;
    uint8 count=0;

    for(i = imageEnd; i > imageStart-1; i--)
    {
        if(imageIn[i] < imageIn[i-1])
        {
            num++;
            num+=count;
            count=0;
        }
        else if(imageIn[i] > imageIn[i-1])
        {
            num = 0;
            count=0;
        }
        else if(imageIn[i] == imageIn[i-1])
        {
            count++;
        }
        if (num >= (imageEnd-imageStart+1)*5/8)
            return 1;
    }

    for(i = imageEnd; i > imageStart-1; i--)
    {
        if(imageIn[i] > imageIn[i-1])
        {
            num++;
            num+=count;
            count=0;
        }
        else if(imageIn[i] < imageIn[i-1])
        {
            num = 0;
            count=0;
        }
        else if(imageIn[i] == imageIn[i-1])
        {
            count++;
        }
        if (num >= (imageEnd-imageStart+1)*5/8)
            return 2;
    }
    return 0;
}
/*
圆环处理函数：
imageOut:放入保存边界的数组
state:圆环标志位
 */
void RoundProcess(int imageOut[COLH][2],uint8* state)
{


    uint8 y;
    switch(*state)
    {
        case 1:
                    if(SideIsNoLost(RoadLostFlag,60,45,1,12))
                    {

        //                if(!RoundaboutLine(ImageSide,22,12,2,50,3))
        //                {
        //                    *state=0;
        //                    break;
        //                }
                        *state=3;
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                        BeepON;
                    }
                    BeepON;
                    ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                    break;
                case 3:
                    if(!SideIsNoLost(RoadLostFlag,60,45,1,1))
                    {
//                        Anchorsite(curvature,12,2,1);
                        *state=5;
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                        BeepOFF;
                    }
                    ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                    break;
                case 5:
                    if(RoundaboutLine(ImageSide,40,30,2,80,6))
                    {
                        //speed_base = 100;
                        GyroOn=1;
                        *state=7;
                        ClearGyro();
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                        BeepON;
                    }
                    ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                    break;
                case 7:
                    if(!RoundaboutLine(ImageSide,40,30,2,80/*80*/,6)&&!SideIsNoLost(RoadLostFlag,50,40,2,3))
                    {

                        *state=9;
                        BeepOFF;
                    }
                    ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                    break;
                case 9://环内
                    if(ForkaboutGetArc(ImageSide,40,18,2,3)&&!SideIsNoLost(RoadLostFlag,55,45,2,4/*9*/))
                    {
                        //strategic_mode = 1;
                        *state=11;
                        ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                        BeepON;
                    }
                    break;
                case 11:
                    //if(yaw > 13 && yaw <13.1) speed_base = 60;
                    if(SideIsNoLost(RoadLostFlag,60,50,2,5)&&!ForkaboutGetArc(ImageSide,50,18,2,2)&&!SideIsNoLost(RoadLostFlag,50,35,2,1))
                    {
//                        Anchorsite(curvature,24,2,1);
                        GyroOn=0;
                        strategic_mode = 0;
//                        speed_base = 155;
                        *state=13;
                        ImageGetSideRoundOut(Bin_Image,ImageSide,1);
                        BeepON;
                    }
                    ImageGetSideRoundArc(Bin_Image,ImageSide,0);
                    break;
                case 13:
                    if(SideIsNoLost(RoadLostFlag,59,50,1,6))
                    {
                        *state=15;
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                    }
                    ImageGetSideRoundOut(Bin_Image,ImageSide,1);
                    break;
                case 15:
                    if(!SideIsNoLost(RoadLostFlag,59,50,1,1))
                    {
                        Elements_flag = 0;
                        *state=0;
                        BeepOFF;

                    }
                    ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                    break;


        //////////////////////////////////////////////////////////////右环/////////////////////////////////////////////////////////////////////////
                    //右圆环
                    case 2:
                        if(SideIsNoLost(RoadLostFlag,60,45,2,12))
                        {
                            *state=4;
                            ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                            BeepON;
                        }
                        BeepON;
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                        break;
                    case 4:
                        if(!SideIsNoLost(RoadLostFlag,60,45,2,1))
                        {
//                            Anchorsite(curvature,28,2,1);
                            //speed_base = 100;
                            *state=6;
                            ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                            BeepOFF;
                        }
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                        break;
                    case 6:
                        if(RoundaboutLine(ImageSide,40,30,1,20,6))
                        {

                            GyroOn=1;
                            *state=8;
                            ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                            BeepON;
                        }
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                        break;
                    case 8:
                        if(!RoundaboutLine(ImageSide,40,30,1,20,6)&&!SideIsNoLost(RoadLostFlag,50,40,1,3))
                        {

                            *state=10;
                            BeepOFF;
                        }
                        ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                        break;
                    case 10:
                        if(ForkaboutGetArc(ImageSide,40,18,1,3))
                        {
                            //strategic_mode = 1;
                            *state=12;
                            ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                            BeepON;
                        }
                        break;
                    case 12:
                                if/*(yaw<-27)// || */((!SideIsNoLost(RoadLostFlag,45,35,1,2)&&!ForkaboutGetArc(ImageSide,50,18,1,2)&&SideIsNoLost(RoadLostFlag,LCDH-1,50,1,5)))//&&!SideIsNoLost(RoadLostFlag,50,35,2,1)/*&&!SideIsNoLost(RoadLostFlag,35,20,2,1)*/)
                                {
//                                    Anchorsite(curvature,24,2,1);
                                    strategic_mode = 0;
//                                    speed_base = 155;

                                    GyroOn=0;
                                    *state=14;
                                    ImageGetSideRoundOut(Bin_Image,ImageSide,0);
                                    BeepOFF;
                                }
                                BeepON;
                                ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                                break;

                    case 14:
                               if(SideIsNoLost(RoadLostFlag,59,50,2,6))
                               {
                                   *state=16;
                                   ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                               }
                               ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                               break;
                           case 16:
                               if(!SideIsNoLost(RoadLostFlag,59,50,2,1))
                               {
                                   *state=0;
                                   BeepOFF;
                                   RoadOut_flag =0;
                               }
                               ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                               break;
    }


}

//-----------------------------------------------------------------------------------------
//  @brief      出库函数
//  @param      type  1：左出库  2：右出库
//  @return     void
//  Sample usage:    Handle_Barn_Out(1);
//-----------------------------------------------------------------------------------------

//void Handle_Barn_Out(uint8 type)
//{//1左2右
//    /*左出库*/
//    if(type == 1)
//    {
//        if (Garage_T < 300) //直走
//        {
//           // DRV_MOTOR_Ctrl(1, 1500);
//           // DRV_MOTOR_Ctrl(2, 1500);
//            motor_control(2000,2000);
//        }
//        if(Garage_T >= 300 && Garage_T < 700)   //转弯
//        {
//         //   DRV_MOTOR_Ctrl(1, 1500);
//          //  DRV_MOTOR_Ctrl(2, 0);
//            motor_control(1000,2500);
//        }
//        if(Garage_T > 700)   //出库结束
//        {
//            road_type.Barn_l_out = 0;
//            flag.start = 1;
//            stage.Barn_l_out = 1;
//            type = 0;
//        }
//    }
//    /*右出库*/
//    else if(type == 2)
//    {
//        if (Garage_T < 300) //直走
//        {
//            //DRV_MOTOR_Ctrl(1, 2000);
//           // DRV_MOTOR_Ctrl(2, 2000);
//            motor_control(1500,1500);
//        }
//        else if(Garage_T >= 300 && Garage_T < 600)   //转弯
//        {
//          //  DRV_MOTOR_Ctrl(1, 4000);
//           // DRV_MOTOR_Ctrl(2, 2000);
//            motor_control(1500,0);
//        }
//        else   //出库结束
//        {
//            road_type.Barn_r_out = 0;
//            flag.start = 1;
//            stage.Barn_r_out = 1;
//        }
//    }
//}
/*
      加速处理函数
imageOut:放入保存边界的数组
state:圆环标志


 */

/*
//十字处理函数：
//imageOut:放入保存边界的数组
//state:圆环标志 */
//void CrossProcess(int imageOut[COLH][2])
//{
//               if(SideIsNoLost(RoadLostFlag,30,15,1,8)&&SideIsNoLost(RoadLostFlag,30,15,2,8))
//               {
////                   for(int i=60;i>=45;i--)
////                   {
////                       AuxiliaryLine[COLH-1]=(COLW-1)/2;
////                   }
//                   BeepON;
//               }
//               BeepON;
//               ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
//
//               if(!SideIsNoLost(RoadLostFlag,60,45,1,12)&&!SideIsNoLost(RoadLostFlag,60,45,2,12))
//                           {
//
//                               BeepOFF;
//                           }
//
//}

/*
路障处理函数：
imageOut:放入保存边界的数组
state:路障标志位
 */
void BarrierProcess(int imageOut[COLH][2],uint8* state)
{
    switch(*state)
       {
           case 3:
               if(fabs(yaw-BarrierIn)<0.1)
               {
                   gpt_count_init();
                   *state=5;
               }
               BeepON;
               break;
           case 5:
               if(gptR_Cir>Barriergpt&&gptL_Cir>Barriergpt)
               {
                   gpt_count_init();
                   *state=7;
               }
               BeepOFF;
               break;
           case 7:
               if(elec_value[0]>2500||elec_value[1]>2500)
               {
                   gpt_count_init();
                   *state=9;
               }
               BeepON;
               break;
           case 9:
               if(!SideIsNoLost(RoadLostFlag,60,40,1,1)&&!SideIsNoLost(RoadLostFlag,60,40,2,1))
               {
                   gpt_count_init();
                   GyroOn=0;
                   *state=0;
               }
               BeepOFF;
               break;
           case 4:
               if(fabs(yaw+BarrierIn)<0.1)
               {
                   gpt_count_init();
                   *state=6;
               }
               BeepON;
               break;
           case 6:
               if(gptR_Cir>Barriergpt&&gptL_Cir>Barriergpt)
               {
                   gpt_count_init();
                   *state=8;
               }
               BeepOFF;
               break;
           case 8:
               if(elec_value[0]>2500||elec_value[1]>2500)
               {
                   gpt_count_init();
                   *state=10;
               }
               BeepON;
               break;
           case 10:
               if(!SideIsNoLost(RoadLostFlag,60,40,1,1)&&!SideIsNoLost(RoadLostFlag,60,40,2,1))
               {
                   gpt_count_init();
                   GyroOn=0;
                   *state=0;
               }
               BeepOFF;
               break;

           case 100:
               RoadOut_flag=1;
               break;
       }

    }

/*
断路处理函数：
imageOut:放入保存边界的数组
state:断路标志位
 */
void OpeningProcess(int imageOut[COLH][2],uint8* state)
{
    switch(*state)
    {
                case 1:
                    if(UpdownSideGet(Bin_Image,20,COLH-2, 80,25,UpSide)&&RoadSideUpMonoBTS(UpSide,30,40)==1&&RoadSideUpMonoSTB(UpSide,60,70)==1)
                    {
                        *state=2;
                    }
                    break;
                case 2:
                    if(SideIsNoLost(RoadLostFlag,59,50,2,7)&&SideIsNoLost(RoadLostFlag,59,50,1,7))
                    {
                        speed_base = 130;
                        *state=3;
                        BeepON;
                    }
                    break;
                case 3:
                    if(AveGray(Image_Use,15,45,85,55,40)&&!SideIsNoLost(RoadLostFlag,59,50,2,2)&&!SideIsNoLost(RoadLostFlag,59,50,2,2)&&RoundaboutLine(ImageSide,55,50,2,57,1)&&RoundaboutLine(ImageSide,55,50,1,43,1))
                    {
                        system_delay_ms(100);
                        *state=0;
                        Block=0;
                        BeepOFF;
                        ips200_show_string(150, 170, "Out");
                    }
                    break;
    }
}

/*
斑马线处理函数：
imageOut:放入保存边界的数组
state:斑马线标志位
 */
void ZebraProcess(int imageOut[COLH][2],uint8* state)
{

        if(SpeedMode == 0 || SpeedMode == 3 || SpeedMode == 4 || SpeedMode == 5 || SpeedMode == 6)
        {
            switch(*state)
               {
        case 21:
            if(yaw>ZebraIn2 && gptR_Cir>12)
            {
                gpt_count_init();
                RoadOut_flag=1;
                flag.stop = 1;
                *state=25;

            }
            BeepON;
            break;

        case 23:
            if(WhitePointNumUP(Bin_Image,49,COLH-2,80,15)<100)
            {
                ImageGetSideRoundArc(Bin_Image,ImageSide,0);
                *state=25;
                BeepON;
            }
            BeepOFF;
            break;

        case 25:
            if(gptR_Cir>0&&gptL_Cir>0)
            {
//                ips200_show_float(0,100,yaw,2,2);
                gpt_count_init();
                RoadOut_flag=1;
                flag.stop = 1;
                *state=0;
            }
            BeepON;
            ImageGetSideRoundArc(Bin_Image,ImageSide,0);
            break;

        case 22:
            speed_base = 0;
            if(yaw<-ZebraIn2&& gptL_Cir>12)//(WhitePointNumUP(Bin_Image,20,COLH-2,49,30)>250)
            {
                gpt_count_init();
                RoadOut_flag=1;
                flag.stop = 1;
                *state=26;
            }
            BeepON;
            break;

        case 24:
            if(WhitePointNumUP(Bin_Image,20,COLH-2,49,15)<100)
            {
                ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                *state=26;
            }
            BeepOFF;
            break;

        case 26:
            if(gptR_Cir>0&&gptL_Cir>0)//(UpdownSideGet(Bin_Image,50,COLH-2,90,20,UpSide))
            {
//                ips200_show_float(0,100,yaw,2,2);
                RoadOut_flag=1;
                flag.stop = 1;
                *state=0;
            }
            BeepON;
            ImageGetSideRoundArc(Bin_Image,ImageSide,1);
            break;
        }
        }
        else
        {
            switch(*state)
               {
        case 21:
                   if(fabs(yaw-ZebraIn1)<0.1)//(WhitePointNumUP(Bin_Image,49,COLH-2,80,30)>250)
                   {
                       gpt_count_init();
                       *state=25;
       //                RoadOut_flag=1;

                   }
                   BeepON;
                   break;

               case 23:
                   if(WhitePointNumUP(Bin_Image,49,COLH-2,80,15)<100)
                   {
                       ImageGetSideRoundArc(Bin_Image,ImageSide,0);
                       *state=25;
                       BeepON;
                   }
                   BeepOFF;
                   break;

               case 25:
                   if(gptR_Cir>15&&gptL_Cir>15)//(UpdownSideGet(Bin_Image,10,COLH-2,50,35,UpSide))
                   {
                       gpt_count_init();
                       RoadOut_flag=1;
                       flag.stop=1;
                       *state=0;
                   }
                   BeepON;
                   ImageGetSideRoundArc(Bin_Image,ImageSide,0);
                   break;



               case 22:
                   if(fabs(yaw+ZebraIn1)<0.1)//(WhitePointNumUP(Bin_Image,20,COLH-2,49,30)>250)
                   {
                       gpt_count_init();
                       *state=26;
       //                RoadOut_flag=1;
//                       ips200_show_float(150, 60, ZebraFlag, 3, 2);
                   }
                   BeepON;
                   break;

               case 24:
                   if(WhitePointNumUP(Bin_Image,20,COLH-2,49,15)<100)
                   {
                       ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                       *state=26;
                   }
                   BeepOFF;
                   break;

               case 26:
                   if(gptR_Cir>15&&gptL_Cir>15)//(UpdownSideGet(Bin_Image,50,COLH-2,90,20,UpSide))
                   {
                       RoadOut_flag=1;
                       flag.stop=1;
                       *state=0;
                   }
                   BeepON;
                   ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                   break;
        }
        }
    }



/*
坡道处理函数：
imageOut:放入保存边界的数组
state:坡道标志位
 */
void RampProcess(uint8* state)
{
    switch(*state)
    {
        case 1:
                    if(dl1a_distance_mm>2000)
                    {
                        *state=2;
//                        speed_base=130;
                        BeepOFF;
                    }
                    BeepON;
                    break;
                case 2:
                    if(dl1a_distance_mm<1400)
                    {
                        *state=3;
                        speed_base=100;
                        BeepON;
                    }
                    ImageAddingLine(ImageSide,1,50,1,0,COLH-2);
                    ImageAddingLine(ImageSide,2,50,1,COLW-1,COLH-2);
                    BeepOFF;
                    break;
                case 3:
                    BeepON;
                    if(dl1a_distance_mm>2000)
                    {
                        *state=0;
                        GyroOn=0;
                        BeepOFF;
                        speed_base=155;
                        road_type.Ramp=0;

                    }
                    break;
            }
    }
/*
十字处理函数：
imageOut:放入保存边界的数组
state:十字标志位
 */
void CrossProcess(int imageOut[COLH][2],uint8* state)
{
    switch(*state)
    {
        case 1:if(SideIsNoLost(RoadLostFlag,30,15,1,8)&&SideIsNoLost(RoadLostFlag,30,15,2,8))
        {

        }

    }
}
