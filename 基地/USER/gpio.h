#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f10x.h"
#define	LED_HEAT() GPIOA->ODR^=GPIO_Pin_7|GPIO_Pin_8

typedef struct gpio_init_t
{
	uint32_t rcc;//ʱ��
	GPIO_TypeDef* gpio;//GPIO
	uint16_t pin;//����
	GPIOSpeed_TypeDef speed;
  GPIOMode_TypeDef mode;
}gpio_init_t;

typedef struct hw
{
	uint8_t time;//����
	uint8_t status;//״̬
	void (*scan)(struct gpio_init_t *oe,struct hw *hw);//ɨ�躯��
	uint8_t (*readinputdatabit)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//��ȡ����
}hw;
extern hw hw1,hw2,hw3,hw4,hw5,hw6,hw7,hw8;
void Bsp_GPIO_Init(void);
void whole_Scan(void);

#endif
