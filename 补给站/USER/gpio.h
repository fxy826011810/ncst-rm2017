#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f10x.h"
#define	LED_HEAT() GPIOC->ODR^=GPIO_Pin_1|GPIO_Pin_2

typedef struct key_t
{
	uint8_t time;
	uint8_t status;
}key_t;

extern key_t key_l,key_r;

void Bsp_GPIO_Init(void);
void Key_Scan(void);

#endif
