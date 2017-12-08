#include "gpio.h"
#include "stm32f4xx.h"



void Bsp_GPIO_Init(void)
{
	GPIO_InitTypeDef						gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC| RCC_AHB1Periph_GPIOB |RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD, ENABLE);

	//led1&led2
	gpio.GPIO_Mode							= GPIO_Mode_OUT;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin							= GPIO_Pin_1| GPIO_Pin_2;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &gpio);

	//usart1(DBus遥控)
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_7;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	//usart3
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_11;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	//can1
	gpio.GPIO_Mode							= GPIO_Mode_AF;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin							= GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &gpio);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

	//can2
	gpio.GPIO_Mode							= GPIO_Mode_AF;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin							= GPIO_Pin_5 | GPIO_Pin_6;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);
	//电机pwm
	gpio.GPIO_Pin = GPIO_Pin_3;//clk
	gpio.GPIO_Mode = GPIO_Mode_AF;//复用
	gpio.GPIO_Speed = GPIO_Speed_100MHz; //速度50MHz 
	gpio.GPIO_OType = GPIO_OType_PP; //复用推挽输出
	gpio.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA,&gpio);//初始化PA3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); //
 
	gpio.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PA.8 端口配置
	gpio.GPIO_Mode = GPIO_Mode_OUT;//复用 	
  gpio.GPIO_OType = GPIO_OType_PP; 	//推挽输出
  gpio.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &gpio);					 //根据设定参数初始化GPIOA.8
  GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
  gpio.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PA.8 端口配置
	gpio.GPIO_Mode = GPIO_Mode_OUT;//复用 	
  gpio.GPIO_OType = GPIO_OType_PP; 	//推挽输出
  gpio.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOC, &gpio);					 //根据设定参数初始化GPIOA.8
  GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	
	
	
//	//IIC
//	//SCL
//	gpio.GPIO_Mode = GPIO_Mode_OUT;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Pin = GPIO_Pin_8;
//	gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
//	gpio.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOB, &gpio);
//	//SDA
//	gpio.GPIO_Mode = GPIO_Mode_OUT;
//	gpio.GPIO_OType = GPIO_OType_OD;
//	gpio.GPIO_Pin = GPIO_Pin_9;
//	gpio.GPIO_PuPd = GPIO_PuPd_UP;
//	gpio.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOB, &gpio);
//        //摩擦轮0，1和拨弹电机2
//  gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOA, &gpio);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM9);
	//激光
}





