/*
 * ImageProcess.h
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */

#ifndef CODE_IMAGEPROCESS_H_
#define CODE_IMAGEPROCESS_H_

#include "zf_common_headfile.h"
#include "camera.h"
#include "ScanLine.h"
#include "control.h"


#define Yaw3 -0.9
#define YawZ -2

extern uint8 Elements_flag;
extern uint8 RoundFlag;
extern uint8 OpenRoadFlag;
extern uint8 BarrierFlag;
extern uint8 ZebraFlag;
extern uint8 RampFlag;
extern uint8 sudu_yingzi;
extern float BarrierIn;
extern float BarrierOut;
int ElementsJudge();
int Element_Handle();
void Handle_Barn_in();
//void Handle_Barn_Out(uint8 type);
int WhitePointNum(uint8 imageInput[COLH][COLW],uint8 startX,uint8 startY,uint8 endX,uint8 endY);
int WhitePointNumUP(uint8 imageInput[COLH][COLW],uint8 startX,uint8 startY,uint8 endX,uint8 endY);
uint8 PointsDistance(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 index,uint8 distance,uint8 count);
uint8 SideIsNoLost(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 index);
uint8 RoundaboutLine(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index);
uint8 RoadSideMono(int imageIn[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status);
uint8 RoundaboutGetArc(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8* index);
uint8 SideColumnSpace(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index);
uint8 SideColumnSpaceContinue(int imageOut[COLH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index);
uint8 ZebraJudge(uint8 imageOut[COLH][COLW],uint8 center,uint8 imageStart,uint8 imageEnd,uint8 status);
uint8 ForkaboutGetArc(int imageOut[COLH][2], uint8 imageStart, uint8 imageEnd, uint8 status, uint8 num);
void ImageAddingLine(int imageOut[COLH][2], uint8 status, int startX, int startY, int endX, int endY);
uint8 ImageAddingLineCompare(int imageOut[COLH][2],uint8 status,int startX,int startY,int endX,int endY,uint8 num);
uint8 AveGray(uint8 imageOut[COLH][COLW],int startX, int startY, int endX, int endY,uint16 GrayThreshold);
uint8 UpdownSideGet(uint8 imageInput[COLH][COLW],uint8 startX, uint8 startY, uint8 endX, uint8 endY,int imageOut[COLW]);
uint8 Track_closure(uint8 imageInput[COLH][COLW],uint8 startX,uint8 endX,int imageOut[COLW], uint8 MaxY);
uint8 Track_closure1(uint8 imageInput[COLH][COLW],uint8 startX,uint8 endX,uint8 imageOut[COLW],int MaxY);
uint8 RoadSideUpMonoBTS(int imageIn[COLW],uint8 imageStart,uint8 imageEnd);
uint8 RoadSideUpMonoSTB(int imageIn[COLW],uint8 imageStart,uint8 imageEnd);
uint8 White_Judgment(uint8 imageInput[COLH][COLW],uint8 status, uint8 X,uint8 white_num);
void QianZhang(uint8 type);
uint8 Tututu(uint8 type);
void Check_Zhidao();
int16 Distance_Measure(void);
void RoundProcess(int imageOut[COLH][2],uint8* state);
void CrossProcess(int imageOut[COLH][2],uint8* state);
void BarrierProcess(int imageOut[COLH][2],uint8* state);
void OpeningProcess(int imageOut[COLH][2],uint8* state);
void ZebraProcess(int imageOut[COLH][2],uint8* state);
void RampProcess(uint8* state);
/************************结构体变量**************************/
//赛道类型结构体
struct XkElement{
     int16 Ramp;                   //坡道
     int16 barrier;                //横断
     int16 straight;               //直道
     int16 bend;                   //弯道
     int16 Cross;                  //十字
     int16 LeftCirque;             //左环岛
     int16 RightCirque;            //右环岛
     int16 LeftOut;                //出左库
     int16 RightOut;               //出右库
     int16 Leftin;                 //入左库
     int16 Rightin;                //入右库
     int16 Open_road;              //断路
     int16 StrGo;                  //笔直发车
};
extern struct XkElement road_type, stage, annulus, jiaodu, xianshi, speed ,Xk;
//标志位结构体
struct  FLAG
{
        int8 init_ok;            //初始化成功标志位
        int8 start;              //发车成功标志位
        int8 strategy1;          //速度策略1标志位
        int8 strategy2;          //速度策略2标志位
        int8 strategy3;          //速度策略3标志位
        int8 strategy4;          //速度策略4标志位
        int8 adjust_ok;          //调参完成标志位
        int8 t_ms;               //中断基准时间标志位
        int8 msg_collect;        //信息采集标志位
        int8 speed;              //速度环标志位
        int8 direction;          //方向环标志位
        int8 open_check_ku;      //检测库标志位
        int8 stop;               //停车标志位
        int8 image_show;         //图像显示标志位
        int8 eeprom_check_ok;
        int8 barrierDir;         //横断方向
};
extern struct FLAG flag;





#endif /* CODE_IMAGEPROCESS_H_ */
