#include "gpio.h"
#include "stm32f4xx.h"



void Bsp_GPIO_Init(void)
{
	GPIO_InitTypeDef						gpio;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI, ENABLE);

	//led1&led2
	gpio.GPIO_Mode							= GPIO_Mode_OUT;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin								= GPIO_Pin_14;
	gpio.GPIO_PuPd							= GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &gpio);
	gpio.GPIO_Mode							= GPIO_Mode_OUT;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin								= GPIO_Pin_7;
	gpio.GPIO_PuPd							= GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &gpio);
	GPIO_SetBits(GPIOF,GPIO_Pin_7);
	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
	//摩擦轮（小）
	gpio.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_6;//PWM输出
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOH,&gpio);
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource9,GPIO_AF_TIM12); 
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource6,GPIO_AF_TIM12);
	
	//摩擦轮（大）
	gpio.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PWM输出
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOH,&gpio);
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource10,GPIO_AF_TIM5); 
	GPIO_PinAFConfig(GPIOH,GPIO_PinSource11,GPIO_AF_TIM5);
	
	//usart1(DBus遥控)
	gpio.GPIO_Mode 							= GPIO_Mode_AF;
	gpio.GPIO_OType 						= GPIO_OType_PP;
	gpio.GPIO_Pin 							= GPIO_Pin_7;
	gpio.GPIO_PuPd 							= GPIO_PuPd_UP;
	gpio.GPIO_Speed 						= GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	//usart3//(裁判系统)
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &gpio);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	
	//can1//（连底盘云台电机）
	gpio.GPIO_Mode							= GPIO_Mode_AF;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin								= GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &gpio);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

	//can2
	gpio.GPIO_Mode							= GPIO_Mode_AF;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin								= GPIO_Pin_12 | GPIO_Pin_13;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &gpio);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
	
//SPI(陀螺仪)
//片选
	gpio.GPIO_Mode							= GPIO_Mode_OUT;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin								= GPIO_Pin_6;
	gpio.GPIO_PuPd							= GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed							= GPIO_High_Speed;
	GPIO_Init(GPIOF, &gpio);
	GPIO_SetBits(GPIOF,GPIO_Pin_6);
	
  gpio.GPIO_Mode							= GPIO_Mode_AF;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin								= GPIO_Pin_7 | GPIO_Pin_8	|	GPIO_Pin_9;
	gpio.GPIO_PuPd							= GPIO_PuPd_UP;
	gpio.GPIO_Speed							= GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &gpio);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_SPI5);
	
	//陀螺仪中断
  gpio.GPIO_Mode=GPIO_Mode_IN;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_Pin = GPIO_Pin_1;
  gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
  gpio.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOE,&gpio);
	
	//磁力计
  gpio.GPIO_Mode							= GPIO_Mode_OUT;
	gpio.GPIO_OType							= GPIO_OType_PP;
	gpio.GPIO_Pin								= GPIO_Pin_2|GPIO_Pin_3;
	gpio.GPIO_PuPd							= GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed							= GPIO_High_Speed;
	GPIO_Init(GPIOE, &gpio);
	GPIO_SetBits(GPIOE,GPIO_Pin_2);

//	//激光
//	gpio.GPIO_Pin = GPIO_Pin_13;
//	gpio.GPIO_Mode = GPIO_Mode_OUT;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOG, &gpio);
//	GPIO_ResetBits(GPIOG, GPIO_Pin_13);
	
//拨弹舵机
	gpio.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_2;//PWM输出
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOI,&gpio);
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource7,GPIO_AF_TIM8); //
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource2,GPIO_AF_TIM8);
//机械臂
//1路pwm
	gpio.GPIO_Pin = GPIO_Pin_0;//PWM输出
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&gpio);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); //
 
	gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;//普通引脚				
	gpio.GPIO_Mode = GPIO_Mode_OUT;
  gpio.GPIO_OType = GPIO_OType_PP; 
  gpio.GPIO_Speed = GPIO_Speed_50MHz;		 
  GPIO_Init(GPIOA, &gpio);					 
  GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	
//2路pwm	
	gpio.GPIO_Pin = GPIO_Pin_3;//PWM输出
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz; 
	gpio.GPIO_OType = GPIO_OType_PP; 
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&gpio);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); 
 
	gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;//普通引脚
	gpio.GPIO_Mode = GPIO_Mode_OUT;
  gpio.GPIO_OType = GPIO_OType_PP; 
  gpio.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOI, &gpio);					
  GPIO_SetBits(GPIOI,GPIO_Pin_5);
	GPIO_ResetBits(GPIOI,GPIO_Pin_6);
	
//3路pwm	
	gpio.GPIO_Pin = GPIO_Pin_12;//PWM输出
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz; 
	gpio.GPIO_OType = GPIO_OType_PP; 
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&gpio);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); 
 
	gpio.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//普通引脚
	gpio.GPIO_Mode = GPIO_Mode_OUT;
  gpio.GPIO_OType = GPIO_OType_PP; 
  gpio.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOD, &gpio);					
  GPIO_SetBits(GPIOD,GPIO_Pin_13);
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);

	//6个限位
	gpio.GPIO_Pin = GPIO_Pin_9;//普通引脚
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOI, &gpio);					
	
	gpio.GPIO_Pin = GPIO_Pin_4;//普通引脚
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &gpio);					
	
	gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;//普通引脚
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;	
  GPIO_Init(GPIOC, &gpio);					

	gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;//普通引脚
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &gpio);
}




