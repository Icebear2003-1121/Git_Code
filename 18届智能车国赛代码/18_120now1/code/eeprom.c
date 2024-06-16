/*
 * eeprom.c
 *
 *  Created on: 2023��7��14��
 *      Author: ����a
 */
#include "eeprom.h"
bool show_tuxiang;
bool show_canshu;
bool chuankou;
bool show_time;
void Parameter_read_eeprom_canshu(void)
{
     flash_buffer_clear ();
     flash_read_page_to_buffer (0, 0);
     CurPid.Kp = flash_union_buffer[0].float_type;
     CurPid.Ki = flash_union_buffer[1].float_type;
     CurPid.Kd = flash_union_buffer[2].float_type;

     JSDPid.Kp = flash_union_buffer[3].float_type;
     JSDPid.Ki = flash_union_buffer[4].float_type;
     JSDPid.Kd = flash_union_buffer[5].float_type;
}
void Parameter_read_eeprom_canshu_2(void)
{
    flash_buffer_clear ();
    flash_read_page_to_buffer (0, 2);
     speed_base      =flash_union_buffer[0].int8_type;
     flag.barrierDir =flash_union_buffer[1].int8_type;
     flag.image_show =flash_union_buffer[2].int8_type;
     flag.strategy1  =flash_union_buffer[3].int8_type;
     flag.strategy2  =flash_union_buffer[4].int8_type;
     flag.strategy3  =flash_union_buffer[5].int8_type;
     flag.strategy4  =flash_union_buffer[6].int8_type;
     RoadOut_flag    =flash_union_buffer[7].uint8_type;

}
void Parameter_read_eeprom_canshu_3(void)
{
    flash_buffer_clear ();
       flash_read_page_to_buffer (0,4);

       GptPidL.Kp=flash_union_buffer[0].float_type;                        // �򻺳����� 0 ��λ��д�� float  ����
       GptPidL.Ki=flash_union_buffer[1].float_type;                         // �򻺳����� 1 ��λ��д�� float  ����
       GptPidL.Kd=flash_union_buffer[2].float_type;                         // �򻺳����� 2 ��λ��д�� float  ����

       GptPidR.Kp=flash_union_buffer[3].float_type;                         // �򻺳����� 3 ��λ��д�� float  ����
       GptPidR.Ki=flash_union_buffer[4].float_type;                         // �򻺳����� 4 ��λ��д�� float  ����
       GptPidR.Kd=flash_union_buffer[5].float_type;

}



void Parameter_write_eeprom_canshu(void)
{
       flash_erase_page(0,0);                                                    //����ҳ
       flash_buffer_clear ();                                                    // ��ջ�����

       flash_union_buffer[0].float_type = CurPid.Kp;                            // �򻺳����� 0 ��λ��д�� float  ����
       flash_union_buffer[1].float_type = CurPid.Ki;                            // �򻺳����� 1 ��λ��д�� float  ����
       flash_union_buffer[2].float_type = CurPid.Kd;                            // �򻺳����� 2 ��λ��д�� float  ����

       flash_union_buffer[3].float_type = JSDPid.Kp;                         // �򻺳����� 3 ��λ��д�� float  ����
       flash_union_buffer[4].float_type = JSDPid.Ki;                         // �򻺳����� 4 ��λ��д�� float  ����
       flash_union_buffer[5].float_type = JSDPid.Kd;                         // �򻺳����� 5 ��λ��д�� float  ����
       flash_write_page_from_buffer(0, 0);                                       // ��ָ�� Flash ������ҳ��д�뻺��������

}
void Parameter_write_eeprom_canshu_2(void)
{
      flash_erase_page(0,2);                                                    //����ҳ
      flash_buffer_clear ();
       flash_union_buffer[0].int8_type    = speed_base;                             // �򻺳����� 9 ��λ��д�� int16  ����
       flash_union_buffer[1].int16_type  = flag.barrierDir;
       flash_union_buffer[2].int8_type   = flag.image_show;
        flash_union_buffer[3].int8_type  = flag.strategy1;
        flash_union_buffer[4].int8_type  = flag.strategy2;
        flash_union_buffer[5].int8_type  = flag.strategy3;
        flash_union_buffer[6].int8_type  = flag.strategy4;
        flash_union_buffer[7].uint8_type = RoadOut_flag;

        flash_write_page_from_buffer(0, 2);                                       // ��ָ�� Flash ������ҳ��д�뻺��������

}
 void Parameter_write_eeprom_canshu_3(void)
 {
     flash_erase_page(0,4);                                                    //����ҳ
         flash_buffer_clear ();
        flash_union_buffer[0].float_type = GptPidL.Kp;                            // �򻺳����� 0 ��λ��д�� float  ����
        flash_union_buffer[1].float_type = GptPidL.Ki;                            // �򻺳����� 1 ��λ��д�� float  ����
        flash_union_buffer[2].float_type = GptPidL.Kd;                            // �򻺳����� 2 ��λ��д�� float  ����

        flash_union_buffer[3].float_type = GptPidR.Kp;                         // �򻺳����� 3 ��λ��д�� float  ����
        flash_union_buffer[4].float_type = GptPidR.Ki;                         // �򻺳����� 4 ��λ��д�� float  ����
        flash_union_buffer[5].float_type = GptPidR.Kd;
       flash_write_page_from_buffer(0, 4);                                       // ��ָ�� Flash ������ҳ��д�뻺��������
}


void Parameter_read_eeprom_xianshi(void)
{
     flash_buffer_clear();
     flash_read_page_to_buffer (0, 1);

     show_tuxiang = flash_union_buffer[0].uint8_type;
     show_canshu  = flash_union_buffer[1].uint8_type;
     chuankou     = flash_union_buffer[2].uint8_type;
     show_time    = flash_union_buffer[3].uint8_type;
//     Xk.RightOut  = flash_union_buffer[4].uint8_type;
//     Xk.LeftOut   = flash_union_buffer[5].uint8_type;
//     Xk.StrGo     = flash_union_buffer[6].uint8_type;
     flash_write_page_from_buffer(0, 1);
}

void Parameter_write_eeprom_xianshi(void)
{
       flash_erase_page(0,1);
       flash_buffer_clear();

       flash_union_buffer[0].uint8_type = show_tuxiang;
       flash_union_buffer[1].uint8_type = show_canshu;
       flash_union_buffer[2].uint8_type = chuankou;
       flash_union_buffer[3].uint8_type = show_time;
//       flash_union_buffer[4].int16_type = Xk.LeftOut;
//       flash_union_buffer[5].int16_type = Xk.RightOut;
//       flash_union_buffer[6].int16_type = Xk.StrGo;


       flash_write_page_from_buffer(0, 1);
}







