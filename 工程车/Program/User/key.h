#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f4xx.h"
#define S2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define S3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
typedef struct{
	volatile u8 PressedDowTimes;//按键被按的次数
	volatile u8 CheckLowTimes;//检测到低电平的次数
}Switch;

extern Switch switch2,switch3;



void KeyInit(void);//按键相关IO初始化

u8 S2_Scan(void);
u8 S3_Scan(void);
#endif