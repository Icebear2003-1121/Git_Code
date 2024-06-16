#include "ScanLine.h"

int ImageSide[LCDH][2], ImageSideRound[LCDH][2], RoadLostFlag[LCDH][2], UpSide[LCDW];  // ͼ���Ե�����ꡢԲ����Ե���ꡢ·�߶�ʧ��־���ϱ�Ե��Ϣ
int ImageSideEro[LCDH][2];  // ������ʴ������ı�Ե������
int rightlostpointnum = 0, leftlostpointnum = 0;  // �Ҳ����ඪʧ�������
int MidLine[LCDH];  // ������Ϣ
uint8 RoadOut_flag = 0;  // ��·�Ƿ񳬳�ͼ��Χ�ı�־
uint8 Block = 0, BlockBar = 0;  // �ϰ�����ϰ�����������

/*��·�������*/
uint8 Track_Width[60]={
                        8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
                        8,  8,  8,  8,  8,  8,  8,  8, 10, 12,
                        14, 16, 18, 20, 22, 24, 26, 28, 31, 33,
                        36, 38, 39, 41, 42, 44, 47, 49, 51, 53,
                        55, 56, 58, 60, 62, 65, 66, 68, 69, 71,
                        73, 75, 76, 77, 79, 81, 82, 83, 83, 99
                      };

/*�涨�߽�*/
uint8 Bounds[60][2]={
                        {30, 69},{29, 70},{29, 70},{28, 71},{28, 71},
                        {27, 72},{26, 73},{26, 73},{25, 74},{25, 74},
                        {24, 75},{23, 76},{23, 76},{22, 77},{22, 77},
                        {21, 78},{21, 78},{20, 79},{19, 80},{19, 80},
                        {18, 81},{18, 81},{17, 82},{16, 83},{16, 83},
                        {15, 84},{15, 84},{14, 85},{13, 86},{13, 86},
                        {12, 87},{12, 87},{11, 88},{10, 89},{10, 89},
                        { 9, 90},{ 9, 90},{ 8, 91},{ 8, 91},{ 7, 92},
                        { 6, 93},{ 6, 93},{ 5, 94},{ 5, 94},{ 4, 95},
                        { 3, 96},{ 3, 96},{ 2, 97},{ 2, 97},{ 1, 98},
                        { 0, 99},{ 0, 99},{ 0, 99},{ 0, 99},{ 0, 99},
                        { 0, 99},{ 0, 99},{ 0, 99},{ 0, 99},{ 0, 99}
                    };

void ImageGetSide(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],int imageLost[LCDH][2])
{
    int x, y, state, blockstate;  //����x��y��ʾ���꣬state��ʾ״̬��blockstate��ʾ�ϰ���״̬
    int AuxiliaryLine[LCDH], RoadWidth[LCDH];  //�����ߺ͵�·�������
    int left_white_num, right_white_num;  //�����Ҳ��ɫ������Ŀ

    AuxiliaryLine[LCDH - 1] = (LCDW - 1) / 2;  //���ø��������һ�е�ֵΪͼ���ȵ�һ��
    RoadWidth[LCDH - 1] = LCDW - 1;  //���õ�·������һ�е�ֵΪͼ���ȵ����֮��
    imageOut[LCDH - 1][1] = LCDW - 1;  //����ͼ��������һ�е��Ҷ˵�����Ϊͼ���ȼ�1
    imageOut[LCDH - 1][0] = 0;  //����ͼ��������һ�е���˵�����Ϊ0
    imageLost[LCDH - 1][1] = 1;  //����ͼ��ʧ���һ��Ϊ1
    imageLost[LCDH - 1][0] = 1;  //����ͼ��ʧ���һ��Ϊ1

    state = 0;  //��״̬����state����Ϊ0
    blockstate = 1;  //���ϰ���״̬����blockstate����Ϊ1
    left_white_num = 0;  //������ɫ������Ŀ������ʼ��Ϊ0
    right_white_num = 0;  //���Ҳ��ɫ������Ŀ������ʼ��Ϊ0
    rightlostpointnum = 0;  //���Ҳඪʧ������������ʼ��Ϊ0
    leftlostpointnum = 0;  //����ඪʧ������������ʼ��Ϊ0

    ////////////////////////////////////////���������ɨ����ʼ��///////////////////////////////////////
    ////////////////////////////////�����ͼ��������Ϊ��ɫ,����Ǻ�ɫ//////////////////////////////////
    if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//���������м�����Ϊ��ɫ
    {
        // ����ɨ��
        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)
        {
            if(imageInput[LCDH-1][x])// �����ǰ����Ϊ��ɫ
            {
                right_white_num++; // �Ҳ��ɫ����������һ
            }
        }

        // ����ɨ��
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)
        {
            if(imageInput[LCDH-1][x])// �����ǰ����Ϊ��ɫ
            {
                left_white_num++; // ����ɫ����������һ
            }
        }

        // ���ݰ�ɫ������������ֵ�����жϸ�����λ��
        if(right_white_num > left_white_num && right_white_num > 3)
        {
            AuxiliaryLine[LCDH-1] = LCDW-6; // ��������λ���ƶ���LCDW-6��
        }
        else if(left_white_num > right_white_num && left_white_num > 3)
        {
            AuxiliaryLine[LCDH-1] = 5; // ��������λ���ƶ���5��
        }
        else
        {
            // ���û���ϰ��ﲢ�Ҳ���Ҫ�����µ�·�������ó�����������־
            if(!BarrierFlag && !OpenRoadFlag)
            {
                RoadOut_flag = 1; // ������������־��1
            }
            else
            {
                // �������Ϊ��
            }
        }
    }
    else
    {
        // ����ɨ��
        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)
        {
            if(imageInput[LCDH-1][x])// �����ǰ����Ϊ��ɫ
            {
                right_white_num++; // �Ҳ��ɫ����������һ
            }
        }

        // ����ɨ��
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)
        {
            if(imageInput[LCDH-1][x])// �����ǰ����Ϊ��ɫ
            {
                left_white_num++; // ����ɫ����������һ
            }
        }

        // ���ݰ�ɫ���������жϸ�����λ�õĵ�������
        if(right_white_num > left_white_num)
        {
            AuxiliaryLine[LCDH-1] = AuxiliaryLine[LCDH-1] + right_white_num/2; // ��������λ�������ƶ�һ��İ�ɫ��������
        }
        else
        {
            AuxiliaryLine[LCDH-1] = AuxiliaryLine[LCDH-1] - left_white_num/2; // ��������λ�������ƶ�һ��İ�ɫ��������
        }
    }

    /////////////////////////////////////////////////����˫��ɨ��///////////////////////////////////////////////////////
    for(y=LCDH-2;y>15;y--)//ɨ�裬�ӵ����ڶ��п�ʼ����ɨ��
    {
        if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/2&&(imageOut[y+2][1]-imageOut[y+1][1])>6)//���Ԥ��
        {
            // �ж��Ƿ�Ϊ���
            // ����·��������ص�� x ���궼С����Ļ��ȵ�һ�룬�����������ص�֮��Ĳ�ֵ����6�����ж�Ϊ���
            // �Ե�ǰ�н�����������߽������ƶ�10����λ���ұ߽������ƶ�10����λ
            imageOut[y][0]=imageOut[y+1][0]-10;
            imageOut[y][1]=imageOut[y+1][1]-10;
            imageLost[y][0]=1;
            imageLost[y][1]=1;
        }
        else if(imageOut[y+1][0]>LCDW/2&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>6)//���Ԥ��
        {
            // �ж��Ƿ�Ϊ���
            // ����·��������ص�� x ���궼������Ļ��ȵ�һ�룬�����������ص�֮��Ĳ�ֵ����6�����ж�Ϊ���
            // �Ե�ǰ�н�����������߽������ƶ�10����λ���ұ߽������ƶ�10����λ
            imageOut[y][0]=imageOut[y+1][0]+10;
            imageOut[y][1]=imageOut[y+1][1]+10;
            imageLost[y][0]=1;
            imageLost[y][1]=1;
        }
        else//����ɨ��
        {
            // ����ɨ���·�߽�
            for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//����ɨ��
            {
                if(imageInput[y][x])
                {
                    // ��������������أ�������ұ߽����꣬�����Ϊ����
                    imageOut[y][1]=x;
                    imageLost[y][1]=1;
                    if(y>25) rightlostpointnum++;
                    break;
                }
                else
                {
                    // ����ڸ÷�Χ��û�������������أ�����Ϊ������
                    imageLost[y][1]=0;
                }
            }
            for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//����ɨ��
            {
                if(imageInput[y][x])
                {
                    // ��������������أ��������߽����꣬�����Ϊ����
                    imageOut[y][0]=x;
                    imageLost[y][0]=1;
                    if(y>25) leftlostpointnum++;
                    break;
                }
                else
                {
                    // ����ڸ÷�Χ��û�������������أ�����Ϊ������
                    imageLost[y][0]=0;
                }
            }
            if(imageLost[y][1]==0&&imageLost[y][0]==1)//ɨ������
            {
                // ���ɨ�費���ұ߽磬������һ�е������������
                if((imageLost[y+1][1]==0&&imageLost[y+1][0]==0))
                {
//                    state=1;
                    // ������һ�еĴ��ڵı߽���㵱ǰ�е��ұ߽磬�����µ�·���
                    imageOut[y][1]=imageOut[y+1][1]-(imageOut[y][0]-imageOut[y+1][0]);
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
                else
                {
                    // ������һ�еĵ�·��ȼ��㵱ǰ�е��ұ߽磬�����µ�·���
                    imageOut[y][1]=imageOut[y][0]+RoadWidth[y+1];//yΪ�����е�xֵ
                    RoadWidth[y]=RoadWidth[y+1];
                }
            }
            else if(imageLost[y][1]==1&&imageLost[y][0]==0)//ɨ������
            {
                // ���ɨ�費����߽磬������һ�е������������
                if((imageLost[y+1][1]==0&&imageLost[y+1][0]==0))
                {
//                    state=1;
                    // ������һ�еĴ��ڵı߽���㵱ǰ�е���߽磬�����µ�·���
                    imageOut[y][0]=imageOut[y+1][0]-(imageOut[y][1]-imageOut[y+1][1]);
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
                else
                {
                    // ������һ�еĵ�·��ȼ��㵱ǰ�е���߽磬�����µ�·���
                    imageOut[y][0]=imageOut[y][1]-RoadWidth[y+1];
                    RoadWidth[y]=RoadWidth[y+1];
                }
            }
            else if(imageLost[y][1]==0&&imageLost[y][0]==0)//��ɨ����
            {
                // �����ɨ�費���߽磬��������߽�Ϊ0���ұ߽�Ϊ��Ļ�����-1�����̳���һ�еĵ�·���
                imageOut[y][1]=LCDW-1;
                imageOut[y][0]=0;
                RoadWidth[y]=RoadWidth[y+1];
            }
            else if(imageLost[y][1]==1&&imageLost[y][0]==1)
            {
                // ������߶�ɨ�赽�˱߽磬��������߽�Ĳ�ֵ�ж��Ƿ���Ҫ������·���
//                state=0;
                if((imageOut[y][1]-imageOut[y][0])>RoadWidth[y+1])
                {
                    RoadWidth[y]=RoadWidth[y+1];
                }
                else
                {
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
            }

            if(imageInput[y][AuxiliaryLine[y+1]]&&y>20&&y<50)
            {
                // ������ض���Χ�ڣ��������ϴ��ڷ������أ������һ�н�������
                for(x=AuxiliaryLine[y+2];x<LCDW-1;x++)//����ɨ�� AuxiliaryLine[y+1] LCDW-1 Bounds[LCDH-1][1]
                 {
                     if(imageInput[y+1][x])
                     {
                         // ��������������أ��������һ�е��ұ߽磬�����Ϊ����
                         imageOut[y+1][1]=x;
                         imageLost[y+1][1]=1;
                         break;
                     }
                     else    imageLost[y+1][1]=0;
                 }
                 for(x=AuxiliaryLine[y+2];x>0;x--)//����ɨ�� 0
                 {
                     if(imageInput[y+1][x])
                     {
                         // ��������������أ��������һ�е���߽磬�����Ϊ����
                         imageOut[y+1][0]=x;
                         imageLost[y+1][0]=1;
                         break;
                     }
                     else    imageLost[y+1][0]=0;
                 }
                 if(imageLost[y+1][0]==0&&imageLost[y+1][1]==1)
                 {
                     // �����һ��ɨ�費����߽磬��Ե�ǰ�����ұ߽������������·�����Ϊ20
                     imageOut[y][0]=imageOut[y][0]+30;
                     imageOut[y][1]=imageOut[y][1]+30;
                     RoadWidth[y]=20;
                 }
                 else if(imageLost[y+1][0]==1&&imageLost[y+1][1]==0)
                 {
                     // �����һ��ɨ�費���ұ߽磬��Ե�ǰ�����ұ߽������������·�����Ϊ20
                     imageOut[y][0]=imageOut[y][0]-30;
                     imageOut[y][1]=imageOut[y][1]-30;
                     RoadWidth[y]=20;
                 }

            }
        }

        AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
    }
}
void ImageGetSideRound(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2])
{
    int x,y,state;
    int AuxiliaryLine[LCDH],RoadWidth[LCDH];
    int left_white_num,right_white_num;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    RoadWidth[LCDH-1]=LCDW-1;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    state=0;
    left_white_num=0;
    right_white_num=0;

    ////////////////////////////////////////���������ɨ����ʼ��///////////////////////////////////////
    ////////////////////////////////�����ͼ��������Ϊ��ɫ,����Ǻ�ɫ//////////////////////////////////
    if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//������м�Ϊ��ɫ
    {
        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
        {
            if(imageInput[LCDH-1][x])
            {
                right_white_num++;
            }
        }
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
        {
            if(imageInput[LCDH-1][x])
            {
                left_white_num++;
            }
        }

        if(right_white_num>left_white_num&&right_white_num>3)
        {
            AuxiliaryLine[LCDH-1]=LCDW-6;
        }
        else if(left_white_num>right_white_num&&left_white_num>3)
        {
            AuxiliaryLine[LCDH-1]=5;
        }
        else
        {
            if(!BarrierFlag&&!OpenRoadFlag)
            {
                RoadOut_flag=1;//����������
            }
            else{}
        }
    }
    else
    {
        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
        {
            if(imageInput[LCDH-1][x])
            {
                right_white_num++;
            }
        }
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
        {
            if(imageInput[LCDH-1][x])
            {
                left_white_num++;
            }
        }

        if(right_white_num>left_white_num)
        {
            AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]+right_white_num/2;
        }
        else
        {
            AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]-left_white_num/2;
        }
    }

    /////////////////////////////////////////////////����˫��ɨ��///////////////////////////////////////////////////////
    for(y=LCDH-2;y>0;y--)//ɨ��
    {

        if(y==LCDH-2)
        {
            for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//����ɨ�� AuxiliaryLine[y+1] LCDW-1 Bounds[LCDH-1][1]
            {
                if(imageInput[y][x])
                {
                    imageOut[y][1]=x;
                    break;
                }
                else
                {
                    imageOut[y][1]=x;
                }
            }
            for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//����ɨ�� 0
            {
                if(imageInput[y][x])
                {
                    imageOut[y][0]=x;
                    break;
                }
                else
                {
                    imageOut[y][0]=x;
                }
            }
        }
        else//���������п�ʼ
        {
            if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/2&&(imageOut[y+2][1]-imageOut[y+1][1])>6)//���Ԥ��
            {
                imageOut[y][0]=imageOut[y+1][0]-10;//6
                imageOut[y][1]=imageOut[y+1][1]-10;
            }
            else if(imageOut[y+1][0]>LCDW/2&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>6)//���Ԥ��
            {
                imageOut[y][0]=imageOut[y+1][0]+10;
                imageOut[y][1]=imageOut[y+1][1]+10;//6
            }
            else//����ɨ��
            {
                for(x=imageOut[y+1][1]-10;x<Bounds[y][1];x++)//����ɨ�� AuxiliaryLine[y+1] LCDW-1 Bounds[LCDH-1][1]
                {
                    if(imageInput[y][x])
                    {
                        imageOut[y][1]=x;
                        break;
                    }
                    else
                    {
                        imageOut[y][1]=x;
                    }
                }
                for(x=imageOut[y+1][0]+10;x>Bounds[y][0];x--)//����ɨ�� 0
                {
                    if(imageInput[y][x])
                    {
                        imageOut[y][0]=x;
                        break;
                    }
                    else
                    {
                        imageOut[y][0]=x;
                    }
                }
            }
        }
    }
}
/*
 * Ԫ�ص���ɨ��
 * status:0���ұ߶��ߣ�ɨ���    1����߶���ɨ�ұ�
 * */
void ImageGetSideElementsJudge(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status)
{
    int x,y;
    int AuxiliaryLine[LCDH];
    int left_white_num,right_white_num;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    left_white_num=0;
    right_white_num=0;

    switch(status)
    {
        case 0:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//������м�Ϊ��ɫ
            {

                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num&&right_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5*4;
                }
                else if(left_white_num>right_white_num&&left_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5;
                }
                else
                {
                    RoadOut_flag=1;
                }
            }
            else
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num)
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]+right_white_num/2;
                }
                else
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]-left_white_num/2;
                }

            }

            for(y=LCDH-2;y>0;y--)//ɨ��
            {
                if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]-8;//6
                    imageOut[y][1]=imageOut[y+1][1]-8;
                }
                else if(imageOut[y+1][0]>LCDW/3*1&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]+8;
                    imageOut[y][1]=imageOut[y+1][1]+8;//6
                }
                else
                {
                    for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//����ɨ��
                    {
                        if(imageInput[y][x])
                        {

                            imageOut[y][0]=x;
                            break;
                        }
                        else imageOut[y][0]=imageOut[y+1][0];
                    }
                    imageOut[y][1]=imageOut[y][0]+Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;

        case 1:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//������м�Ϊ��ɫ
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num&&right_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5*4;
                }
                else if(left_white_num>right_white_num&&left_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5;
                }
                else
                {
                    RoadOut_flag=1;
                }
            }
            else
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num)
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]+right_white_num/2;
                }
                else
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]-left_white_num/2;
                }

            }

            for(y=LCDH-2;y>0;y--)//ɨ��
            {
                if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]-8;//6
                    imageOut[y][1]=imageOut[y+1][1]-8;
                }
                else if(imageOut[y+1][0]>LCDW/3*1&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]+8;
                    imageOut[y][1]=imageOut[y+1][1]+8;//6
                }
                else
                {
                    for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//����ɨ��
                    {
                        if(imageInput[y][x])
                        {
                            imageOut[y][1]=x;
                            break;
                        }
                        else imageOut[y][1]=imageOut[y+1][1];
                    }

                    imageOut[y][0]=imageOut[y][1]-Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;
    }
}
/*
 * Բ���յ㵥��ɨ��
 * status:0���ұ߶��ߣ�ɨ���    1����߶���ɨ�ұ�
 * */
void ImageGetSideRoundArc(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status)
{
    int x,y,state=0;
    int AuxiliaryLine[LCDH];
    int left_white_num,right_white_num;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    left_white_num=0;
    right_white_num=0;

    switch(status)
    {
        case 0:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//������м�Ϊ��ɫ
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num&&right_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5*4;
                }
                else if(left_white_num>right_white_num&&left_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5;
                }
                else
                {
                    RoadOut_flag=1;
                }
            }
            else
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num)
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]+right_white_num/2;
                }
                else
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]-left_white_num/2;
                }
            }

            for(y=LCDH-2;y>0;y--)//ɨ��
            {
                if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]-8;//6
                    imageOut[y][1]=imageOut[y+1][1]-8;
                }
                else if(imageOut[y+1][0]>LCDW/3*1&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]+8;
                    imageOut[y][1]=imageOut[y+1][1]+8;//6
                }
                else
                {
                    for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//����ɨ��
                    {
                        if(imageInput[y][x])
                        {
                            imageOut[y][0]=x;
                            break;
                        }
                        else if(!imageInput[y][x]&&x==Bounds[y][0]+1/*&&y<40*/)
                        {
                            state=1;
                        }
                        else
                        {
                            imageOut[y][0]=x;
                        }
                    }

                    if(state)
                    {
                        imageOut[y][0]=imageOut[y+1][0]-1;
                    }

                    imageOut[y][1]=imageOut[y][0]+Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;

        case 1:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//������м�Ϊ��ɫ
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num&&right_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5*4;
                }
                else if(left_white_num>right_white_num&&left_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5;
                }
                else
                {
                    RoadOut_flag=1;
                }
            }
            else
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num)
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]+right_white_num/2;
                }
                else
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]-left_white_num/2;
                }
            }

            for(y=LCDH-2;y>0;y--)//ɨ��
            {
                if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]-8;//6
                    imageOut[y][1]=imageOut[y+1][1]-8;
                }
                else if(imageOut[y+1][0]>LCDW/3*1&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]+8;
                    imageOut[y][1]=imageOut[y+1][1]+8;//6
                }
                else
                {
                    for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//����ɨ��
                    {
                        if(imageInput[y][x])
                        {
                            imageOut[y][1]=x;
                            break;
                        }
                        else if(!imageInput[y][x]&&x==Bounds[y][1]-1/*&&y<40*/)
                        {
                            state=1;
                        }
                        else
                        {
                            imageOut[y][1]=x;
                        }
                    }
                    if(state)
                    {
                        imageOut[y][1]=imageOut[y+1][1]+1;
                    }

                    imageOut[y][0]=imageOut[y][1]-Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;
    }
}
/*
 * Բ����������ɨ��
 * status:0���ұ߶��ߣ�ɨ���    1����߶���ɨ�ұ�
 * */
void ImageGetSideRoundOut(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status)
{
    int x,y,state=0;
    int AuxiliaryLine[LCDH];
    int left_white_num,right_white_num;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    left_white_num=0;
    right_white_num=0;

    switch(status)
    {
        case 0:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//������м�Ϊ��ɫ
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num&&right_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5*4;
                }
                else if(left_white_num>right_white_num&&left_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5;
                }
                else
                {
                    RoadOut_flag=1;
                }
            }
            else
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num)
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]+right_white_num/2;
                }
                else
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]-left_white_num/2;
                }
            }

            for(y=LCDH-2;y>0;y--)//ɨ��
            {
                if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]-8;//6
                    imageOut[y][1]=imageOut[y+1][1]-8;
                }
                else if(imageOut[y+1][0]>LCDW/3*1&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]+8;
                    imageOut[y][1]=imageOut[y+1][1]+8;//6
                }
                else
                {
                    for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//����ɨ��
                    {
                        if(imageInput[y][x])
                        {
                            imageOut[y][0]=x;
                            break;
                        }
                        else if(!imageInput[y][x]&&x==(Bounds[y][0]+1)&&y<40)
                        {
                            state=1;
                        }
                        else
                        {
                            imageOut[y][0]=x;
                        }
                    }

                    if(state)
                    {
                        imageOut[y][0]=imageOut[y+1][0]-1;
                    }

                    imageOut[y][1]=imageOut[y][0]+Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;

        case 1:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//������м�Ϊ��ɫ
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num&&right_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5*4;
                }
                else if(left_white_num>right_white_num&&left_white_num>3)
                {
                    AuxiliaryLine[LCDH-1]=LCDW/5;
                }
                else
                {
                    RoadOut_flag=1;
                }
            }
            else
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//����ɨ��
                {
                    if(imageInput[LCDH-1][x])
                    {
                        left_white_num++;
                    }
                }

                if(right_white_num>left_white_num)
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]+right_white_num/2;
                }
                else
                {
                    AuxiliaryLine[LCDH-1]=AuxiliaryLine[LCDH-1]-left_white_num/2;
                }
            }

            for(y=LCDH-2;y>0;y--)//ɨ��
            {
                if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]-8;//6
                    imageOut[y][1]=imageOut[y+1][1]-8;
                }
                else if(imageOut[y+1][0]>LCDW/3*1&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
                {
                    imageOut[y][0]=imageOut[y+1][0]+8;
                    imageOut[y][1]=imageOut[y+1][1]+8;//6
                }
                else
                {
                    for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//����ɨ��
                    {
                        if(imageInput[y][x])
                        {
                            imageOut[y][1]=x;
                            break;
                        }
                        else if(!imageInput[y][x]&&x==(Bounds[y][1]-1)&&y<40)
                        {
                            state=1;
                        }
                        else
                        {
                            imageOut[y][1]=x;
                        }
                    }
                    if(state)
                    {
                        imageOut[y][1]=imageOut[y+1][1]+1;
                    }

                    imageOut[y][0]=imageOut[y][1]-Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;
    }
}

/*��ȡֱ���߽�*/
void ImageGetbounds (int imageInput[LCDH][LCDW],int imageOut[LCDH][2])//imageInput����ԭʼͼ��imageOut����߽�
{
    int i,j;
    for(i=0;i<LCDH;i++)
    {
       for(j=(LCDW-1)/2;j<LCDW;j++)
       {
          if(!imageInput[i][j])// �����ǰ����Ϊ��ɫ
          {
              imageOut[i][1]=j;//���±߽�
              break;
          }
       }
       for(j=(LCDW-1)/2;j>0;j--)
       {
           if(!imageInput[i][j])
           {
               imageOut[i][0]=j;
               break;
           }
       }
    }
}
void ImageSideTransform(int imageInput[LCDH][2],uint8 imageOut1[LCDH],uint8 imageOut2[LCDH])
{
    int i;
    for(i=LCDH-1;i>=0;i--)
    {
        if(imageInput[i][0]>=0&&imageInput[i][0]<=100)
        {
            imageOut1[i]=(uint8)imageInput[i][0];
        }
        else imageOut1[i]=0;
        if(imageInput[i][1]>=0&&imageInput[i][1]<=100)
        {
            imageOut2[i]=(uint8)imageInput[i][1];
        }
        else imageOut2[i]=100;
    }
}
