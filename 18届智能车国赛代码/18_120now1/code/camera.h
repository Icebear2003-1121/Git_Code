/*
 * camera.h
 *
 *  Created on: 2023Äê3ÔÂ20ÈÕ
 *      Author: ÈþÕÙa
 */

#ifndef CODE_CAMERA_H_
#define CODE_CAMERA_H_

#include "zf_common_headfile.h"

extern uint8 Image_Use[COLH][COLW];
extern uint8 Bin_Image[COLH][COLW];
extern int CollectTim;
void Test_CAMERA(void);
void Get_Use_Image(void);
void Bin_Image_Filter(void);
void Get_Use_Image1(uint8 *imageIn,uint8 imageOut[COLH][COLW]);
void Get_Bin_Image(unsigned char mode);
short Get_OSTU(unsigned char Image[COLH][COLW]);
void robert(unsigned char imageIn[COLH][COLW], unsigned char imageOut[COLH][COLW], unsigned char Threshold);
uint8 adapt_threshold(uint8 *image, uint16 startW, uint16 endW);
void EdgeTrack(int x, int y);
void Canny(int input[][COLH], int output[][COLH], int width, int height, double low_threshold, double high_threshold);
void sobel (unsigned char imageIn[COLH][COLW], unsigned char imageOut[COLH][COLW], unsigned char Threshold);
void Blacking();
extern int SpeedMode;
//typedef enum
//{
//    mydefine,
//    prewitt
//}Gradient;
typedef struct
{
    uint8  *imgsrc;
    uint8  imgfliter[COLW*COLH];
    uint8  imgout[COLW*COLH];
    uint32 width;
    uint32 heigh;
    float Xgrad[COLW*COLH];
    float Ygrad[COLW*COLH];
    float grad[COLW*COLH];
    float Seita[COLW*COLH];
}IMG_Typedef;
extern IMG_Typedef img_Handle;
void Get_Width_Road();
void image_canny(IMG_Typedef *image);




#endif /* CODE_CAMERA_H_ */
