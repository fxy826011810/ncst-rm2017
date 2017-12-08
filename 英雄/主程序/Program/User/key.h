#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f4xx.h"

#define GPIOsw1 GPIOI
#define GPIO_Pin_sw1 GPIO_Pin_9

#define GPIOsw2 GPIOA
#define GPIO_Pin_sw2 GPIO_Pin_4

#define GPIOsw3 GPIOC
#define GPIO_Pin_sw3 GPIO_Pin_1

#define GPIOsw4 GPIOC
#define GPIO_Pin_sw4 GPIO_Pin_0

#define GPIOsw5 GPIOB
#define GPIO_Pin_sw5 GPIO_Pin_1

#define GPIOsw6 GPIOB
#define GPIO_Pin_sw6 GPIO_Pin_0

typedef struct
{
	GPIO_TypeDef* gpio;//GPIO
	uint16_t pin;//引脚
	uint8_t time;//次数
	uint8_t status;//状态
	uint8_t lock;//状态
}Switch;

typedef struct Stepper_t
{
	int16_t ch;//遥控器通道值保存
	uint8_t lock;//锁遥控标志位
	uint8_t check;//判断是不是已经触发开关
}Stepper_t;

typedef enum SW_Status_t
{
	SW1_ON=0,
	SW2_ON=1,
	SW3_ON=2,
	SW4_ON=3,
	SW5_ON=4,
	SW6_ON=5,
	SW_ALL_OFF=6,
}SW_Status_t;
extern SW_Status_t SW1_Status,SW2_Status,SW3_Status;
extern Switch sw1,sw2,sw3,sw4,sw5,sw6,sw7,sw8;
extern Stepper_t stp1up,stp1down,stp2up,stp2down,stp3up,stp3down;
void Switch_Scan(void);
void MechArm_Switch(SW_Status_t sw1_status,SW_Status_t sw2_status,SW_Status_t sw3_status);
#endif