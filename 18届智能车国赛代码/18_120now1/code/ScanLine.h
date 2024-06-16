/*
 * ScanLine.h
 *
 *  Created on: 2023Äê3ÔÂ24ÈÕ
 *      Author: ÈþÕÙa
 */

#ifndef CODE_SCANLINE_H_
#define CODE_SCANLINE_H_


#include "zf_common_headfile.h"


extern int ImageSide[COLH][2];
extern int RoadLostFlag[COLH][2];
extern int UpSide[COLW];
extern int MidLine[COLH];
extern uint8 Bounds[60][2];
extern uint8 RoadOut_flag;
extern uint8 Block,BlockBar;
extern uint8 Out_flag;
void ImageGetSide(uint8 imageInput[COLH][COLW],int imageOut[COLH][2],int imageLost[COLH][2]);
void ImageGetMidLine(int imageInput[COLH][2],int imageOut[COLH]);
void ImageGetSideElementsJudge(uint8 imageInput[COLH][COLW],int imageOut[COLH][2],uint8 status);
void ImageGetSideRoundArc(uint8 imageInput[COLH][COLW],int imageOut[COLH][2],uint8 status);
void ImageGetSideRoundOut(uint8 imageInput[COLH][COLW],int imageOut[COLH][2],uint8 status);
void ImageSideTransform(int imageInput[COLH][2],uint8 imageOut1[COLH],uint8 imageOut2[COLH]);
void Outside_protect(uint8 value);




#endif /* CODE_SCANLINE_H_ */
