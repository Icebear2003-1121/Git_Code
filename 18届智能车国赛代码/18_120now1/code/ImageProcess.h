/*
 * ImageProcess.h
 *
 *  Created on: 2023��3��24��
 *      Author: ����a
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
/************************�ṹ�����**************************/
//�������ͽṹ��
struct XkElement{
     int16 Ramp;                   //�µ�
     int16 barrier;                //���
     int16 straight;               //ֱ��
     int16 bend;                   //���
     int16 Cross;                  //ʮ��
     int16 LeftCirque;             //�󻷵�
     int16 RightCirque;            //�һ���
     int16 LeftOut;                //�����
     int16 RightOut;               //���ҿ�
     int16 Leftin;                 //�����
     int16 Rightin;                //���ҿ�
     int16 Open_road;              //��·
     int16 StrGo;                  //��ֱ����
};
extern struct XkElement road_type, stage, annulus, jiaodu, xianshi, speed ,Xk;
//��־λ�ṹ��
struct  FLAG
{
        int8 init_ok;            //��ʼ���ɹ���־λ
        int8 start;              //�����ɹ���־λ
        int8 strategy1;          //�ٶȲ���1��־λ
        int8 strategy2;          //�ٶȲ���2��־λ
        int8 strategy3;          //�ٶȲ���3��־λ
        int8 strategy4;          //�ٶȲ���4��־λ
        int8 adjust_ok;          //������ɱ�־λ
        int8 t_ms;               //�жϻ�׼ʱ���־λ
        int8 msg_collect;        //��Ϣ�ɼ���־λ
        int8 speed;              //�ٶȻ���־λ
        int8 direction;          //���򻷱�־λ
        int8 open_check_ku;      //�����־λ
        int8 stop;               //ͣ����־λ
        int8 image_show;         //ͼ����ʾ��־λ
        int8 eeprom_check_ok;
        int8 barrierDir;         //��Ϸ���
};
extern struct FLAG flag;





#endif /* CODE_IMAGEPROCESS_H_ */
