#include "mpu6050_inter.h"
#include "stm32f4xx.h"
#include "mpu6050.h"
void mpu6050_inter_init(void)
{
  EXTI_InitTypeDef exti;

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
  exti.EXTI_Line = EXTI_Line4;
  exti.EXTI_LineCmd = ENABLE;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_Init(&exti); 
}

extern uint8_t isMPU6050_is_DRY;
void EXTI4_IRQHandler(void)
{
	
	if (EXTI_GetITStatus(EXTI_Line4) != 0)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		EXTI_ClearFlag(EXTI_Line4);
		isMPU6050_is_DRY = 1;
	}
}
