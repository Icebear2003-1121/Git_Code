#include "ImageProcess.h"

static uint8 g_ucFlagRound=0;
static uint8 g_ucFlagZebraFlag=1;
static uint8 g_ucFlagOpening=0;
static uint8 g_ucFlagBarrier=0;
static uint8 g_ucFlagZebra=0;
static uint8 g_ucFlagRamp=0;
uint8 RoundFlag;
uint8 RoundFlag;
uint8 OpenRoadFlag;
uint8 BarrierFlag=0;
uint8 ZebraFlag=0;
uint8 RampFlag=0;
uint8 Elements_flag=0;
uint8 fock;
uint8 gptflag=0;
int a,b,c,d,e,f;
void ElementsJudge()
{
//    SideIsNoLost(RoadLostFlag,55,25,1,5)&&!SideIsNoLost(RoadLostFlag,LCDH-1,40,2,5)
//    ||(SideIsNoLost(RoadLostFlag,55,25,2,5)&&!SideIsNoLost(RoadLostFlag,LCDH-1,40,1,5))
    if(ZebraJudge(Bin_Image,LCDW/2,55,25,12)/*||ZebraJudge(Bin_Image,LCDW/2+5,55,15,10)||ZebraJudge(Bin_Image,LCDW/2-6,55,15,10)*/)
        {
              if(g_ucFlagRound==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagZebraFlag==1&&g_ucFlagRamp==0&&g_ucFlagOpening==0)
              {
                    BeepON
                    if(LeftGo)//左车库
                    {
            //                speed_base=40;
                         gpt_count_init();
                         RoundFlag=0;
                         g_ucFlagRound=0;
                         g_ucFlagOpening=0;
            //                OpenRoadFlag=0;
                         g_ucFlagZebra=21;
                         g_ucFlagZebraFlag=1;
        //                    printf("Zebra\n");
                     }

                     else if(RightGo)//右车库
                     {
                         gpt_count_init();
            //                speed_base=40;
                          RoundFlag=0;
                          g_ucFlagRound=0;
                          g_ucFlagOpening=0;
            //                OpenRoadFlag=0;
                          g_ucFlagZebra=22;
                          g_ucFlagZebraFlag=1;
        //                    printf("Zebra\n");
                      }
                  }
    //          printf("%d\n",g_ucFlagZebra);
            }

             if(g_ucFlagZebra)Elements_flag=1;
             else Elements_flag=0;

             if(g_ucFlagZebra)
             {
//                 Anchorsite(curvature,33,2,1);
                  ZebraProcess(ImageSide,&g_ucFlagZebra);
                  ZebraFlag=g_ucFlagZebra;
             }

             ////////////////////////////////////////圆环判断start////////////////////////////////////////
                  //左圆环//
//                   a=RoundaboutLine(ImageSide,35,20,2,50/*57*/,3);
     //                     b=RoundaboutLine(ImageSide,LCDH-1,50,2,70,1);
     //                     c=SideIsNoLost(RoadLostFlag,45,20,1,5);
     //                     d=!SideIsNoLost(RoadLostFlag,50,20,2,3);
     //                     e=!ForkaboutGetArc(ImageSide,LCDH-2,20,2,3);
     //                     f=RoadSideMono(ImageSide,50,20,2);
     ////                     g=SideColumnSpace(ImageSide,LCDH-2,40,2,1,3);
//                          ips114_show_uint(0, 0, a, 3);
     //                     ips114_show_uint(0, 20,b, 3);
     //                     ips114_show_uint(0, 40, c, 3);
     //                     ips114_show_uint(0, 60, d, 3);
     //                     ips114_show_uint(0, 80, e, 3);
     //                     ips114_show_uint(0, 100, f, 3);
     //                     ips114_show_uint(20, 0, g, 3);
                 if(RoundaboutLine(ImageSide,35,20,2,57/*57*/,3)&&RoundaboutLine(ImageSide,LCDH-1,50,2,74,1)&&SideIsNoLost(RoadLostFlag,45,20,1,5)//第15行到第30行右边界小于3条小于55,第50行到最后一行右边界小于74,第20到第45行左边界连续丢线大于5
                         &&!SideIsNoLost(RoadLostFlag,50,20,2,3)&&!ForkaboutGetArc(ImageSide,LCDH-2,20,2,3)&&RoadSideMono(ImageSide,50,20,2)==2//第15到第50行右边界连续丢线小于等于3行,第15到倒数第2行右边界没有拐点,第15到第50右边界单调减
                         )//第40到倒数第二行左边界列间距大于1的行数小于等于3.满足以上条件,进入左圆环
                 {
                     if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)//所有元素为0
                     {
                         g_ucFlagRound=1;//圆环标志位为1
                     }
                 }
                 //右圆环//
                 if(RoundaboutLine(ImageSide,35,22,1,45/*43*/,3)&&RoundaboutLine(ImageSide,LCDH-1,50,1,25,1)&&SideIsNoLost(RoadLostFlag,45,20,2,5)//第15行到第30行左边界小于3条大于47，第50行到最后一行右边界不大于25，第20到第45行右边界连续丢线大于5
                         &&!SideIsNoLost(RoadLostFlag,55,20,1,3)&&!ForkaboutGetArc(ImageSide,LCDH-2,20,1,3)&&RoadSideMono(ImageSide,50,20,1)==1//第15到第50行左边界连续丢线小于等于3行,第15到倒数第2行左边界没有拐点,第15到第50左边界单调增
                         )//第40到倒数第二行右边界列间距大于1的行数大于3.满足以上条件,进入右圆环
                 {
     //                if(!gptflag)
     //                {
     //                 gpt_count_init();
     //                 gptflag=1;
     //                }
                     if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
                     {
                         g_ucFlagRound=2;//圆环标志位为2
                     }
                 }
            ////////////////////////////////////////圆环判断end////////////////////////////////////////////
            ////////////////////////////////////////坡道判断start//////////////////////////////////////////
           //         a=TofContinue(dl1a_distance_mm,800,1);
           //         b=!SideIsNoLost(RoadLostFlag,35,20,1,1);
           //         c=!SideIsNoLost(RoadLostFlag,35,20,2,1);
           //         d=!UpdownSideGet(Bin_Image,20,LCDH-2, 80,15,UpSide,4);
           //         e=RoundaboutLine(ImageSide,30,15,1,45/*43*/,3);
           //         f=RoundaboutLine(ImageSide,LCDH-1,50,1,25,1);
           //         g=RoundaboutLine(ImageSide,30,15,2,55/*57*/,3);
           //         h=RoundaboutLine(ImageSide,LCDH-1,50,2,74,1);
           //                             ips114_show_uint(0, 0, a, 3);
           //                             ips114_show_uint(0, 20,b, 3);
           //                             ips114_show_uint(0, 40, c, 3);
           //                             ips114_show_uint(0, 60, d, 3);
           //                             ips114_show_uint(0, 80, e, 3);
           //                             ips114_show_uint(0, 100, f, 3);
           //                             ips114_show_uint(20, 0, g, 3);
           //                             ips114_show_uint(20, 20, h, 3);
            if(TofContinue(dl1a_distance_mm,800,1)&&!SideIsNoLost(RoadLostFlag,35,20,1,1)&&!SideIsNoLost(RoadLostFlag,35,20,2,1)&&!UpdownSideGet(Bin_Image,20,LCDH-2, 80,15,UpSide,4)
                &&RoundaboutLine(ImageSide,30,15,1,45/*43*/,3)&&RoundaboutLine(ImageSide,LCDH-1,50,1,25,1)&&RoundaboutLine(ImageSide,30,15,2,55/*57*/,3)&&RoundaboutLine(ImageSide,LCDH-1,50,2,74,1))

            {
                if(g_ucFlagRound==0&&g_ucFlagOpening==0&&g_ucFlagBarrier==0&&g_ucFlagZebra==0&&g_ucFlagRamp==0)
                {
                     speed_base=80;
                     g_ucFlagRamp=1;
                     BeepON;
    //                 printf("ramp\n");
                }
            }
            ////////////////////////////////////////坡道判断end////////////////////////////////////////////
            if(g_ucFlagRound||g_ucFlagRamp||g_ucFlagOpening||g_ucFlagBarrier)
            {
                Elements_flag=1;
            }
            else
            {
                Elements_flag=0;
            }
            if(g_ucFlagRound)//进入圆环
            {
                SteerPid.Kp=37;
                SteerPid.Kd=30;
//                Anchorsite(curvature,29,2,3);
                RoundProcess(ImageSide,&g_ucFlagRound);
//                ips114_show_uint(20, 0, g_ucFlagRound, 3);
                speed_base=155;
                RoundFlag=g_ucFlagRound;
            }
            if(g_ucFlagRamp)
             {
                 RampProcess(&g_ucFlagRamp);
                 RampFlag=g_ucFlagRamp;
             }
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
uint8 RoundaboutLine(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index)
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
判断边界是否连续丢线
imageOut:放入保存丢边信息的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
index:连续丢失多少行，大于index返回1,小于等于index返回0
*/
uint8 SideIsNoLost(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 index)
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
判断前方是否有拐点（连续）
imageOut:二值化后的图像数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
num:判断拐点的条件,连续单调增和连续单调减的行数
返回:无拐点:0,有拐点:1
*/
uint8 ForkaboutGetArc(int imageOut[LCDH][2], uint8 imageStart, uint8 imageEnd, uint8 status, uint8 num)
{
    int i = 0;
    uint8 inc = 0, dec = 0, flag = 0;
    uint8 inc2 = 0, dec2 = 0, flag2 = 0;
    switch (status)
    {
    case 1:
        for (i = imageStart; i > imageEnd - 1; i--)
        {
            if (imageOut[i][0] > imageOut[i + 1][0])
            {
                flag = 0;
                dec = 0;
                inc++;
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
                    if (dec > num)
                    {
                        return 1;
                    }
                }
            }

            if (imageOut[i][0] >= imageOut[i + 1][0])
            {
                flag2 = 0;
                dec2 = 0;
                inc2++;
                if (inc2 > num)
                {
                    flag2 = 1;
                }
            }
            else
            {
                inc2 = 0;
                if (flag2)
                {
                    dec2++;
                    if (dec2 > num)
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
            if (imageOut[i][1] < imageOut[i + 1][1])
            {
                flag=0;
                dec = 0;
                inc++;
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
                    if (dec > num)
                    {
                        return 1;
                    }
                }
            }

            if (imageOut[i][1] <= imageOut[i + 1][1])
            {
                flag2=0;
                dec2 = 0;
                inc2++;
                if (inc2 > num)
                {
                    flag2 = 1;
                }
            }
            else
            {
                inc2 = 0;
                if (flag2)
                {
                    dec2++;
                    if (dec2 > num)
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
判断边界单调性
imageIn:放入保存边界的数组
imageStart:从哪行开始判断
imageEnd:哪行结束判断
imageStart>imageEnd
status:判断哪条边,左边界:1,右边界:2
返回:不单调or错误:0,单调递增:1,单调递减:2
*/
uint8 RoadSideMono(int imageIn[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status)
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
                if (num >= (imageStart-imageEnd)*4/5)
                    return 1;
            }

            for(i = imageStart; i > imageEnd+1; i--)
            {
                if(imageIn[i][0] >= imageIn[i-1][0])
                    num++;
                else
                    num = 0;
                if (num >= (imageStart-imageEnd)*4/5)
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
                if (num >= (imageStart-imageEnd)*4/5)
                    return 1;
            }

            for(i = imageStart; i > imageEnd+1; i--)
            {
                if(imageIn[i][1] >= imageIn[i-1][1])
                    num++;
                else
                    num = 0;
                if (num >= (imageStart-imageEnd)*4/5)
                    return 2;
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
uint8 SideColumnSpace(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index)
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
圆环处理函数：
imageOut:放入保存边界的数组
state:圆环标志位
 */
void RoundProcess(int imageOut[LCDH][2],uint8* state)
{
    switch(*state)
    {
        //////////////////////////////////////////////////////////////左环/////////////////////////////////////////////////////////////////////////
        case 1:
                            if(SideIsNoLost(RoadLostFlag,60,45,1,12))
                            {

                //                if(!RoundaboutLine(ImageSide,22,12,2,50,3))
                //                {
                //                    *state=0;
                //                    break;=        //                }
                                *state=3;
                                ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                                BeepON;
                            }
                            BeepON;
                            ImageGetSideElementsJudge(Bin_Image,ImageSide,1);
                            break;
                        case 3:
                            if(!SideIsNoLost(RoadLostFlag,55,50,1,1))
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
                                *state=7;
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

                            //////////////////////////////////////////////////////////////右环/////////////////////////////////////////////////////////////////////////
                            case 2:
                                if(SideIsNoLost(RoadLostFlag,LCDH-1,45,2,12))
                                {
                                    *state=4;
                                    ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                                    BeepON;
                                }
                                BeepON;
                                ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                                break;

                            case 4:
                                if(!SideIsNoLost(RoadLostFlag,LCDH-1,45,2,1))
                                {
                                    Anchorsite(curvature,32,2,1);
                                    Anchorsite(curvature,32,2,3);
                    //                FPIDChang(CurPid,9,0,6);
                                    *state=6;
                                    ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                                    BeepOFF;
                                }
                                BeepON;
                                ImageGetSideElementsJudge(Bin_Image,ImageSide,0);
                                break;

                            case 6://环内
                                if(ForkaboutGetArc(ImageSide,50,22,1,3)&&TrackWidth(ImageSideRound,LCDH-4,30,10,90)&&!SideIsNoLost(RoadLostFlag,40,22,1,2))
                                {
                                    *state=8;
                                    ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                                    BeepON;
                                }
                                BeepOFF;
                                ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                                break;

                    //        case 8://环内
                    //            if(ForkaboutGetArc(ImageSide,50,22,1,3)&&TrackWidth(ImageSideRound,LCDH-4,30,10,85)&&!SideIsNoLost(RoadLostFlag,50,22,1,3))
                    //            {
                    //                *state=6;
                    //                ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                    //                BeepOFF;
                    //            }
                    //            BeepON;
                    //            ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                    //            break;

                            case 8:
                                if(!SideIsNoLost(RoadLostFlag,45,20,1,2)&&!ForkaboutGetArc(ImageSide,50,18,1,2)&&SideIsNoLost(RoadLostFlag,LCDH-1,50,1,5))//&&!SideIsNoLost(RoadLostFlag,50,35,2,1)/*&&!SideIsNoLost(RoadLostFlag,35,20,2,1)*/)
                                {
                                    *state=10;
                    //                ImageGetSideRoundOut(Bin_Image,ImageSide,0);
                                    BeepOFF;
                                }
                                BeepON;
                                ImageGetSideRoundArc(Bin_Image,ImageSide,1);
                                break;

                            case 10:
                                if(SideIsNoLost(RoadLostFlag,LCDH-1,45,2,12))
                                {
                                    *state=12;
                                    ImageGetSideRoundOut(Bin_Image,ImageSide,0);
                                    BeepOFF;
                                }
                                ImageGetSideRoundOut(Bin_Image,ImageSide,0);
                                break;

                            case 12:
                                if(!SideIsNoLost(RoadLostFlag,LCDH-1,25,2,1)&&!SideIsNoLost(RoadLostFlag,LCDH-1,25,1,1))
                                {
                                    BeepOFF;
                                    speed_base=150;
                                    Anchorsite(curvature,25,2,1);
                                    Anchorsite(curvature,25,2,3);
                    //                FPIDChang(CurPid,16,0,100);
                                    *state=0;
                                }
                                BeepOFF;
                                ImageGetSideRoundOut(Bin_Image,ImageSide,0);
                                break;
                                }

}
/*
 * 判断道路宽度
 * imageOut：边线数组
 * imageStart>imageEnd
 *
 * */
uint8 TrackWidth(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 num,uint8 index)
{
    uint8 y,n=0;
    int ActRoadWidth[LCDH];

    for(y=imageStart;y>imageEnd;y--)
    {
        ActRoadWidth[y]=imageOut[y][1]-imageOut[y][0];
        if(ActRoadWidth[y]<=index)
        {
            n++;
        }
    }

    if(n>=num)
    {
        return 1;
    }
    return 0;
}
/*
 * 出库函数
 * Dir:1 左出库  2 右出库
 * */
void Depart(uint8 Dir)
{
    gpio_set_level(MOTOR1_DIR, 1);
    gpio_set_level(MOTOR2_DIR, 0);
    switch (Dir)
    {
        case 1:
            if(gptL_Cir>30)
            {
                Car=1;
            }
            pwm_set_duty(S_MOTOR_PIN, 680);
            motor_control(3000,5500);
            break;
        case 2:
            pwm_set_duty(S_MOTOR_PIN, 560);
            if(gptL_Cir>70)
            {
                Car=1;
            }
            motor_control(5500,3000);
            break;
    }
}

/*
 * 计算中间列最长白点数
 * imageInput[LCDH][LCDW]:二值化图像
 * * endline：截止行 &DecsionEndLine
 * */
uint8 whitehang(uint8 imageInput[LCDH][LCDW],uint8* endline)
{
    int y=0;
    uint8 blacknum=0;
    for(y=LCDH-2;y>0;y--)//扫描
    {
        if(imageInput[y-1][(LCDW-1)/2]&&imageInput[y-1][(LCDW-1)/2-1]&&imageInput[y-1][(LCDW-1)/2+1])//白色
        {
            *endline=y-1;
            return blacknum;
        }
        else
        {
            blacknum++;
        }

    }
    return blacknum;
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
uint8 ZebraJudge(uint8 imageOut[LCDH][LCDW],uint8 center,uint8 imageStart,uint8 imageEnd,uint8 status)//Bin_Image 30 10 12
{
    uint8 num=0,index=0,flag=0;
    uint8 y,x;
    uint8 imageFlag[LCDW];
    uint8 imagePoint[LCDW];
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
        if(flag>4)
        {
            flag=0;
            return 1;
        }
    }
    return 0;
}

/*
斑马线处理函数：
imageOut:放入保存边界的数组
state:斑马线标志位
 */
void ZebraProcess(int imageOut[LCDH][2],uint8* state)
{
    switch(*state)
    {
        case 21:
            BeepON;
            if(gptL_Cir>0&&gptR_Cir>70)
            {
                ZebraStop=1;
                *state=50;
                BeepOFF;
            }
            break;

        case 22:
            BeepON;
            if(gptR_Cir>0&&gptL_Cir>60)
            {
                ZebraStop=1;
                *state=51;
                BeepOFF;
            }
            break;

        case 50:
            RoadOut_flag=1;
            break;

        case 51:
            RoadOut_flag=1;
            break;
    }
}

/*
 改前瞻函数
a:前瞻数组
cur:中间点的行数
step:相邻两个相差
mode:速度决策时的速度模式
 例
Anchorsite(curvature,28,2,3);
Anchorsite(curvatureUp,28,2,3);
*/
void Anchorsite(int a[][3],int cur,int step,uint8 mode)
{
    a[mode][0]=cur-step;
    a[mode][1]=cur;
    a[mode][2]=cur+step;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       快速平方根倒数算法
// @param       float number
// @return      y
// Sample usage:                Q_rsqrt(9);
//-------------------------------------------------------------------------------------------------------------------
float Q_rsqrt(float number)
{
 long i;
 float x2,y;
 const float threehalfs = 1.5F;
 x2= number * 0.5F;
 y= number;
 i= * (long * ) &y;
 i= 0x5f3759df-(i>>1);
 y= * (float * ) &i;
 //上述代码给牛顿迭代法提供了一个较好的初始值
 y=y * (threehalfs - (x2*y*y));  //牛顿迭代一次
 y=y * (threehalfs - (x2*y*y));  //  可持续迭代
 return y;
}

/*
 * 多次判断测距
 * tofdistance:tof实际值
 * distance:设定值
 * status:1 有障碍   2 没障碍
 * 最大8191（没障碍）
 */
uint8 TofContinue(uint16 tofdistance,uint16 distance,uint8 status)
{
    int num=0,i=0;

    switch(status)
    {
        case 1:
            for(i=0;i<3;i++)
            {
                if(tofdistance<distance)
                {
                    num++;

                }
            }
            if(num>2)
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case 2:
            for(i=0;i<3;i++)
            {
                if(tofdistance>distance)
                {
                    num++;

                }
            }
            if(num>2)
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
    }

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
uint8 UpdownSideGet(uint8 imageInput[LCDH][LCDW],uint8 startX, uint8 startY, uint8 endX, uint8 endY,int imageOut[LCDW],uint8 index)
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
        if(imageOut[x]==0||(x>startX&&abs(imageOut[x]-imageOut[x-1])>index))
        {
            return 0;
        }
    }
    return 1;
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
void ImageAddingLine(int imageOut[LCDH][2], uint8 status, int startX, int startY, int endX, int endY)
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
            if(TofContinue(dl1a_distance_mm,2000,2))
            {
                *state=2;
                speed_base=100;
                BeepOFF;
            }
            BeepON;
            break;
        case 2:
            if(TofContinue(dl1a_distance_mm,1400,1))
            {
                *state=3;
                speed_base=155;
                BeepON;
            }
//            ImageAddingLine(ImageSide,1,50,1,0,LCDH-2);
//            ImageAddingLine(ImageSide,2,50,1,LCDW-1,LCDH-2);
            BeepOFF;
            break;
        case 3:
            BeepON;
            if(TofContinue(dl1a_distance_mm,1000,2))
            {
                *state=0;
                BeepOFF;
            }
            break;
    }
}










