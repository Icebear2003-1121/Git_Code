#include "ScanLine.h"

int ImageSide[LCDH][2], ImageSideRound[LCDH][2], RoadLostFlag[LCDH][2], UpSide[LCDW];  // 图像边缘点坐标、圆环边缘坐标、路线丢失标志、上边缘信息
int ImageSideEro[LCDH][2];  // 经过腐蚀操作后的边缘点坐标
int rightlostpointnum = 0, leftlostpointnum = 0;  // 右侧和左侧丢失点的数量
int MidLine[LCDH];  // 中线信息
uint8 RoadOut_flag = 0;  // 道路是否超出图像范围的标志
uint8 Block = 0, BlockBar = 0;  // 障碍物和障碍物条的数量

/*道路宽度数组*/
uint8 Track_Width[60]={
                        8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
                        8,  8,  8,  8,  8,  8,  8,  8, 10, 12,
                        14, 16, 18, 20, 22, 24, 26, 28, 31, 33,
                        36, 38, 39, 41, 42, 44, 47, 49, 51, 53,
                        55, 56, 58, 60, 62, 65, 66, 68, 69, 71,
                        73, 75, 76, 77, 79, 81, 82, 83, 83, 99
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

void ImageGetSide(uint8 imageInput[LCDH][LCDW],int imageOut[LCDH][2],int imageLost[LCDH][2])
{
    int x, y, state, blockstate;  //变量x，y表示坐标，state表示状态，blockstate表示障碍物状态
    int AuxiliaryLine[LCDH], RoadWidth[LCDH];  //辅助线和道路宽度数组
    int left_white_num, right_white_num;  //左侧和右侧白色像素数目

    AuxiliaryLine[LCDH - 1] = (LCDW - 1) / 2;  //设置辅助线最后一行的值为图像宽度的一半
    RoadWidth[LCDH - 1] = LCDW - 1;  //设置道路宽度最后一行的值为图像宽度的五分之四
    imageOut[LCDH - 1][1] = LCDW - 1;  //设置图像输出最后一行的右端点坐标为图像宽度减1
    imageOut[LCDH - 1][0] = 0;  //设置图像输出最后一行的左端点坐标为0
    imageLost[LCDH - 1][1] = 1;  //设置图像丢失最后一行为1
    imageLost[LCDH - 1][0] = 1;  //设置图像丢失最后一行为1

    state = 0;  //将状态变量state设置为0
    blockstate = 1;  //将障碍物状态变量blockstate设置为1
    left_white_num = 0;  //将左侧白色像素数目变量初始化为0
    right_white_num = 0;  //将右侧白色像素数目变量初始化为0
    rightlostpointnum = 0;  //将右侧丢失点数量变量初始化为0
    leftlostpointnum = 0;  //将左侧丢失点数量变量初始化为0

    ////////////////////////////////////////先找最底行扫线起始点///////////////////////////////////////
    ////////////////////////////////最底行图像是赛道为白色,外边是黑色//////////////////////////////////
    if(!imageInput[LCDH-1][AuxiliaryLine[LCDH-1]])//如果最底行中间像素为黑色
    {
        // 向右扫描
        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)
        {
            if(imageInput[LCDH-1][x])// 如果当前像素为白色
            {
                right_white_num++; // 右侧白色像素数量加一
            }
        }

        // 向左扫描
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)
        {
            if(imageInput[LCDH-1][x])// 如果当前像素为白色
            {
                left_white_num++; // 左侧白色像素数量加一
            }
        }

        // 根据白色像素数量和阈值进行判断辅助线位置
        if(right_white_num > left_white_num && right_white_num > 3)
        {
            AuxiliaryLine[LCDH-1] = LCDW-6; // 将辅助线位置移动到LCDW-6处
        }
        else if(left_white_num > right_white_num && left_white_num > 3)
        {
            AuxiliaryLine[LCDH-1] = 5; // 将辅助线位置移动到5处
        }
        else
        {
            // 如果没有障碍物并且不需要开启新道路，则设置出赛道保护标志
            if(!BarrierFlag && !OpenRoadFlag)
            {
                RoadOut_flag = 1; // 出赛道保护标志置1
            }
            else
            {
                // 其他情况为空
            }
        }
    }
    else
    {
        // 向右扫描
        for(x=AuxiliaryLine[LCDH-1];x<Bounds[LCDH-1][1];x++)
        {
            if(imageInput[LCDH-1][x])// 如果当前像素为白色
            {
                right_white_num++; // 右侧白色像素数量加一
            }
        }

        // 向左扫描
        for(x=AuxiliaryLine[LCDH-1];x>Bounds[LCDH-1][0];x--)
        {
            if(imageInput[LCDH-1][x])// 如果当前像素为白色
            {
                left_white_num++; // 左侧白色像素数量加一
            }
        }

        // 根据白色像素数量判断辅助线位置的调整方向
        if(right_white_num > left_white_num)
        {
            AuxiliaryLine[LCDH-1] = AuxiliaryLine[LCDH-1] + right_white_num/2; // 将辅助线位置向右移动一半的白色像素数量
        }
        else
        {
            AuxiliaryLine[LCDH-1] = AuxiliaryLine[LCDH-1] - left_white_num/2; // 将辅助线位置向左移动一半的白色像素数量
        }
    }

    /////////////////////////////////////////////////正常双边扫线///////////////////////////////////////////////////////
    for(y=LCDH-2;y>15;y--)//扫描，从倒数第二行开始向上扫描
    {
        if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/2&&(imageOut[y+2][1]-imageOut[y+1][1])>6)//弯道预测
        {
            // 判断是否为弯道
            // 如果下方两个像素点的 x 坐标都小于屏幕宽度的一半，并且两个像素点之间的差值大于6，则判断为弯道
            // 对当前行进行修正，左边界向左移动10个单位，右边界向左移动10个单位
            imageOut[y][0]=imageOut[y+1][0]-10;
            imageOut[y][1]=imageOut[y+1][1]-10;
            imageLost[y][0]=1;
            imageLost[y][1]=1;
        }
        else if(imageOut[y+1][0]>LCDW/2&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>6)//弯道预测
        {
            // 判断是否为弯道
            // 如果下方两个像素点的 x 坐标都大于屏幕宽度的一半，并且两个像素点之间的差值大于6，则判断为弯道
            // 对当前行进行修正，左边界向右移动10个单位，右边界向右移动10个单位
            imageOut[y][0]=imageOut[y+1][0]+10;
            imageOut[y][1]=imageOut[y+1][1]+10;
            imageLost[y][0]=1;
            imageLost[y][1]=1;
        }
        else//正常扫线
        {
            // 正常扫描道路边界
            for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//向右扫描
            {
                if(imageInput[y][x])
                {
                    // 如果遇到非零像素，则更新右边界坐标，并标记为存在
                    imageOut[y][1]=x;
                    imageLost[y][1]=1;
                    if(y>25) rightlostpointnum++;
                    break;
                }
                else
                {
                    // 如果在该范围内没有遇到非零像素，则标记为不存在
                    imageLost[y][1]=0;
                }
            }
            for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//向左扫描
            {
                if(imageInput[y][x])
                {
                    // 如果遇到非零像素，则更新左边界坐标，并标记为存在
                    imageOut[y][0]=x;
                    imageLost[y][0]=1;
                    if(y>25) leftlostpointnum++;
                    break;
                }
                else
                {
                    // 如果在该范围内没有遇到非零像素，则标记为不存在
                    imageLost[y][0]=0;
                }
            }
            if(imageLost[y][1]==0&&imageLost[y][0]==1)//扫不到右
            {
                // 如果扫描不到右边界，根据上一行的情况进行修正
                if((imageLost[y+1][1]==0&&imageLost[y+1][0]==0))
                {
//                    state=1;
                    // 根据上一行的存在的边界计算当前行的右边界，并更新道路宽度
                    imageOut[y][1]=imageOut[y+1][1]-(imageOut[y][0]-imageOut[y+1][0]);
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
                else
                {
                    // 根据上一行的道路宽度计算当前行的右边界，并更新道路宽度
                    imageOut[y][1]=imageOut[y][0]+RoadWidth[y+1];//y为数组中的x值
                    RoadWidth[y]=RoadWidth[y+1];
                }
            }
            else if(imageLost[y][1]==1&&imageLost[y][0]==0)//扫不到左
            {
                // 如果扫描不到左边界，根据上一行的情况进行修正
                if((imageLost[y+1][1]==0&&imageLost[y+1][0]==0))
                {
//                    state=1;
                    // 根据上一行的存在的边界计算当前行的左边界，并更新道路宽度
                    imageOut[y][0]=imageOut[y+1][0]-(imageOut[y][1]-imageOut[y+1][1]);
                    RoadWidth[y]=imageOut[y][1]-imageOut[y][0];
                }
                else
                {
                    // 根据上一行的道路宽度计算当前行的左边界，并更新道路宽度
                    imageOut[y][0]=imageOut[y][1]-RoadWidth[y+1];
                    RoadWidth[y]=RoadWidth[y+1];
                }
            }
            else if(imageLost[y][1]==0&&imageLost[y][0]==0)//都扫不到
            {
                // 如果都扫描不到边界，则设置左边界为0，右边界为屏幕最大宽度-1，并继承上一行的道路宽度
                imageOut[y][1]=LCDW-1;
                imageOut[y][0]=0;
                RoadWidth[y]=RoadWidth[y+1];
            }
            else if(imageLost[y][1]==1&&imageLost[y][0]==1)
            {
                // 如果两边都扫描到了边界，则根据两边界的差值判断是否需要修正道路宽度
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
                // 如果在特定范围内，辅助线上存在非零像素，则对下一行进行修正
                for(x=AuxiliaryLine[y+2];x<LCDW-1;x++)//向右扫描 AuxiliaryLine[y+1] LCDW-1 Bounds[LCDH-1][1]
                 {
                     if(imageInput[y+1][x])
                     {
                         // 如果遇到非零像素，则更新下一行的右边界，并标记为存在
                         imageOut[y+1][1]=x;
                         imageLost[y+1][1]=1;
                         break;
                     }
                     else    imageLost[y+1][1]=0;
                 }
                 for(x=AuxiliaryLine[y+2];x>0;x--)//向左扫描 0
                 {
                     if(imageInput[y+1][x])
                     {
                         // 如果遇到非零像素，则更新下一行的左边界，并标记为存在
                         imageOut[y+1][0]=x;
                         imageLost[y+1][0]=1;
                         break;
                     }
                     else    imageLost[y+1][0]=0;
                 }
                 if(imageLost[y+1][0]==0&&imageLost[y+1][1]==1)
                 {
                     // 如果下一行扫描不到左边界，则对当前行左右边界进行修正，道路宽度设为20
                     imageOut[y][0]=imageOut[y][0]+30;
                     imageOut[y][1]=imageOut[y][1]+30;
                     RoadWidth[y]=20;
                 }
                 else if(imageLost[y+1][0]==1&&imageLost[y+1][1]==0)
                 {
                     // 如果下一行扫描不到右边界，则对当前行左右边界进行修正，道路宽度设为20
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

        if(y==LCDH-2)
        {
            for(x=AuxiliaryLine[y+1];x<Bounds[y][1];x++)//向右扫描 AuxiliaryLine[y+1] LCDW-1 Bounds[LCDH-1][1]
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
            for(x=AuxiliaryLine[y+1];x>Bounds[y][0];x--)//向左扫描 0
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
        else//倒数第三行开始
        {
            if(imageOut[y+1][1]<LCDW/2&&imageOut[y+2][1]<LCDW/2&&(imageOut[y+2][1]-imageOut[y+1][1])>6)//弯道预测
            {
                imageOut[y][0]=imageOut[y+1][0]-10;//6
                imageOut[y][1]=imageOut[y+1][1]-10;
            }
            else if(imageOut[y+1][0]>LCDW/2&&imageOut[y+2][0]>LCDW/2&&(imageOut[y+1][0]-imageOut[y+2][0])>6)//弯道预测
            {
                imageOut[y][0]=imageOut[y+1][0]+10;
                imageOut[y][1]=imageOut[y+1][1]+10;//6
            }
            else//正常扫线
            {
                for(x=imageOut[y+1][1]-10;x<Bounds[y][1];x++)//向右扫描 AuxiliaryLine[y+1] LCDW-1 Bounds[LCDH-1][1]
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
                for(x=imageOut[y+1][0]+10;x>Bounds[y][0];x--)//向左扫描 0
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
 * 元素单边扫线
 * status:0是右边丢线，扫左边    1是左边丢线扫右边
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
                        imageOut[y][0]=imageOut[y+1][0]-1;
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
 * 圆环出环单边扫线
 * status:0是右边丢线，扫左边    1是左边丢线扫右边
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

/*获取直道边界*/
void ImageGetbounds (int imageInput[LCDH][LCDW],int imageOut[LCDH][2])//imageInput输入原始图像，imageOut输出边界
{
    int i,j;
    for(i=0;i<LCDH;i++)
    {
       for(j=(LCDW-1)/2;j<LCDW;j++)
       {
          if(!imageInput[i][j])// 如果当前像素为黑色
          {
              imageOut[i][1]=j;//更新边界
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
