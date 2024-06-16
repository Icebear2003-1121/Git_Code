/*
 * Gyro.c
 *
 *  Created on: 2023��5��13��
 *      Author: ����a
 */

#include "Gyro.h"

float X,Y,Z;
float acc_x,acc_y,acc_z;
float gyro_x,gyro_y,gyro_z;
float Q0=1,Q1=0,Q2=0,Q3=0;//��¼��Ԫ�� ��ʼ��1 0 0 0
float I_ex, I_ey, I_ez;  // ������
float icm_kp= 0.17;    // ���ٶȼƵ��������ʱ�������
float icm_ki= 0.004;   // �������������ʵĻ�������
float pitch, roll,yaw;
//float a,b,c;
uint8 GyroOn=0;//�Ƿ����
uint8 GyroINT=1;

void gyroOffsetInit(void)
{
    uint8 i;
    X = 0;
    Y = 0;
    Z = 0;
    for (i = 0; i < 100; ++i)
    {
        imu660ra_get_gyro();// ��ȡ�����ǽ��ٶ�
        X += imu660ra_gyro_x;
        Y += imu660ra_gyro_y;
        Z += imu660ra_gyro_z;
        //delay_ms(5);    // ��� 1Khz
        system_delay_ms(5);
    }

    X /= 100;
    Y /= 100;
    Z /= 100;
}

float myRsqrt(float num)
{
    float halfx = 0.5f * num;
    float y = num;
    long i = *(long*)&y;
    i = 0x5f375a86 - (i >> 1);

    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));

    return y;
}

void ImuGetValues(void)
{
    float alpha = 0.3;

    //һ�׵�ͨ�˲�����λg
    acc_x = (((float) imu660ra_acc_x) * alpha) / 4096 + acc_x * (1 - alpha);
    acc_y = (((float) imu660ra_acc_y) * alpha) / 4096 + acc_y * (1 - alpha);
    acc_z = (((float) imu660ra_acc_z) * alpha) / 4096 + acc_z * (1 - alpha);

    //! �����ǽ��ٶȱ���ת��Ϊ�����ƽ��ٶ�: deg/s -> rad/s
    gyro_x = ((float) imu660ra_gyro_x - X) * PI / 180 / 16.4f;
    gyro_y = ((float) imu660ra_gyro_y - Y) * PI / 180 / 16.4f;
    gyro_z = ((float) imu660ra_gyro_z - Z) * PI / 180 / 16.4f;
}

void ImuAHRSupdate(void)
{
    float halfT = 0.5 * delta_T;    // ��������һ��
    float vx, vy, vz;               // ��ǰ��̬��������������������ϵķ���
    float ex, ey, ez;               // ��ǰ���ټƲ�õ��������ٶ��������ϵķ������õ�ǰ��̬��������������������ϵķ��������

    float q0 = Q0;  //��Ԫ��
    float q1 = Q1;
    float q2 = Q2;
    float q3 = Q3;

    float q0q0 = q0 * q0;  //����ˣ������������
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;
    float norm;
    // ������ֹ״̬Ϊ-g ����������
//    if(acc_x * acc_y * acc_z == 0) // �Ӽƴ�����������״̬ʱ(��ʱg = 0)��������̬���㣬��Ϊ�������ĸ���������
//        return;

//      if(acc_y<0.01)return;

    // �Լ��ٶ����ݽ��й�һ�� �õ���λ���ٶ� (a^b -> ��������ϵ�µļ��ٶ�)
    norm = myRsqrt(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);
    acc_x = acc_x * norm;
    acc_y = acc_y * norm;
    acc_z = acc_z * norm;

    // ��������ϵ���������������ϵķ���
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    // g^b �� a^b ��������ˣ��õ������ǵ�У����������e��ϵ��
    ex = acc_y * vz - acc_z * vy;
    ey = acc_z * vx - acc_x * vz;
    ez = acc_x * vy - acc_y * vx;

    // ����ۼ�
    I_ex += halfT * ex;
    I_ey += halfT * ey;
    I_ez += halfT * ez;
//      if(I_ey>0.1)I_ey=0;
//      if(I_ex>0.01)I_ex=0;


    // ʹ��PI�������������������(������Ư�����)
    gyro_x = gyro_x + icm_kp* ex + icm_ki* I_ex;
    gyro_y = gyro_y + icm_kp* ey + icm_ki* I_ey;
    gyro_z = gyro_z + icm_kp* ez + icm_ki* I_ez;

    // һ����������������Ԫ��΢�ַ��̣�����halfTΪ�������ڵ�1/2��gx gy gzΪbϵ�����ǽ��ٶȡ�
    q0 = q0 + (-q1 * gyro_x - q2 * gyro_y - q3 * gyro_z) * halfT;
    q1 = q1 + (q0 * gyro_x + q2 * gyro_z - q3 * gyro_y) * halfT;
    q2 = q2 + (q0 * gyro_y - q1 * gyro_z + q3 * gyro_x) * halfT;
    q3 = q3 + (q0 * gyro_z + q1 * gyro_y - q2 * gyro_x) * halfT;

    // ��λ����Ԫ���ڿռ���תʱ�������죬������ת�Ƕȣ������㷨�������Դ�����������任
    norm = myRsqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    Q0 = q0 * norm;
    Q1 = q1 * norm;
    Q2 = q2 * norm;
    Q3 = q3 * norm;  // ��ȫ�ֱ�����¼��һ�μ������Ԫ��ֵ
}

void ReadGyro(void)
{
    // atan2�������������������ԭ��������X�������μнǵĻ���ֵ
    pitch = asin(2 * Q0 * Q2 - 2 * Q1 * Q3) * 180 / PI;
    roll = atan2(2 * Q2 * Q3 + 2 * Q0 * Q1, -2 * Q1 * Q1 - 2 * Q2 * Q2 + 1) * 180 / PI;
    yaw = atan2(2 * Q1 * Q2 + 2 * Q0 * Q3, -2 * Q2 * Q2 - 2 * Q3 * Q3 + 1) * 180 / PI;
//    oled_show_float(105,2,yaw,2,3);

}

void get_gyro(void)
{
    imu660ra_get_gyro();

    if(imu660ra_gyro_x>2500)imu660ra_gyro_x=2500;
    else if(imu660ra_gyro_x<-2500)imu660ra_gyro_x=-2500;

    if(imu660ra_gyro_y>2500)imu660ra_gyro_y=2500;
    else if(imu660ra_gyro_y<-2500)imu660ra_gyro_y=-2500;

    if(imu660ra_gyro_z>2500)imu660ra_gyro_z=2500;
    else if(imu660ra_gyro_z<-2500)imu660ra_gyro_z=-2500;

}

void ClearGyro(void)
{
    Q0=1;
    Q1=0;
    Q2=0;
    Q3=0;
    I_ex=0;
    I_ey=0;
    I_ez=0;
    yaw=0;
}

void GyroResolve(void)
{
    if(GyroINT)
    {
        get_gyro();

        if(GyroOn)
        {
            ImuGetValues();
            ImuAHRSupdate();
            ReadGyro();
        }
        GyroINT=0;
    }

}



