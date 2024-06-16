#include "key.h"

uint8 key1_status = 1;       //����״̬����
uint8 key2_status = 1;       //����״̬����
uint8 key3_status = 1;       //����״̬����
uint8 key4_status = 1;       //����״̬����

uint8 sw_status=0;           //���뿪��״̬����

uint8 LeftGo=0;              //�����
uint8 RightGo=0;             //�ҳ���
uint8 Go=0;                  //ֱ�ӳ���


uint8  key_tem=0x00;         //�����İ���ֵ�밴��ɨ�������ɱ���
uint8  key_bak=0x00;         //����ɨ�������ɱ���
uint8  keydown_data=0x00;    //�������º�ͷ��ص�ֵ
uint8  keyup_data=0x00;      //����̧�𷵻�ֵ
uint16 key_time=0x00;        //��������֮���ʱ���������ֵ����ɨ��һ�ΰ���������ʱ��͵��ڰ������µ�ʱ��

void key_Init(void)
{
    //���뿪�س�ʼ��
    gpio_init(SW1,GPI,0,GPI_PULL_UP);
    gpio_init(SW2,GPI,0,GPI_PULL_UP);
    gpio_init(KEY1,GPI,0,GPI_PULL_UP);
    gpio_init(KEY2,GPI,0,GPI_PULL_UP);
    gpio_init(KEY3,GPI,0,GPI_PULL_UP);
    gpio_init(KEY4,GPI,0,GPI_PULL_UP);
}

/*���뿪��״̬��ȡ*/
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
/*����ɨ��*/
void key_scan(void)
{
     keyup_data=0;         //��̧��󰴼�ֵһ����Ч

     key1_status=gpio_get_level(KEY1);
     key2_status=gpio_get_level(KEY2);
     key3_status=gpio_get_level(KEY3);
     key4_status=gpio_get_level(KEY4);


    if(key1_status==0||key2_status==0||key3_status==0||key4_status==0)   //�м�������
    {
        if(key1_status==0)      key_tem=1;
        else if(key2_status==0) key_tem=2;
        else if(key3_status==0) key_tem=3;
        else if(key4_status==0) key_tem=4;

        if (key_tem == key_bak)      //�м����º��һ��ɨ�費������else��ϵڶ���ɨ����Ч������ʵ����ȥ����
        {
           key_time++;             //�м����º�ִ��һ��ɨ�躯�����ñ�����1
           keydown_data=key_tem;   //����ֵ����keydown_data

//           if( (mode==0)&&(key_time>1) )//key_time>1����ֵ��Ч������ǵ��������modeΪ1��Ϊ����
//            keydown_data=0;
        }
        else                             //ȥ����
        {
           key_time=0;
           key_bak=key_tem;
        }

    }
    else       //��̧��
    {
        if(key_time>2)            //����̧��󷵻�һ�ΰ���ֵ
        {
            keyup_data=key_tem;  //��̧��󰴼�ֵ����keyupata
        }
        key_bak=0;               //Ҫ���㣬��Ȼ�´�ִ��ɨ�����ʱ������ֵ���ϴΰ���ֵһ������û��ȥ����������
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
