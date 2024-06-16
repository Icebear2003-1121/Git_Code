#ifndef _ImageProcess_h
#define _ImageProcess_h

#include "zf_common_headfile.h"
#include "camera.h"
#include "ScanLine.h"
#include "Key.h"
#include "Steer.h"


extern uint8 RoundFlag;
extern uint8 RoundFlag;
extern uint8 OpenRoadFlag;
extern uint8 BarrierFlag;
extern uint8 ZebraFlag;
extern uint8 RampFlag;
extern uint8 Elements_flag;


void ElementsJudge();
void RoundProcess(int imageOut[LCDH][2],uint8* state);
uint8 RoundaboutLine(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index);
uint8 SideIsNoLost(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 index);
uint8 ForkaboutGetArc(int imageOut[LCDH][2], uint8 imageStart, uint8 imageEnd, uint8 status, uint8 num);
uint8 RoadSideMono(int imageIn[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status);
uint8 SideColumnSpace(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 status,uint8 num,uint8 index);
uint8 TrackWidth(int imageOut[LCDH][2],uint8 imageStart,uint8 imageEnd,uint8 num,uint8 index);
void Depart(uint8 Dir);
uint8 whitehang(uint8 imageInput[LCDH][LCDW],uint8* endline);
void ZebraProcess(int imageOut[LCDH][2],uint8* state);
uint8 ZebraJudge(uint8 imageOut[LCDH][LCDW],uint8 center,uint8 imageStart,uint8 imageEnd,uint8 status);
void Anchorsite(int a[][3],int cur,int step,uint8 mode);
float Q_rsqrt(float number);
uint8 UpdownSideGet(uint8 imageInput[LCDH][LCDW],uint8 startX, uint8 startY, uint8 endX, uint8 endY,int imageOut[LCDW],uint8 index);
uint8 TofContinue(uint16 tofdistance,uint16 distance,uint8 status);
void ImageAddingLine(int imageOut[LCDH][2], uint8 status, int startX, int startY, int endX, int endY);
void RampProcess(uint8* state);


#endif






