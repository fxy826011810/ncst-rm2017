#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f10x.h"

#define init_time_ms 6000//��ʼ��б��
#define MaxWheelSpeed 50
#define MaxWheelPower 75
//#define speed 15

typedef enum System_mode_t
{
	prepare=2,//ϵͳ����״̬
	checkon=0,//������翪�غ�
  normal=1,//ƽʱ״̬
	stop=3,//������ը����ϵͳ���ֹ���
}System_mode_t;//����ģʽ

extern System_mode_t System_mode;

void HWControlLoop(void);
void CMControlLoop(void);
void Control_Loop(void);
#endif
