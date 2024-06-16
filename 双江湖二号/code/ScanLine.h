#ifndef _ScanLine_h
#define _ScanLine_h

#include "zf_common_headfile.h"
#include "camera.h"

extern int rightlostpointnum,leftlostpointnum;
extern int ImageSideEro[LCDH][2];
extern int ImageSide[LCDH][2],ImageSideRound[LCDH][2];
extern int RoadLostFlag[LCDH][2];
extern int UpSide[LCDW];
extern int MidLine[LCDH];
extern uint8 Bounds[60][2];
extern uint8 RoadOut_flag;
extern uint8 Block,BlockBar;

void ImageGetSide(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],int imageLost[LCDH][2]);
void ImageGetMidLine(int imageInput[LCDH][2],int imageOut[LCDH]);
void ImageGetSideElementsJudge(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status);
void ImageGetSideRoundArc(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status);
void ImageGetSideRoundOut(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status);
void ImageGetSideRound(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2]);
void ImageGetbounds (int imageInput[LCDH][LCDW],int imageOut[LCDH][2]);
void ImageSideTransform(int imageInput[LCDH][2],uint8 imageOut1[LCDH],uint8 imageOut2[LCDH]);

#endif
