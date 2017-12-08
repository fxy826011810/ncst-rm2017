#include "gpio.h"
#include "stm32f4xx.h"



void Bsp_GPIO_Init(void)
{
	GPIO_InitTypeDef						gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC| RCC_AHB1Periph_GPIOB |RCC_AHB1Periph_GPIOA, ENABLE);

	//led1&led2
	gpio.GPIO_Mode							= GPIO_Mode_OUT;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin							= GPIO_Pin_1| GPIO_Pin_2;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &gpio);

	//can1
	gpio.GPIO_Mode							= GPIO_Mode_AF;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin							= GPIO_Pin_11 | GPIO_Pin_12;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &gpio);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

	//can2
	gpio.GPIO_Mode							= GPIO_Mode_AF;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin							= GPIO_Pin_12 | GPIO_Pin_13;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	//SCL
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_8;
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpio);
	//SDA
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_OD;
	gpio.GPIO_Pin = GPIO_Pin_9;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpio);
	//�������ж�
	gpio.GPIO_Mode=GPIO_Mode_IN;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_Pin = GPIO_Pin_4;
  gpio.GPIO_PuPd = GPIO_PuPd_UP;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&gpio);
	//����
	gpio.GPIO_Pin = GPIO_Pin_8;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &gpio);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}





