/*
 * camera.c
 *
 *  Created on: 2023年3月20日
 *      Author: 叁召a
 */
#include "camera.h"

uint8 Image_Use[COLH][COLW];// 原始图像 or 压缩图像
uint8 Bin_Image[COLH][COLW];// 二值化图像
uint8 Tim_Track_Width[COLH];//实时图像宽度
uint8 LeftSide[COLH];
uint8 RightSide[COLH];
int CollectTim=0;//收集时间变量
int SpeedMode;
int TimeFlag = 0;

void Test_CAMERA(void)
{

    if(mt9v03x_finish_flag)
    {

        if(TimeFlag)
        {
            system_start();// 启动定时器，记录下当前的时间
        }
        Get_Use_Image1(*mt9v03x_image,Image_Use);//图像压缩
        Get_Bin_Image(0);//得到二值化
        Bin_Image_Filter();//去噪
        ImageGetSide(Bin_Image,ImageSide,RoadLostFlag);//得到边界
        GyroResolve();
//       Outside_protect(95);
        ElementsJudge();//元素判断
//        ImageSideTransform(ImageSide,LeftSide,RightSide);//打印边界用于在上位机中显示c
        ImageGetMidLine(ImageSide,MidLine);//得到中线
        bias_curvature();//得到偏差
        SpeedControl(SpeedMode);
//#ifdef IPS200
//        if(show_tuxiang)
//        {
//            ips200_clear();
//            ips200_show_gray_image(0, 0, Bin_Image[0], COLW, COLH,COLW,COLH,0);
//        }
//        ips200_show_string(10,    80, "ZebraFlag");
//        ips200_show_string(10,    60, "yaw");
//        ips200_show_string(10,    40, "curvature_mid");
//        ips200_show_float(150, 80, ZebraFlag, 3, 2);
//        ips200_show_float(150, 60, yaw, 3, 2);
//        ips200_show_float(150, 40, curvature_mid, 3, 2);
//#endif
//        ZW_Send_Image((ZW_uint8*) Image_Use);
//        sendimg(Image_Use,COLW,COLH);
//         sendline(3,LeftSide,COLH);//发送左边线
//         sendline(3,RightSide,COLH);//发送右边线
//         sendline(2,MidLine,COLH);//发送中线
//        ips114_displayimage03x(mt9v03x_image[0],COLW,COLH);
//        ips114_show_gray_image(50, 50, mt9v03x_image[0], COLW, COLH, COLW, COLH,0);
//        Blacking();
//        ips114_show_gray_image(0,0,(uint8 *)Bin_Image,COLW,COLH,COLW,COLH,0);
//        ips200_show_gray_image(0, 0, Bin_Image[0], COLW, COLH,COLW,COLH,0);
        //ips200_show_gray_image(0, COLH + 10, mt9v03x_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
#ifdef oled
        if(show_tuxiang)
        {
            oled_show_gray_image(0,0,(uint8 *)Bin_Image,COLW,COLH,COLW,COLH,0);
        }
        if(TimeFlag)
        {
            CollectTim=system_getval_ms();//获取当前计时时间  单位us
            oled_show_int(50,2,CollectTim,3);
        }
#endif
        mt9v03x_finish_flag = 0;
   }
}
/*---------------------------------------------------------------
 【函    数】Get_Use_Image1
 【功    能】图像压缩
 【参    数】原始采集图像，和存储图像数组
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Get_Use_Image1(uint8 *imageIn,uint8 imageOut[COLH][COLW])      //图像压缩
{
    uint32 i,j;
    for(j=0;j<COLH;j++)
    {
        for(i=0;i<COLW;i++)
        {
            imageOut[j][i] = *(imageIn+(j*MT9V03X_H/COLH)*MT9V03X_W+i*MT9V03X_W/COLW);//读取像素点
        }
    }
}
/*---------------------------------------------------------------
 【函    数】Get_Use_Image
 【功    能】得到原始图像
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Get_Use_Image(void)    //得到原始图像
{
    short i = 0, j = 0, row = 0, line = 0;

    for (i = 0; i < MT9V03X_H; i += 1)
    {
        for (j = 0; j < MT9V03X_W; j += 1)
        {
            Image_Use[row][line] = mt9v03x_image[i][j];
            line++;
        }
        line = 0;
        row++;
    }
}

/*---------------------------------------------------------------
 【函    数】Get_OSTU
 【功    能】用大津法得到图像最佳阈值
 【参    数】Image[COLH][COLW]
 【返 回 值】Threshold
 【注意事项】
 ----------------------------------------------------------------*/
short Get_OSTU(unsigned char Image[COLH][COLW])
{
   signed short i,j;
   unsigned long Foreground_pixels = 0; //前景像素点
   unsigned long Background_pixels = 0; //背景像素点
   unsigned long Overall_pixels = 0; //总体像素点
   unsigned long Foreground_grayscale = 0;  //前景灰度值
   unsigned long Background_grayscale = 0;  //背景灰度值
   unsigned long Overall_grayscale = 0;  //总体灰度值
   unsigned char Grayscale[256];    //灰度直方图
   signed short Min_grayscale = 0, Max_grayscale = 256;  //最小灰度值(初始为0),最大灰度值(初始为256)
   float Fore_pixel_scale, Back_pixel_scale, Fore_avreage_grayscale, Back_avreage_grayscale, Variance_A, Variance_B = -1;
   // 前景像素比例, 背景像素比例, 前景平均灰度, 背景平均灰度值, 类间方差1, 类间方差2(初始化为-1)
   signed short Threshold = 0;  //阈值

   for (i=0;i<256;i++)  Grayscale[i] = 0;   //初始化灰度直方图
   for (j=0;j<COLH;j++){
       for (i=0;i<COLW;i++){
           Grayscale[Image[j][i]]++;    //统计图像像素点的灰度值
       }
   }
   for (Min_grayscale; Min_grayscale <256 && Grayscale[Min_grayscale] == 0; Min_grayscale++);  //得到最小灰度值
   for (Max_grayscale; Max_grayscale > Min_grayscale && Grayscale[Min_grayscale] == 0; Max_grayscale--);    //得到最大灰度值

   for (j=Min_grayscale;j<Max_grayscale;j++){
       Overall_pixels += Grayscale[j];  //统计总体像素点
       Overall_grayscale += Grayscale[j] * j;   //统计总体灰度值
   }

   for (j=Min_grayscale;j<Max_grayscale;j++){
       Foreground_pixels += Grayscale[j];   //前景像素点
       Background_pixels = Overall_pixels - Foreground_pixels;  //背景像素点
       Fore_pixel_scale = (float)Foreground_pixels / Overall_pixels;   //前景像素比例
       Back_pixel_scale = (float)Background_pixels / Overall_pixels;   //背景像素比例
       Foreground_grayscale += Grayscale[j] * j;    //前景灰度值
       Background_grayscale = Overall_grayscale - Foreground_grayscale; //背景灰度值
       Fore_avreage_grayscale = (float)Foreground_grayscale / (j + 1); //前景平均灰度值
       Back_avreage_grayscale = (float)Background_grayscale / (Max_grayscale - j -1);  //背景平均灰度值
       Variance_A = Fore_pixel_scale * Back_pixel_scale * (Fore_avreage_grayscale - Back_avreage_grayscale) * (Fore_avreage_grayscale - Back_avreage_grayscale);    //类间方差
       if (Variance_A > Variance_B){    //遍历整个图像找出最大类间方差
           Variance_B = Variance_A;
           Threshold = j;
       }
   }
   return (signed short int)Threshold;  //返回最佳阈值
}
/*
 同上都是大津法
 */
uint8 adapt_threshold(uint8 *image, uint16 startW, uint16 endW)   //注意计算阈值的一定要是原图像
{
    #define GrayScale 256
    uint16 width = endW-startW;
    uint16 height = COLH;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)
    {
        for (j = startW; j < endW; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
        }
    }

    //计算每个像素值的点在整幅图像中的比例

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;


    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

       w1=1-w0;
       u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = j;
        }
        if (deltaTmp < deltaMax)
        {
        break;
        }
     }

    return threshold;
}

/*---------------------------------------------------------------
 【函    数】Bin_Image_Filter
 【功    能】过滤噪点
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Bin_Image_Filter(void)
{
    sint16 nr; //行
    sint16 nc; //列

    for (nr = 1; nr < COLH - 1; nr++)
    {
        for (nc = 1; nc < COLW - 1; nc++)
        {
            if ((Bin_Image[nr][nc] == 0)
                    && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] > 2))
            {
                Bin_Image[nr][nc] = 1;
            }
            else if ((Bin_Image[nr][nc] == 1)
                    && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] < 2))
            {
                Bin_Image[nr][nc] = 0;
            }
        }
    }
}
/*---------------------------------------------------------------
 【函    数】robert
 【功    能】边缘检测
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void robert(unsigned char imageIn[COLH][COLW], unsigned char imageOut[COLH][COLW], unsigned char Threshold)
{
    /** 卷积核大小 */
    short xStart = 1;
    short xEnd = COLW-2;
    short yStart = 1;
    short yEnd = COLH-1;
    short i, j;
    short temp[2];
    for (i = yStart; i < yEnd; i++)
    {
        unsigned char *imagedata0 = imageIn[i];
        unsigned char *imagedata1 = imageIn[i + 1];
        for (j = xStart; j < xEnd+1; j++)
        {
            /* 计算不同方向梯度幅值  */
//            temp[0] = imagedata1[j+1] - imagedata0[j];//imagedata0[j] - imagedata1[j - 1];
//            temp[1] = imagedata0[j+1] - imagedata1[j];
            temp[0] = imagedata0[j] - imagedata1[j - 1];
            temp[1] = imagedata0[j] - imagedata1[j + 1];

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);

            /* 找出梯度幅值最大值  */
            if (temp[0] < temp[1])
            {
                temp[0] = temp[1];
            }

            if (temp[0] > Threshold)
            {
                imageOut[i][j] = 1;
            }
            else
            {
                imageOut[i][j] = 0;
            }
        }
    }
}
/*---------------------------------------------------------------
 【函    数】Get_Bin_Image
 【功    能】获取图像的二值化图像
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Get_Bin_Image(unsigned char mode)
{
    unsigned short i = 0;
    unsigned short ThresholdRobert=12;
    unsigned short ThresholdOstu;
    unsigned short ThresholdSobel=70;
    if (mode == 0)
    {
        sobel(Image_Use, Bin_Image, (unsigned char) ThresholdSobel);
    }
    if (mode == 1)
    {
        robert(Image_Use, Bin_Image, (unsigned char) ThresholdRobert);
    }
    ThresholdOstu = 40;//
    /* 图片最后一行二值化 */
    for (i = 0; i < COLW; i++)
    {
        if (Image_Use[COLH-1][i] > ThresholdOstu) //数值越大，显示的内容越多，较浅的图像也能显示出来
            Bin_Image[COLH-1][i] = 1;//1
        else
            Bin_Image[COLH-1][i] = 0;//0
    }
}

#define PI      3.141593
#define PI_8    0.3927


//IFX_ALIGN(4) IMG_Typedef img_Handle;

//inline static void _soble(IMG_Typedef *image)
//{
//    /* 卷积算子soble */
//    for(uint32 i=1; i<image->heigh-1; i++)
//    {
//        for(uint32 j=1;j<image->width-1;j++)
//        {
//            uint8* ptr[9];
//            /** 1  2  3
//              * 4  0  5
//              * 6  7  8 */
//            /** 1  2  1
//              * 2  0  2
//              * 1  2  1 */
//            ptr[0]=&image->imgfliter[i*image->width+j];
//            ptr[1]=&image->imgfliter[(i-1)*image->width+j-1];
//            ptr[2]=&image->imgfliter[(i-1)*image->width+j];
//            ptr[3]=&image->imgfliter[(i-1)*image->width+j+1];
//            ptr[4]=&image->imgfliter[i*image->width+j-1];
//            ptr[5]=&image->imgfliter[i*image->width+j+1];
//            ptr[6]=&image->imgfliter[(i+1)*image->width+j-1];
//            ptr[7]=&image->imgfliter[(i+1)*image->width+j];
//            ptr[8]=&image->imgfliter[(i+1)*image->width+j+1];
//            image->Xgrad[i*image->width+j] = (float)(*ptr[3]+*ptr[5]*2+*ptr[8]-*ptr[1]-*ptr[4]*2-*ptr[6]);
//            image->Ygrad[i*image->width+j] = (float)(*ptr[1]+*ptr[2]*2+*ptr[3]-*ptr[6]-*ptr[7]*2-*ptr[8]);
//            if(image->Xgrad[i*image->width+j] == 0)image->Xgrad[i*image->width+j] = 0.001;
//            image->Seita[i*image->width+j]  =  atan2(image->Ygrad[i*image->width+j],image->Xgrad[i*image->width+j]);
//            image->grad[i*image->width+j]   =  sqrt(image->Xgrad[i*image->width+j]*image->Xgrad[i*image->width+j]+image->Ygrad[i*image->width+j]*image->Ygrad[i*image->width+j]);
//        }
//    }
//}

inline static void _gauss(IMG_Typedef *image)
{
    float ptr[9];
    /* 高斯滤波 */
    for(uint32 i=1; i<image->heigh-1; i++)
    {
        for(uint32 j=1;j<image->width-1;j++)
        {
            /** 1  2  3
              * 4  0  5
              * 6  7  8 */
            /** 0.0751  0.1238  0.0751
              * 0.1238  0.2042  0.1238
              * 0.0751  0.1238  0.0751 */
            ptr[0]=image->imgsrc[i*image->width+j]*0.2042;
            ptr[1]=image->imgsrc[(i-1)*image->width+j-1]*0.0751;
            ptr[2]=image->imgsrc[(i-1)*image->width+j]*0.1238;
            ptr[3]=image->imgsrc[(i-1)*image->width+j+1]*0.0751;
            ptr[4]=image->imgsrc[i*image->width+j-1]*0.1238;
            ptr[5]=image->imgsrc[i*image->width+j+1]*0.1238;
            ptr[6]=image->imgsrc[(i+1)*image->width+j-1]*0.0751;
            ptr[7]=image->imgsrc[(i+1)*image->width+j]*0.1238;
            ptr[8]=image->imgsrc[(i+1)*image->width+j+1]*0.0751;
            image->imgfliter[i*image->width+j] = (uint8)(ptr[0]+ptr[1]+ptr[2]+ptr[3]+ptr[4]+ptr[5]+ptr[6]+ptr[7]+ptr[8]);
        }
    }
    /* 边缘处理 */
    for(uint32 j=1; j<image->width-1; j++)
    {
        ptr[0]=image->imgsrc[0*image->width+j]*0.3787;
        ptr[5]=image->imgsrc[0*image->width+j+1]*0.2347;
        ptr[7]=image->imgsrc[(0+1)*image->width+j]*0.2347;
        ptr[8]=image->imgsrc[(0+1)*image->width+j+1]*0.1424;
        image->imgfliter[0*image->width+j] = (uint8)(ptr[0]+ptr[5]+ptr[7]+ptr[8]);
        ptr[0]=image->imgsrc[(image->heigh-1)*image->width+j]*0.3787;
        ptr[2]=image->imgsrc[(image->heigh-2)*image->width+j]*0.2347;
        ptr[5]=image->imgsrc[(image->heigh-1)*image->width+j+1]*0.2347;
        ptr[3]=image->imgsrc[(image->heigh-2)*image->width+j+1]*0.1424;
        image->imgfliter[(image->heigh-1)*image->width+j] = (uint8)(ptr[0]+ptr[2]+ptr[5]+ptr[3]);
    }
    for(uint32 i=1; i<image->heigh-1; i++)
    {
        ptr[0]=image->imgsrc[i*image->width]*0.3787;
        ptr[5]=image->imgsrc[i*image->width+1]*0.2347;
        ptr[7]=image->imgsrc[(i+1)*image->width]*0.2347;
        ptr[8]=image->imgsrc[(i+1)*image->width+1]*0.1424;
        image->imgfliter[i*image->width] = (uint8)(ptr[0]+ptr[5]+ptr[7]+ptr[8]);
        ptr[0]=image->imgsrc[(i+1)*image->width-1]*0.3787;
        ptr[4]=image->imgsrc[(i+1)*image->width-2]*0.2347;
        ptr[7]=image->imgsrc[(i+2)*image->width-1]*0.2347;
        ptr[6]=image->imgsrc[(i+2)*image->width-2]*0.1424;
        image->imgfliter[(i+1)*image->width-1] = (uint8)(ptr[0]+ptr[4]+ptr[7]+ptr[6]);
    }
    ptr[0]=image->imgsrc[0]*0.3787;
    ptr[5]=image->imgsrc[1]*0.2347;
    ptr[7]=image->imgsrc[image->width]*0.2347;
    ptr[8]=image->imgsrc[image->width+1]*0.1424;
    image->imgfliter[0]                             = (uint8)(ptr[0]+ptr[5]+ptr[7]+ptr[8]);
    ptr[0]=image->imgsrc[image->width-1]*0.3787;
    ptr[4]=image->imgsrc[image->width-2]*0.2347;
    ptr[7]=image->imgsrc[2*image->width-1]*0.2347;
    ptr[6]=image->imgsrc[2*image->width-2]*0.1424;
    image->imgfliter[image->width-1]                = (uint8)(ptr[0]+ptr[4]+ptr[7]+ptr[6]);
    ptr[0]=image->imgsrc[(image->heigh-1)*image->width]*0.3787;
    ptr[2]=image->imgsrc[(image->heigh-2)*image->width]*0.2347;
    ptr[5]=image->imgsrc[(image->heigh-1)*image->width+1]*0.2347;
    ptr[3]=image->imgsrc[(image->heigh-2)*image->width+1]*0.1424;
    image->imgfliter[(image->heigh-1)*image->width] = (uint8)(ptr[0]+ptr[2]+ptr[5]+ptr[3]);
    ptr[0]=image->imgsrc[image->heigh*image->width-1]*0.3787;
    ptr[2]=image->imgsrc[(image->heigh-1)*image->width-1]*0.2347;
    ptr[4]=image->imgsrc[image->heigh*image->width-2]*0.2347;
    ptr[1]=image->imgsrc[(image->heigh-1)*image->width-2]*0.1424;
    image->imgfliter[image->heigh*image->width-1]   = (uint8)(ptr[0]+ptr[2]+ptr[4]+ptr[1]);
}
inline static void _restrain(IMG_Typedef *image)
{
    /*非极大值抑制*/
    for(uint32 i=1; i<image->heigh-1; i++)
    {
        for(uint32 j=1;j<image->width-1;j++)
        {
            float  dir   =image->Seita[i*image->width+j];
            float  g     =image->grad[i*image->width+j];
            float  g0, g1;

            /* 0 */
            if ((dir >= -PI_8)&&(dir < PI_8))
            {
                 g0=image->grad[i*image->width+j-1];
                 g1=image->grad[i*image->width+j+1];
            }
            /* 45 */
            else if ((dir >= PI_8)&&(dir < 3*PI_8))
            {
                 g0=image->grad[(i-1)*image->width+j+1];
                 g1=image->grad[(i+1)*image->width+j-1];
            }
            /* 90 */
            else if ((dir >= 3*PI_8)||(dir < -3*PI_8))
            {
                 g0=image->grad[(i-1)*image->width+j];
                 g1=image->grad[(i+1)*image->width+j];
            }
            /* 135 */
            else//    ((dir >= -3*PI_8)&&(dir < -PI_8))
            {
                 g0=image->grad[(i-1)*image->width+j-1];
                 g1=image->grad[(i-1)*image->width+j+1];
            }
            if (g<=g0 || g<=g1)image->grad[i*image->width+j] = 0;
        }
    }
}

#define Strong_threshold    180
#define Weak_threshold      50
inline static void _threshold(IMG_Typedef *image)
{
    /* 双阈值检测 */
    for(uint32 i=1; i<image->heigh-1; i++)
    {
        for(uint32 j=1;j<image->width-1;j++)
        {
            if(image->grad[i*image->width+j] > Strong_threshold)
            {
                image->grad[i*image->width+j] = 255;
            }
            else if(image->grad[i*image->width+j] > Weak_threshold)
            {
                image->grad[i*image->width+j] = 80;
            }
            else
            {
                image->grad[i*image->width+j] = 0;
            }
        }
    }
}
inline static void _connect(IMG_Typedef *image)
{
    /* 双阈值边缘链接 */
    for(uint32 i=1; i<image->heigh-1; i++)
    {
        for(uint32 j=1;j<image->width-1;j++)
        {
            if(image->grad[i*image->width+j] == 255)
            {
                for (int8 n = -1; n < 1; n++)
                {
                    for (int8 m = -1; m < 1; m++)
                    {
                        if(image->grad[(i+n)*image->width+j+m] != 0)
                           image->grad[(i+n)*image->width+j+m] = 255;
                    }
                }
            }
        }
    }
    /* 过滤孤立边缘 */
    for(uint32 i=1; i<image->heigh-1; i++)
    {
        for(uint32 j=1;j<image->width-1;j++)
        {
            if(image->grad[i*image->width+j] != 255)
            {
                image->grad[i*image->width+j] = 0;
            }
        }
    }
}
//void image_canny(IMG_Typedef *image)
//{
//    _gauss(image);
//    _soble(image);
//    _restrain(image);
//    _threshold(image);
//    _connect(image);
//
//    /* 导出 */
//    for(uint32 i=0; i<image->heigh; i++)
//    {
//        for(uint32 j=0;j<image->width;j++)
//        {
//            image->imgout[i*image->width+j] = (uint8)image->grad[i*image->width+j];
//        }
//    }
//}

void sobel (unsigned char imageIn[COLH][COLW], unsigned char imageOut[COLH][COLW], unsigned char Threshold)
{
    /** 卷积核大小 */
    //short KERNEL_SIZE = 3;
    short xStart = 1;
    short xEnd = COLW-2;
    short yStart = 1;
    short yEnd = COLH-1;
    short i, j, k;
    short temp[2];
    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd+1; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]     //{{-1, 0, 1},
            - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]        // {-1, 0, 1},
            - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];    // {-1, 0, 1}};

            temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]     //{{-1, -1, -1},
            - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]       // { 0,  0,  0},
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];    // { 1,  1,  1}};


            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);

            temp[0] = (temp[0] < temp[1])?temp[1]:temp[0];

            imageOut[i][j] = (temp[0] > Threshold)?1:0;


//            /* 找出梯度幅值最大值  */
//            for (k = 1; k < 2; k++)
//            {
//                if (temp[0] < temp[k])
//                {
//                    temp[0] = temp[k];
//                }
//            }
//
//            if (temp[0] > Threshold)
//            {
//                imageOut[i][j] = 1;
//            }
//            else
//            {
//                imageOut[i][j] = 0;
//            }
        }
    }
}
//void Get_Width_Road()
//{
//    //打印赛道宽度
//    for(uint8 y = COLH-1; y > 0; y--)
//    {
//        if(y == COLH-1)
//        {
//            printf("%d",  1022);
//            printf(",");
//        }
//        if(y%10 == 0)
//        {
//            printf("%d", ImageSide[y][1] - ImageSide[y][0]);
//            printf(",");
//            printf(", \n");
//        }
//        else
//        {
//            printf("%d", ImageSide[y][1] - ImageSide[y][0]);
//            printf(",");
//        }
//    }
//}
void Get_Width_Road()
{
    //打印赛道宽度
        for(uint8 y = COLH-1; y > 0; y--)
        {
            Tim_Track_Width[y] = ImageSide[y][1] - ImageSide[y][0];
            printf("%d\n",Tim_Track_Width[y]);
        }
}
//-----------------------------------------------------------------------------------------
//  @brief      画边线和中线(彩色)
//  @param      void
//  @return     void
//  Sample usage:    Blacking();
//  explain:  如果屏幕显示的图像进行了缩放就不能适用
//-----------------------------------------------------------------------------------------
void Blacking()
{
    for(uint8 y=(COLH-1); y > 0; y--)
    {
        if(ImageSide[y][0] < 2)
        {
            ImageSide[y][0] = 0;
        }
        if(ImageSide[y][1] > COLW-2)
        {
            ImageSide[y][1] = COLW - 1;
        }

        ips114_draw_point(ImageSide[y][0]+2, y, RGB565_GREEN);
        ips114_draw_point(ImageSide[y][0]+3, y, RGB565_GREEN);

        ips114_draw_point(ImageSide[y][1]-2, y, RGB565_PURPLE);
        ips114_draw_point(ImageSide[y][1]-3, y, RGB565_PURPLE);

        ips114_draw_point( (ImageSide[y][0] + ImageSide[y][1])/2, y, RGB565_BLACK);
    }
}

