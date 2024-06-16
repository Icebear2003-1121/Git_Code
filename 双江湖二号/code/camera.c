#include "camera.h"

uint8 Image_Use[LCDH][LCDW]; // 存储使用中的图像数据的数组，大小为LCDH行LCDW列
uint8 Bin_Image[LCDH][LCDW]; // 存储二值化后的图像数据的数组，大小为LCDH行LCDW列
//uint16 ThresholdSobel; // Sobel算子阈值的无符号整型变量
//uint16 ThresholdOstu; // Otsu算法阈值的无符号整型变量
uint8 LeftSide[LCDH]; // 存储左侧边缘的数组，长度为LCDH
uint8 RightSide[LCDH]; // 存储右侧边缘的数组，长度为LCDH
uint16 ExposureTime; // 曝光时间的无符号整型变量
uint16 ImageGain; // 图像增益的无符号整型变量
uint8 Tim_Track_Width[COLH];//实时图像宽度
//int a,b,c,d;
void Test_CAMERA(void)
{
    if (mt9v03x_finish_flag) // 如果mt9v03x_finish_flag为真
    {
        Get_Use_Image(*mt9v03x_image,Image_Use); // 调用Get_Use_Image函数，将图像数据压缩并存储在Image_Use数组中

        Get_Bin_Image(0); // 调用Get_Bin_Image函数，生成二值化图像数据并存储在Bin_Image数组中

        Bin_Image_Filter(); // 调用Bin_Image_Filter函数，对二值化图像进行去噪处理

        ImageGetSide(Bin_Image,ImageSide,RoadLostFlag); // 调用ImageGetSide函数，从二值化图像中提取边界信息，存储在ImageSide数组中，并根据RoadLostFlag标志标识是否道路丢失

        dl1a_get_distance(); // 调用dl1a_get_distance函数，获取距离信息

        ElementsJudge(); // 调用ElementsJudge函数，对元素进行判断和处理
        if (RoundFlag==5 || RoundFlag==6) // 如果RoundFlag的值等于5或6
        {
            ImageGetSideRound(Bin_Image,ImageSideRound); // 调用ImageGetSideRound函数，从二值化图像中提取圆环的边界信息，存储在ImageSideRound数组中
        }
        ImageGetMidLine(ImageSide, MidLine); // 调用ImageGetMidLine函数，从边界信息中计算得到中线数据，并存储在MidLine数组中
        if(!ZebraFlag)
        {
          bias_curvature2();//计算偏差
          steer_allcontrol();//舵机控制
        }
        outgoing();
        SpeedDecision();
//        Get_Width_Road();
//        ips114_show_gray_image(0, 0, (uint8 *)Bin_Image, LCDW, LCDH, LCDW, LCDH, 0); // 在OLED显示屏上显示灰度图像，参数为图像数据的指针以及显示相关的参数
        /*XK图传函数*/
//        XK_uart();
//        a=RoundaboutLine(ImageSide,30,15,1,43/*43*/,3);
//        b=RoundaboutLine(ImageSide,LCDH-1,50,1,25,1);
//        c=ForkaboutGetArc(ImageSide,50,22,1,3);
//        d=TrackWidth(ImageSideRound,LCDH-4,30,10,90);
//        e=!SideIsNoLost(RoadLostFlag,40,22,1,4);
//        f=RoadSideMono(ImageSide,50,15,1);
//        g=SideColumnSpace(ImageSide,LCDH-2,40,2,1,3);
//        ips114_show_uint(0, 0, a, 3);
//        ips114_show_uint(120, 0, gptR_Cir, 3);
//        ips114_show_uint(0, 40, c, 3);
//        ips114_show_uint(0, 60, d, 3);
//        ips114_show_uint(0, 80, e, 3);
//        ips114_show_uint(0, 100, f, 3);
//        ips114_show_uint(20, 0, g, 3);

//          printf("%d,%d,%d,%d\n",a,b,c,d);

        mt9v03x_finish_flag = 0; // 将mt9v03x_finish_flag重设为0，表示当前处理完成
    }
}

void Get_Use_Image(uint8 *imageIn, uint8 imageOut[LCDH][LCDW])
{

    uint32 i, j;

    // 使用嵌套的循环遍历图像的每个像素点
    for (j = 0; j < LCDH; j++)
    {
        for (i = 0; i < LCDW; i++)
        {
            // 根据传入的图像数据指针和图像尺寸进行计算，获取目标像素点的值，并赋值给输出数组对应位置的像素值
            imageOut[j][i] = *(imageIn + (j * MT9V03X_H / LCDH) * MT9V03X_W + i * MT9V03X_W / LCDW);
        }
    }
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
    sint16 nr; // 行
    sint16 nc; // 列

    // 遍历二值化图像的每个像素点（除去边缘）
    for (nr = 1; nr < LCDH - 1; nr++)
    {
        for (nc = 1; nc < LCDW - 1; nc++)
        {
            // 对当前像素点进行条件判断，并根据判断结果进行处理
            if ((Bin_Image[nr][nc] == 0) && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] > 2))
            {
                // 如果当前像素为黑色且周围至少有3个白色像素，则将当前像素设为白色
                Bin_Image[nr][nc] = 1;
            }
            else if ((Bin_Image[nr][nc] == 1) && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] < 2))
            {
                // 如果当前像素为白色且周围至少有3个黑色像素，则将当前像素设为黑色
                Bin_Image[nr][nc] = 0;
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

//void Get_Bin_Image(void)
//{
//    unsigned short ThresholdSobel=60;
//    unsigned short ThresholdOstu = 35;
//    uint8 i = 0, j = 0;
//
//    sobel(Image_Use, Bin_Image, (unsigned char) ThresholdSobel);
//
//    // 将最后一行图像进行二值化处理
//    for (i = 0; i < LCDW; i++)
//    {
//        if (Image_Use[LCDH - 1][i] > ThresholdOstu)
//        {
//            // 如果最后一行图像的像素值大于全局阈值，则将相应位置的二值化图像像素设为1
//            Bin_Image[LCDH - 1][i] = 1;
//        }
//        else
//        {
//            // 如果最后一行图像的像素值小于等于全局阈值，则将相应位置的二值化图像像素设为0
//            Bin_Image[LCDH - 1][i] = 0;
//        }
//    }
//}

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

void sobel (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
{
    short xStart = 1;
    short xEnd = LCDW-2;
    short yStart = 1;
    short yEnd = LCDH-1;
    short i, j;
    short temp[2];
    for (i = yStart; i < yEnd; i++)
    {
        for (j = xStart; j < xEnd+1; j++)
        {

            temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]     //{-1, 0, 1}
            - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]                      //{-1, 0, 1}
            - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];             //{-1, 0, 1}

            temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]     //{-1, -1, -1}
            - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]                      //{ 0,  0,  0}
            - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];             //{ 1,  1,  1}

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);

            temp[0]=(temp[0] < temp[1])?temp[1]:temp[0];

            imageOut[i][j]=(temp[0] > Threshold)?1:0;
        }
    }
}
void ImageGetMidLine(int imageInput[LCDH][2],int imageOut[LCDH])
{
    int y;
    for(y=LCDH-2;y>0;y--)
    {
        imageOut[y]=(imageInput[y][0]+imageInput[y][1])/2;
        if(imageOut[y]<=LCDW-3&&imageOut[y]>=1)
        {
            Bin_Image[y][imageOut[y]]=1;
        }
    }
}

void camera_Init(void)
{
//    ThresholdSobel = 60;//70 越大越暗
//    ThresholdOstu = 35;
    ExposureTime = 300;
    ImageGain = 32;
}

void Get_Width_Road()
{
    //打印赛道宽度
        for(uint8 y = COLH-1; y > 0; y--)
        {
            Tim_Track_Width[y] = ImageSide[y][1] - ImageSide[y][0];
            printf("%d\n",Tim_Track_Width[y]);
        }
}


