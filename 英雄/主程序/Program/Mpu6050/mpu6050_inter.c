#include "mpu6050_inter.h"
#include "stm32f4xx.h"
#include "mpu6050.h"
void mpu6500_inter_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  EXTI_InitTypeDef exti;
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
  exti.EXTI_Line = EXTI_Line1;
  exti.EXTI_LineCmd = ENABLE;
  exti.EXTI_Mode = EXTI_Mode_Interrupt;
  exti.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&exti);
}


void EXTI1_IRQHandler(void)
{
	
	if (EXTI_GetITStatus(EXTI_Line1) != 0)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		EXTI_ClearFlag(EXTI_Line1);
		isMPU6500_is_DRY = 1;
		GetPitchYawGxGyGz();
		
	}
}
