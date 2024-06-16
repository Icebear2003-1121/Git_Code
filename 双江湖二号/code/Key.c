#include "key.h"

uint8 key1_status = 1;       //开关状态变量
uint8 key2_status = 1;       //开关状态变量
uint8 key3_status = 1;       //开关状态变量
uint8 key4_status = 1;       //开关状态变量

uint8 sw_status=0;           //拨码开关状态变量

uint8 LeftGo=0;              //左出库
uint8 RightGo=0;             //右出库
uint8 Go=0;                  //直接出发


uint8  key_tem=0x00;         //长按的按键值与按键扫描程序过渡变量
uint8  key_bak=0x00;         //按键扫描程序过渡变量
uint8  keydown_data=0x00;    //按键按下后就返回的值
uint8  keyup_data=0x00;      //按键抬起返回值
uint16 key_time=0x00;        //按键按下之后的时间计数，该值乘以扫描一次按键函数的时间就等于按键按下的时间

void key_Init(void)
{
    //拨码开关初始化
    gpio_init(SW1,GPI,0,GPI_PULL_UP);
    gpio_init(SW2,GPI,0,GPI_PULL_UP);
    gpio_init(KEY1,GPI,0,GPI_PULL_UP);
    gpio_init(KEY2,GPI,0,GPI_PULL_UP);
    gpio_init(KEY3,GPI,0,GPI_PULL_UP);
    gpio_init(KEY4,GPI,0,GPI_PULL_UP);
}

/*拨码开关状态获取*/
void DIP_Switch_Status(void)
{
    if(gpio_get_level(SW1)==0&&gpio_get_level(SW2)==0)
    {
        sw_status=0;
    }
    if(gpio_get_level(SW1)==1&&gpio_get_level(SW2)==0)
    {
        sw_status=1;
    }
    if(gpio_get_level(SW1)==1&&gpio_get_level(SW2)==1)
    {
        sw_status=2;
    }
    if(gpio_get_level(SW1)==0&&gpio_get_level(SW2)==1)
    {
        sw_status=3;
    }
}
/*按键扫描*/
void key_scan(void)
{
     keyup_data=0;         //键抬起后按键值一次有效

     key1_status=gpio_get_level(KEY1);
     key2_status=gpio_get_level(KEY2);
     key3_status=gpio_get_level(KEY3);
     key4_status=gpio_get_level(KEY4);


    if(key1_status==0||key2_status==0||key3_status==0||key4_status==0)   //有键正按下
    {
        if(key1_status==0)      key_tem=1;
        else if(key2_status==0) key_tem=2;
        else if(key3_status==0) key_tem=3;
        else if(key4_status==0) key_tem=4;

        if (key_tem == key_bak)      //有键按下后第一次扫描不处理，与else配合第二次扫描有效，这样实现了去抖动
        {
           key_time++;             //有键按下后执行一次扫描函数，该变量加1
           keydown_data=key_tem;   //按键值赋予keydown_data

//           if( (mode==0)&&(key_time>1) )//key_time>1按键值无效，这就是单按，如果mode为1就为连按
//            keydown_data=0;
        }
        else                             //去抖动
        {
           key_time=0;
           key_bak=key_tem;
        }

    }
    else       //键抬起
    {
        if(key_time>2)            //按键抬起后返回一次按键值
        {
            keyup_data=key_tem;  //键抬起后按键值赋予keyupata
        }
        key_bak=0;               //要清零，不然下次执行扫描程序时按键的值跟上次按的值一样，就没有去抖动处理了
        key_time=0;
        keydown_data=0;
    }
}
void Keystroke_Depart(void)
{
    switch(keyup_data)
    {
        case 1:
            Go = 1;
            break;
        case 2:
            LeftGo = 1;
            break;
        case 3:
            RightGo = 1;
            break;
    }
}
