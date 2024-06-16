/*
 * ScanLine.c
 *
 *  Created on: 2023年3月24日
 *      Author: 叁召a
 */


#include "ScanLine.h"


int ImageSide[LCDH][2],RoadLostFlag[LCDH][2],UpSide[LCDW];
int MidLine[LCDH];
uint8 RoadOut_flag=0;
uint8 Block=0,BlockBar=0;

/*道路宽度数组*/
uint8 Track_Width[60]={
                        20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
                        20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
                        20, 22, 24, 25, 26, 28, 30, 30, 32, 35,
                        38, 40, 40, 42, 44, 45, 46, 48, 49, 51,
                        52, 53, 55, 55, 58, 60, 61, 62, 63, 64,
                        64, 66, 67, 69, 69, 71, 72, 74, 74, 99
                      };

/*规定边界*/
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

/*规定边界*/
//uint8 Bounds[60][2]={
//        30,29,29,28,28,27,26,26,25,25,24,23,23,22,22,21,21,20,19,19,18,18,17,16,16,15,15,14,13,13,12,12,11,10,10, 9, 9, 8, 8, 7, 6, 6, 5, 5, 4, 3, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//        69,70,70,71,71,72,73,73,74,74,75,76,76,77,77,78,78,79,80,80,81,81,82,83,83,84,84,85,86,86,87,87,88,89,89,90,90,91,91,92,92,92,94,94,95,96,96,97,97,98,99,99,99,99,99,99,99,99,99,99
//                    };

//uint8 Bounds[60][2]={
//                        30, 69,29, 70,29, 70,28, 71,28, 71,
//                        27, 72,26, 73,26, 73,25, 74,25, 74,
//                        24, 75,23, 76,23, 76,22, 77,22, 77,
//                        21, 78,21, 78,20, 79,19, 80,19, 80,
//                        18, 81,18, 81,17, 82,16, 83,16, 83,
//                        15, 84,15, 84,14, 85,13, 86,13, 86,
//                        12, 87,12, 87,11, 88,10, 89,10, 89,
//                         9, 90, 9, 90, 8, 91, 8, 91, 7, 92,
//                         6, 93, 6, 93, 5, 94, 5, 94, 4, 95,
//                         3, 96, 3, 96, 2, 97, 2, 97, 1, 98,
//                         0, 99, 0, 99, 0, 99, 0, 99, 0, 99,
//                         0, 99, 0, 99, 0, 99, 0, 99, 0, 99
//};

void ImageGetSide(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],int imageLost[LCDH][2])
{
    int x,y,state,blockstate;
    int AuxiliaryLine[LCDH],RoadWidth[LCDH];
    int left_white_num,right_white_num;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    RoadWidth[LCDH-1]=(LCDW-1)/8*5;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    imageLost[LCDH-1][1]=1;
    imageLost[LCDH-1][0]=1;
    state=0;
    blockstate=1;
    left_white_num=0;
    right_white_num=0;


    ////////////////////////////////////////先找最底行扫线起始点///////////////////////////////////////
    ////////////////////////////////最底行图像是赛道为白色,外边是黑色//////////////////////////////////
    if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//最底行中间为黑色
    {

        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
        {
            if(imageInput[LCDH-1][x])
            {
                right_white_num++;
            }
        }
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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
                RoadOut_flag=1;//出赛道保护
            }
            else{}
        }
    }
    else
    {
        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
        {
            if(imageInput[LCDH-1][x])
            {
                right_white_num++;
            }
        }
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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

    /////////////////////////////////////////////////正常双边扫线///////////////////////////////////////////////////////
    for(y=LCDH-2;y>0;y--)//扫描
    {
        if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/2&&(imageOut[y+2][1]-imageOut[y+1][1])>6)//弯道预测
        {
            imageOut[y][0]=imageOut[y+1][0]-10;//6
            imageOut[y][1]=imageOut[y+1][1]-10;
            imageLost[y][0]=1;
            imageLost[y][1]=1;
        }
        else if(imageOut[y+1][0]>LCDW/2&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>6)//弯道预测
        {
            imageOut[y][0]=imageOut[y+1][0]+10;
            imageOut[y][1]=imageOut[y+1][1]+10;//6
            imageLost[y][0]=1;
            imageLost[y][1]=1;
        }
        else//正常扫线
        {
            for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//向右扫描 AuxiliaryLine[y+1] LCDW-1 Bounds[LCDH-1][1]
            {
                if(imageInput[y][x])
                {
                    imageOut[y][1]=x;
                    imageLost[y][1]=1;
                    break;
                }
                else    imageLost[y][1]=0;
            }
            for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//向左扫描 0
            {
                if(imageInput[y][x])
                {
                    imageOut[y][0]=x;
                    imageLost[y][0]=1;
                    break;
                }
                else    imageLost[y][0]=0;
            }
            if(imageLost[y][1]==0&&imageLost[y][0]==1)//扫不到右
            {
                if((imageLost[y+1][1]==0&&imageLost[y+1][0]==0)||state==1)
                {
                    state=1;
                    imageOut[y][1]=imageOut[y+1][1]-(imageOut[y][0]-imageOut[y+1][0]);
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
                else
                {
                    imageOut[y][1]=imageOut[y][0]+RoadWidth[y+1];//y为数组中的x值
                    RoadWidth[y]=RoadWidth[y+1];
                }
            }
            else if(imageLost[y][1]==1&&imageLost[y][0]==0)//扫不到左
            {
                if((imageLost[y+1][1]==0&&imageLost[y+1][0]==0)||state==1)
                {
                    state=1;
                    imageOut[y][0]=imageOut[y+1][0]-(imageOut[y][1]-imageOut[y+1][1]);
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
                else
                {
                    imageOut[y][0]=imageOut[y][1]-RoadWidth[y+1];
                    RoadWidth[y]=RoadWidth[y+1];
                }
            }
            else if(imageLost[y][1]==0&&imageLost[y][0]==0)//都扫不到
            {
                imageOut[y][1]=LCDW-1;
                imageOut[y][0]=0;
                RoadWidth[y]=RoadWidth[y+1];//？？？？
            }
            else if(imageLost[y][1]==1&&imageLost[y][0]==1)
            {
                state=0;
                if((imageOut[y][1]-imageOut[y][0])>RoadWidth[y+1])
                {
                    //imageOut[y][1]=imageOut[y+1][1];
                    //imageOut[y][0]=imageOut[y+1][0];
                    RoadWidth[y]=RoadWidth[y+1];
                }
                else
                {
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
            }
        }

//        if(y>20&&imageOut[y][1]==imageOut[y][0]&&!OpenRoadFlag)
//        {
//            Block=1;//赛道封闭
//        }
//
//        if(y>15&&imageOut[y][1]==imageOut[y][0]&&!OpenRoadFlag)
//        {
//            BlockBar=1;//赛道封闭
//        }

        AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
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

//        for(y=LCDH-1;y>0;y--)
//        {
////            Bounds[y][0]=1;
//            Bin_Image[y][Bounds[y][0]]= 1;
//            Bin_Image[y][Bounds[y][1]]= 1;
//        }

}

/*
 * 元素单边扫线
 * status:0是右边丢线，扫左边    1是左边丢线扫右边
 * */
void ImageGetSideElementsJudge(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status)
{
    int x,y,state;
    int AuxiliaryLine[LCDH],RoadWidth[LCDH];
    int left_white_num,right_white_num;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    left_white_num=0;
    right_white_num=0;

    switch(status)
    {
        case 0:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//最低行中间为黑色
            {

                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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

            for(y=LCDH-2;y>0;y--)//扫描
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
                    for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//向左扫描
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
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//最低行中间为黑色
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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

            for(y=LCDH-2;y>0;y--)//扫描
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
                    for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//向右扫描
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
 * 圆环拐点单边扫线
 * status:0是右边丢线，扫左边    1是左边丢线扫右边
 * */
void ImageGetSideRoundArc(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status)
{
    int x,y,state=0;
    int AuxiliaryLine[LCDH],RoadWidth[LCDH];
    int left_white_num,right_white_num;
    int index=0;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    left_white_num=0;
    right_white_num=0;

    switch(status)
    {
        case 0:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//最低行中间为黑色
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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

            for(y=LCDH-2;y>0;y--)//扫描
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
                    for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//向左扫描
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
                        imageOut[y][0]=imageOut[y+1][0]-2;
                    }

                    imageOut[y][1]=imageOut[y][0]+Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;

        case 1:
            if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//最低行中间为黑色
            {
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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
                for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)//向右扫描
                {
                    if(imageInput[LCDH-1][x])
                    {
                        right_white_num++;
                    }
                }
                for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)//向左扫描
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

            for(y=LCDH-2;y>0;y--)//扫描
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
                    for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//向右扫描
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
                        imageOut[y][1]=imageOut[y+1][1]+2;
                    }

                    imageOut[y][0]=imageOut[y][1]-Track_Width[y];
                }
                AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
            }
            break;
    }
}

/*
 * 圆环出环扫线
 * status:0是右边丢线，扫左边    1是左边丢线扫右边
 * */
void ImageGetSideRoundOut(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],uint8 status)
{
    int x,y,state=0;
    int AuxiliaryLine[LCDH],RoadWidth[LCDH];
    int left_white_num,right_white_num;
    int index=0;
    AuxiliaryLine[LCDH-1]=(LCDW-1)/2;
    imageOut[LCDH-1][1]=LCDW-1;
    imageOut[LCDH-1][0]=0;
    left_white_num=0;
    right_white_num=0;

    switch(status)
    {
        int x,y,state=0;
        int AuxiliaryLine[COLH],RoadWidth[COLH];
        int left_white_num,right_white_num;
        int index=0;
        AuxiliaryLine[COLH-1]=(COLW-1)/2;
        imageOut[COLH-1][1]=COLW-1;
        imageOut[COLH-1][0]=0;
        left_white_num=0;
        right_white_num=0;

        switch(status)
        {
            case 0:
                if(!imageInput[COLH-1][AuxiliaryLine[COLH-1]])//最低行中间为黑色
                {
                    for(x=AuxiliaryLine[COLH-1];x<Bounds[COLH-1][1];x++)//向右扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            right_white_num++;
                        }
                    }
                    for(x=AuxiliaryLine[COLH-1];x>Bounds[COLH-1][0];x--)//向左扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            left_white_num++;
                        }
                    }

                    if(right_white_num>left_white_num&&right_white_num>3)
                    {
                        AuxiliaryLine[COLH-1]=COLW/5*4;
                    }
                    else if(left_white_num>right_white_num&&left_white_num>3)
                    {
                        AuxiliaryLine[COLH-1]=COLW/5;
                    }
                    else
                    {
                        RoadOut_flag=1;
                    }
                }
                else
                {
                    for(x=AuxiliaryLine[COLH-1];x<Bounds[COLH-1][1];x++)//向右扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            right_white_num++;
                        }
                    }
                    for(x=AuxiliaryLine[COLH-1];x>Bounds[COLH-1][0];x--)//向左扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            left_white_num++;
                        }
                    }

                    if(right_white_num>left_white_num)
                    {
                        AuxiliaryLine[COLH-1]=AuxiliaryLine[COLH-1]+right_white_num/2;
                    }
                    else
                    {
                        AuxiliaryLine[COLH-1]=AuxiliaryLine[COLH-1]-left_white_num/2;
                    }
                }

                for(y=COLH-2;y>0;y--)//扫描
                {
    //                if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
    //                {
    //                    imageOut[y][0]=imageOut[y+1][0]-8;//6
    //                    imageOut[y][1]=imageOut[y+1][1]-8;
    //                }
                    /*else */if(imageOut[y+1][0]>COLW/3*1&&imageOut[y+2][0]>COLW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
                    {
                        imageOut[y][0]=imageOut[y+1][0]+8;
                        imageOut[y][1]=imageOut[y+1][1]+8;//6
                    }
                    else
                    {
                        for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//向左扫描
                        {
                            if(imageInput[y][x])
                            {
                                imageOut[y][0]=x;
                                break;
                            }
                            else if(!imageInput[y][x]&&x==Bounds[y][0]+1&&y<40)
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
                            imageOut[y][0]=imageOut[y+1][0]+6;//大圆环：0
                        }

                        if(y>=20&&y<=50&&imageOut[y][0]<30)
                        {
                            index++;
                            if(index>=29&&y==20)
                            {
                                for(y=50;y>20;y--)
                                {
                                    imageOut[y][0]=imageOut[0][1]+Track_Width[y]/2;
                                }
                            }
                        }

                        imageOut[y][1]=imageOut[y][0]+Track_Width[y];
                    }
                    AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
                }
                break;

            case 1:
                if(!imageInput[COLH-1][AuxiliaryLine[COLH-1]])//最低行中间为黑色
                {
                    for(x=AuxiliaryLine[COLH-1];x<Bounds[COLH-1][1];x++)//向右扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            right_white_num++;
                        }
                    }
                    for(x=AuxiliaryLine[COLH-1];x>Bounds[COLH-1][0];x--)//向左扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            left_white_num++;
                        }
                    }

                    if(right_white_num>left_white_num&&right_white_num>3)
                    {
                        AuxiliaryLine[COLH-1]=COLW/5*4;
                    }
                    else if(left_white_num>right_white_num&&left_white_num>3)
                    {
                        AuxiliaryLine[COLH-1]=COLW/5;
                    }
                    else
                    {
                        RoadOut_flag=1;
                    }
                }
                else
                {
                    for(x=AuxiliaryLine[COLH-1];x<Bounds[COLH-1][1];x++)//向右扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            right_white_num++;
                        }
                    }
                    for(x=AuxiliaryLine[COLH-1];x>Bounds[COLH-1][0];x--)//向左扫描
                    {
                        if(imageInput[COLH-1][x])
                        {
                            left_white_num++;
                        }
                    }

                    if(right_white_num>left_white_num)
                    {
                        AuxiliaryLine[COLH-1]=AuxiliaryLine[COLH-1]+right_white_num/2;
                    }
                    else
                    {
                        AuxiliaryLine[COLH-1]=AuxiliaryLine[COLH-1]-left_white_num/2;
                    }
                }

                for(y=COLH-2;y>0;y--)//扫描
                {
                    if(imageOut[y+1][1]<COLW/2&&imageOut[y+2][1]<COLW/3*2&&(imageOut[y+2][1]-imageOut[y+1][1])>4)
                    {
                        imageOut[y][0]=imageOut[y+1][0]-8;//6
                        imageOut[y][1]=imageOut[y+1][1]-8;
                    }
    //                else if(imageOut[y+1][0]>LCDW/3*1&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>4)
    //                {
    //                    imageOut[y][0]=imageOut[y+1][0]+8;
    //                    imageOut[y][1]=imageOut[y+1][1]+8;//6
    //                }
                    else
                    {
                        for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//向右扫描
                        {
                            if(imageInput[y][x])
                            {
                                imageOut[y][1]=x;
                                break;
                            }
                            else if(!imageInput[y][x]&&x==Bounds[y][1]-1&&y<40)
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
                            imageOut[y][1]=imageOut[y+1][1]-6;//大圆环：0
                            state=0;
                        }

                        if(y>=20&&y<=50&&imageOut[y][1]>70)
                        {
                            index++;
                            if(index>=29&&y==20)
                            {
                                for(y=COLH-2;y>0;y--)
                                {
                                    imageOut[y][1]=imageOut[y][1]-Track_Width[y]/2;
                                }
                            }
                        }

                        imageOut[y][0]=imageOut[y][1]-Track_Width[y];
                    }
                    AuxiliaryLine[y]=(imageOut[y][1]+imageOut[y][0])/2;
                }
                break;
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


//-------------------------------------------------------------------------------------------------------------------
//  @brief      摄像头判断是否出界
//  @param      void
//  @return     void
//  Sample usage:    Outside_protect(100);
//-------------------------------------------------------------------------------------------------------------------
uint8 Out_flag;
void Outside_protect(uint8 value)
{
    uint8 j=0;
    for(int16 x = 0; x < COLW - 1; x++)
    {
        if(Bin_Image[COLH -45][x] == 0)
        {
            j++;
            if(j > value)  //摄像头识别出界
            {
                Out_flag = 1;
            }
        }
    }
}

