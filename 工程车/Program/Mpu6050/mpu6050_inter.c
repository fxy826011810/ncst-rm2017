#include "mpu6050_inter.h"
#include "stm32f4xx.h"
#include "mpu6050.h"
void mpu6050_inter_init(void)
{
  GPIO_InitTypeDef gpio;
  NVIC_InitTypeDef nvic;
  EXTI_InitTypeDef exti;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
  gpio.GPIO_Mode=GPIO_Mode_IN;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_Pin = GPIO_Pin_4;
  gpio.GPIO_PuPd = GPIO_PuPd_UP;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&gpio);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
  exti.EXTI_Line = EXTI_Line4;
  exti.EXTI_LineCmd = ENABLE;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_Init(&exti);

  nvic.NVIC_IRQChannel = EXTI4_IRQn;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  nvic.NVIC_IRQChannelPreemptionPriority = 2;
  nvic.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&nvic);
}
extern float angle[3];
extern uint8_t isMPU6050_is_DRY;
void EXTI4_IRQHandler(void)
{
	
	if (EXTI_GetITStatus(EXTI_Line4) != 0)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		EXTI_ClearFlag(EXTI_Line4);
		isMPU6050_is_DRY = 1;
                GetPitchYawGxGyGz();
		
	}
}
