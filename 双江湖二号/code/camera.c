#include "camera.h"

uint8 Image_Use[LCDH][LCDW]; // �洢ʹ���е�ͼ�����ݵ����飬��СΪLCDH��LCDW��
uint8 Bin_Image[LCDH][LCDW]; // �洢��ֵ�����ͼ�����ݵ����飬��СΪLCDH��LCDW��
//uint16 ThresholdSobel; // Sobel������ֵ���޷������ͱ���
//uint16 ThresholdOstu; // Otsu�㷨��ֵ���޷������ͱ���
uint8 LeftSide[LCDH]; // �洢����Ե�����飬����ΪLCDH
uint8 RightSide[LCDH]; // �洢�Ҳ��Ե�����飬����ΪLCDH
uint16 ExposureTime; // �ع�ʱ����޷������ͱ���
uint16 ImageGain; // ͼ��������޷������ͱ���
uint8 Tim_Track_Width[COLH];//ʵʱͼ����
//int a,b,c,d;
void Test_CAMERA(void)
{
    if (mt9v03x_finish_flag) // ���mt9v03x_finish_flagΪ��
    {
        Get_Use_Image(*mt9v03x_image,Image_Use); // ����Get_Use_Image��������ͼ������ѹ�����洢��Image_Use������

        Get_Bin_Image(0); // ����Get_Bin_Image���������ɶ�ֵ��ͼ�����ݲ��洢��Bin_Image������

        Bin_Image_Filter(); // ����Bin_Image_Filter�������Զ�ֵ��ͼ�����ȥ�봦��

        ImageGetSide(Bin_Image,ImageSide,RoadLostFlag); // ����ImageGetSide�������Ӷ�ֵ��ͼ������ȡ�߽���Ϣ���洢��ImageSide�����У�������RoadLostFlag��־��ʶ�Ƿ��·��ʧ

        dl1a_get_distance(); // ����dl1a_get_distance��������ȡ������Ϣ

        ElementsJudge(); // ����ElementsJudge��������Ԫ�ؽ����жϺʹ���
        if (RoundFlag==5 || RoundFlag==6) // ���RoundFlag��ֵ����5��6
        {
            ImageGetSideRound(Bin_Image,ImageSideRound); // ����ImageGetSideRound�������Ӷ�ֵ��ͼ������ȡԲ���ı߽���Ϣ���洢��ImageSideRound������
        }
        ImageGetMidLine(ImageSide, MidLine); // ����ImageGetMidLine�������ӱ߽���Ϣ�м���õ��������ݣ����洢��MidLine������
        if(!ZebraFlag)
        {
          bias_curvature2();//����ƫ��
          steer_allcontrol();//�������
        }
        outgoing();
        SpeedDecision();
//        Get_Width_Road();
//        ips114_show_gray_image(0, 0, (uint8 *)Bin_Image, LCDW, LCDH, LCDW, LCDH, 0); // ��OLED��ʾ������ʾ�Ҷ�ͼ�񣬲���Ϊͼ�����ݵ�ָ���Լ���ʾ��صĲ���
        /*XKͼ������*/
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

        mt9v03x_finish_flag = 0; // ��mt9v03x_finish_flag����Ϊ0����ʾ��ǰ�������
    }
}

void Get_Use_Image(uint8 *imageIn, uint8 imageOut[LCDH][LCDW])
{

    uint32 i, j;

    // ʹ��Ƕ�׵�ѭ������ͼ���ÿ�����ص�
    for (j = 0; j < LCDH; j++)
    {
        for (i = 0; i < LCDW; i++)
        {
            // ���ݴ����ͼ������ָ���ͼ��ߴ���м��㣬��ȡĿ�����ص��ֵ������ֵ����������Ӧλ�õ�����ֵ
            imageOut[j][i] = *(imageIn + (j * MT9V03X_H / LCDH) * MT9V03X_W + i * MT9V03X_W / LCDW);
        }
    }
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
    sint16 nr; // ��
    sint16 nc; // ��

    // ������ֵ��ͼ���ÿ�����ص㣨��ȥ��Ե��
    for (nr = 1; nr < LCDH - 1; nr++)
    {
        for (nc = 1; nc < LCDW - 1; nc++)
        {
            // �Ե�ǰ���ص���������жϣ��������жϽ�����д���
            if ((Bin_Image[nr][nc] == 0) && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] > 2))
            {
                // �����ǰ����Ϊ��ɫ����Χ������3����ɫ���أ��򽫵�ǰ������Ϊ��ɫ
                Bin_Image[nr][nc] = 1;
            }
            else if ((Bin_Image[nr][nc] == 1) && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] < 2))
            {
                // �����ǰ����Ϊ��ɫ����Χ������3����ɫ���أ��򽫵�ǰ������Ϊ��ɫ
                Bin_Image[nr][nc] = 0;
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

//void Get_Bin_Image(void)
//{
//    unsigned short ThresholdSobel=60;
//    unsigned short ThresholdOstu = 35;
//    uint8 i = 0, j = 0;
//
//    sobel(Image_Use, Bin_Image, (unsigned char) ThresholdSobel);
//
//    // �����һ��ͼ����ж�ֵ������
//    for (i = 0; i < LCDW; i++)
//    {
//        if (Image_Use[LCDH - 1][i] > ThresholdOstu)
//        {
//            // ������һ��ͼ�������ֵ����ȫ����ֵ������Ӧλ�õĶ�ֵ��ͼ��������Ϊ1
//            Bin_Image[LCDH - 1][i] = 1;
//        }
//        else
//        {
//            // ������һ��ͼ�������ֵС�ڵ���ȫ����ֵ������Ӧλ�õĶ�ֵ��ͼ��������Ϊ0
//            Bin_Image[LCDH - 1][i] = 0;
//        }
//    }
//}

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
//    ThresholdSobel = 60;//70 Խ��Խ��
//    ThresholdOstu = 35;
    ExposureTime = 300;
    ImageGain = 32;
}

void Get_Width_Road()
{
    //��ӡ�������
        for(uint8 y = COLH-1; y > 0; y--)
        {
            Tim_Track_Width[y] = ImageSide[y][1] - ImageSide[y][0];
            printf("%d\n",Tim_Track_Width[y]);
        }
}


