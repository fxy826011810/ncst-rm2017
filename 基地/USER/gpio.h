#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f10x.h"
#define	LED_HEAT() GPIOA->ODR^=GPIO_Pin_7|GPIO_Pin_8

typedef struct gpio_init_t
{
	uint32_t rcc;//时钟
	GPIO_TypeDef* gpio;//GPIO
	uint16_t pin;//引脚
	GPIOSpeed_TypeDef speed;
  GPIOMode_TypeDef mode;
}gpio_init_t;

typedef struct hw
{
	uint8_t time;//次数
	uint8_t status;//状态
	void (*scan)(struct gpio_init_t *oe,struct hw *hw);//扫描函数
	uint8_t (*readinputdatabit)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//读取函数
}hw;
extern hw hw1,hw2,hw3,hw4,hw5,hw6,hw7,hw8;
void Bsp_GPIO_Init(void);
void whole_Scan(void);

#endif
