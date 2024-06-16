#ifndef _camera_h
#define _camera_h

#include "zf_common_headfile.h"
#include"Key.h"

extern uint8 Erosion[LCDH][LCDW];
extern uint8 Image_Use[LCDH][LCDW];
extern uint8 Bin_Image[LCDH][LCDW];

//extern uint16 ThresholdSobel;
//extern uint16 ThresholdOstu;
extern uint16 ExposureTime;
extern uint16 ImageGain;

extern uint8 LeftSide[LCDH]; // 存储左侧边缘的数组，长度为LCDH
extern uint8 RightSide[LCDH]; // 存储右侧边缘的数组，长度为LCDH

void ImageGetMidLine(int imageInput[LCDH][2],int imageOut[LCDH]);
void Test_CAMERA(void);
void Get_Use_Image(uint8 *imageIn,uint8 imageOut[LCDH][LCDW]);
void Bin_Image_Filter(void);
//void Get_Bin_Image(void);
void sobel (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold);
void Get_Bin_Image(unsigned char mode);
void robert(unsigned char imageIn[COLH][COLW], unsigned char imageOut[COLH][COLW], unsigned char Threshold);
void camera_Init(void);
void Get_Width_Road();

#endif





