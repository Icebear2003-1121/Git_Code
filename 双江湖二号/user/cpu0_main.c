/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������


// **************************** �������� ****************************
/***********************��������*****************************
 ����ӿ�P33_9

 ����ӿ�
 gpio��  P21_4
 gpio��  P21_2
 pwm��   P21_5
 pwm��   P21_3

 �������ӿ�
 �� P02_8 P00_9
 �� P33_7 P33_6

 XKͼ��
 TX14_0   RX14_1

************************************************************/

void Init(void)
{
    camera_Init();//��ʼ�����
    mt9v03x_init();//��ʼ������ͷ
    mt9v03x_set_exposure_time(ExposureTime);//�����ع�ʱ��
    while(!mt9v03x_finish_flag);
//    oled_init();//��ʼ��OLED��ʾ��
    ips114_init();
    steer_init();
    Pid_Init();
    motor_init();
    gpt_Init();
    key_Init();
    dl1a_init();//��ʼ��TOF������
    gpio_init(BEEP, GPO, 0, GPO_PUSH_PULL);
//    gpt_count_init();
    VOFA_init();
//    XK_init();
}

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    Init();

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����

    while(TRUE)
    {
        gpt_get();
        if(LeftGo)//�����
        {
            Depart(1);
            if(Car)
            break;
        }
        else if(RightGo)//�ҳ���
        {
            Depart(2);
            if(Car)
            break;
        }
        else if(Go)//ֱ�ӷ���
        {
            Car=1;
            break;
        }
        else
        {
            key_scan();//����ɨ��
            Keystroke_Depart();
        }
    }
    pit_init(CCU60_CH0, 5000);//�������
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
                Test_CAMERA();//ͼ����
//        printf("%d,%d,%d,%d\n",gpt[0],gpt[1],gptL_Cir,gptR_Cir);
//        pwm_set_duty(S_MOTOR_PIN,613);
//        motor_control(2000, 2000);
//        VOFA_gpt();
//        oled_show_int(0, 0, 1, 3);//����oled�Ƿ���Ч
//        gpt_get();
//                ips114_show_int(0, 0, gpt[0], 3);//����oled�Ƿ���Ч
//                ips114_show_int(0, 20, gpt[1], 3);//����oled�Ƿ���Ч
//                ips114_show_float(0, 40, gDir_MidFilter*1500, 3,3);
//                ips114_show_int(0, 100, BlackNumAll, 3);
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore


// **************************** �������� ****************************
