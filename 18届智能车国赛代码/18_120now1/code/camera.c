/*
 * camera.c
 *
 *  Created on: 2023��3��20��
 *      Author: ����a
 */
#include "camera.h"

uint8 Image_Use[COLH][COLW];// ԭʼͼ�� or ѹ��ͼ��
uint8 Bin_Image[COLH][COLW];// ��ֵ��ͼ��
uint8 Tim_Track_Width[COLH];//ʵʱͼ����
uint8 LeftSide[COLH];
uint8 RightSide[COLH];
int CollectTim=0;//�ռ�ʱ�����
int SpeedMode;
int TimeFlag = 0;

void Test_CAMERA(void)
{

    if(mt9v03x_finish_flag)
    {

        if(TimeFlag)
        {
            system_start();// ������ʱ������¼�µ�ǰ��ʱ��
        }
        Get_Use_Image1(*mt9v03x_image,Image_Use);//ͼ��ѹ��
        Get_Bin_Image(0);//�õ���ֵ��
        Bin_Image_Filter();//ȥ��
        ImageGetSide(Bin_Image,ImageSide,RoadLostFlag);//�õ��߽�
        GyroResolve();
//       Outside_protect(95);
        ElementsJudge();//Ԫ���ж�
//        ImageSideTransform(ImageSide,LeftSide,RightSide);//��ӡ�߽���������λ������ʾc
        ImageGetMidLine(ImageSide,MidLine);//�õ�����
        bias_curvature();//�õ�ƫ��
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
//         sendline(3,LeftSide,COLH);//���������
//         sendline(3,RightSide,COLH);//�����ұ���
//         sendline(2,MidLine,COLH);//��������
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
            CollectTim=system_getval_ms();//��ȡ��ǰ��ʱʱ��  ��λus
            oled_show_int(50,2,CollectTim,3);
        }
#endif
        mt9v03x_finish_flag = 0;
   }
}
/*---------------------------------------------------------------
 ����    ����Get_Use_Image1
 ����    �ܡ�ͼ��ѹ��
 ����    ����ԭʼ�ɼ�ͼ�񣬺ʹ洢ͼ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void Get_Use_Image1(uint8 *imageIn,uint8 imageOut[COLH][COLW])      //ͼ��ѹ��
{
    uint32 i,j;
    for(j=0;j<COLH;j++)
    {
        for(i=0;i<COLW;i++)
        {
            imageOut[j][i] = *(imageIn+(j*MT9V03X_H/COLH)*MT9V03X_W+i*MT9V03X_W/COLW);//��ȡ���ص�
        }
    }
}
/*---------------------------------------------------------------
 ����    ����Get_Use_Image
 ����    �ܡ��õ�ԭʼͼ��
 ����    ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void Get_Use_Image(void)    //�õ�ԭʼͼ��
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
 ����    ����Get_OSTU
 ����    �ܡ��ô�򷨵õ�ͼ�������ֵ
 ����    ����Image[COLH][COLW]
 ���� �� ֵ��Threshold
 ��ע�����
 ----------------------------------------------------------------*/
short Get_OSTU(unsigned char Image[COLH][COLW])
{
   signed short i,j;
   unsigned long Foreground_pixels = 0; //ǰ�����ص�
   unsigned long Background_pixels = 0; //�������ص�
   unsigned long Overall_pixels = 0; //�������ص�
   unsigned long Foreground_grayscale = 0;  //ǰ���Ҷ�ֵ
   unsigned long Background_grayscale = 0;  //�����Ҷ�ֵ
   unsigned long Overall_grayscale = 0;  //����Ҷ�ֵ
   unsigned char Grayscale[256];    //�Ҷ�ֱ��ͼ
   signed short Min_grayscale = 0, Max_grayscale = 256;  //��С�Ҷ�ֵ(��ʼΪ0),���Ҷ�ֵ(��ʼΪ256)
   float Fore_pixel_scale, Back_pixel_scale, Fore_avreage_grayscale, Back_avreage_grayscale, Variance_A, Variance_B = -1;
   // ǰ�����ر���, �������ر���, ǰ��ƽ���Ҷ�, ����ƽ���Ҷ�ֵ, ��䷽��1, ��䷽��2(��ʼ��Ϊ-1)
   signed short Threshold = 0;  //��ֵ

   for (i=0;i<256;i++)  Grayscale[i] = 0;   //��ʼ���Ҷ�ֱ��ͼ
   for (j=0;j<COLH;j++){
       for (i=0;i<COLW;i++){
           Grayscale[Image[j][i]]++;    //ͳ��ͼ�����ص�ĻҶ�ֵ
       }
   }
   for (Min_grayscale; Min_grayscale <256 && Grayscale[Min_grayscale] == 0; Min_grayscale++);  //�õ���С�Ҷ�ֵ
   for (Max_grayscale; Max_grayscale > Min_grayscale && Grayscale[Min_grayscale] == 0; Max_grayscale--);    //�õ����Ҷ�ֵ

   for (j=Min_grayscale;j<Max_grayscale;j++){
       Overall_pixels += Grayscale[j];  //ͳ���������ص�
       Overall_grayscale += Grayscale[j] * j;   //ͳ������Ҷ�ֵ
   }

   for (j=Min_grayscale;j<Max_grayscale;j++){
       Foreground_pixels += Grayscale[j];   //ǰ�����ص�
       Background_pixels = Overall_pixels - Foreground_pixels;  //�������ص�
       Fore_pixel_scale = (float)Foreground_pixels / Overall_pixels;   //ǰ�����ر���
       Back_pixel_scale = (float)Background_pixels / Overall_pixels;   //�������ر���
       Foreground_grayscale += Grayscale[j] * j;    //ǰ���Ҷ�ֵ
       Background_grayscale = Overall_grayscale - Foreground_grayscale; //�����Ҷ�ֵ
       Fore_avreage_grayscale = (float)Foreground_grayscale / (j + 1); //ǰ��ƽ���Ҷ�ֵ
       Back_avreage_grayscale = (float)Background_grayscale / (Max_grayscale - j -1);  //����ƽ���Ҷ�ֵ
       Variance_A = Fore_pixel_scale * Back_pixel_scale * (Fore_avreage_grayscale - Back_avreage_grayscale) * (Fore_avreage_grayscale - Back_avreage_grayscale);    //��䷽��
       if (Variance_A > Variance_B){    //��������ͼ���ҳ������䷽��
           Variance_B = Variance_A;
           Threshold = j;
       }
   }
   return (signed short int)Threshold;  //���������ֵ
}
/*
 ͬ�϶��Ǵ��
 */
uint8 adapt_threshold(uint8 *image, uint16 startW, uint16 endW)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
    #define GrayScale 256
    uint16 width = endW-startW;
    uint16 height = COLH;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height/4;
    uint8 threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i+=2)
    {
        for (j = startW; j < endW; j+=2)
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
        }
    }

    //����ÿ������ֵ�ĵ�������ͼ���еı���

    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;

    }

    //�����Ҷȼ�[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;


    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = 0; j < GrayScale; j++)
    {
        w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

       w1=1-w0;
       u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
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
 ����    ����Bin_Image_Filter
 ����    �ܡ��������
 ����    ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void Bin_Image_Filter(void)
{
    sint16 nr; //��
    sint16 nc; //��

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
 ����    ����robert
 ����    �ܡ���Ե���
 ����    ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void robert(unsigned char imageIn[COLH][COLW], unsigned char imageOut[COLH][COLW], unsigned char Threshold)
{
    /** ����˴�С */
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
            /* ���㲻ͬ�����ݶȷ�ֵ  */
//            temp[0] = imagedata1[j+1] - imagedata0[j];//imagedata0[j] - imagedata1[j - 1];
//            temp[1] = imagedata0[j+1] - imagedata1[j];
            temp[0] = imagedata0[j] - imagedata1[j - 1];
            temp[1] = imagedata0[j] - imagedata1[j + 1];

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);

            /* �ҳ��ݶȷ�ֵ���ֵ  */
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
 ����    ����Get_Bin_Image
 ����    �ܡ���ȡͼ��Ķ�ֵ��ͼ��
 ����    ������
 ���� �� ֵ����
 ��ע�����
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
    /* ͼƬ���һ�ж�ֵ�� */
    for (i = 0; i < COLW; i++)
    {
        if (Image_Use[COLH-1][i] > ThresholdOstu) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����
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
//    /* �������soble */
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
    /* ��˹�˲� */
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
    /* ��Ե���� */
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
    /*�Ǽ���ֵ����*/
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
    /* ˫��ֵ��� */
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
    /* ˫��ֵ��Ե���� */
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
    /* ���˹�����Ե */
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
//    /* ���� */
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
    /** ����˴�С */
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
            /* ���㲻ͬ�����ݶȷ�ֵ  */
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


//            /* �ҳ��ݶȷ�ֵ���ֵ  */
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
//    //��ӡ�������
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
    //��ӡ�������
        for(uint8 y = COLH-1; y > 0; y--)
        {
            Tim_Track_Width[y] = ImageSide[y][1] - ImageSide[y][0];
            printf("%d\n",Tim_Track_Width[y]);
        }
}
//-----------------------------------------------------------------------------------------
//  @brief      �����ߺ�����(��ɫ)
//  @param      void
//  @return     void
//  Sample usage:    Blacking();
//  explain:  �����Ļ��ʾ��ͼ����������žͲ�������
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

