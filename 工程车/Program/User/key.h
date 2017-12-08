#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f4xx.h"
#define S2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define S3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
typedef struct{
	volatile u8 PressedDowTimes;//���������Ĵ���
	volatile u8 CheckLowTimes;//��⵽�͵�ƽ�Ĵ���
}Switch;

extern Switch switch2,switch3;



void KeyInit(void);//�������IO��ʼ��

u8 S2_Scan(void);
u8 S3_Scan(void);
#endif