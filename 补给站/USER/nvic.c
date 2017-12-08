#include "nvic.h"
#include "stm32f10x.h"
void Bsp_NVIC_Init(void)
{
	NVIC_InitTypeDef											nvic;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	//can1
	nvic.NVIC_IRQChannel									= USB_LP_CAN1_RX0_IRQn;
	nvic.NVIC_IRQChannelCmd								= ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority					= 0;
	nvic.NVIC_IRQChannelSubPriority						= 3;
	NVIC_Init(&nvic);
//	//can2
//	nvic.NVIC_IRQChannel									= USB_LP_CAN2_RX0_IRQHandler;
//	nvic.NVIC_IRQChannelCmd								= ENABLE;
//	nvic.NVIC_IRQChannelPreemptionPriority					= 1;
//	nvic.NVIC_IRQChannelSubPriority						= 1;
//	NVIC_Init(&nvic);

	//tim6
	nvic.NVIC_IRQChannel						= TIM6_IRQn;
	nvic.NVIC_IRQChannelCmd						= ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority		= 1;
	nvic.NVIC_IRQChannelSubPriority				= 0;
	NVIC_Init(&nvic);
//	//exti
//	//tim6
//	nvic.NVIC_IRQChannel						= EXTI1_IRQn;
//	nvic.NVIC_IRQChannelCmd						= ENABLE;
//	nvic.NVIC_IRQChannelPreemptionPriority		= 0;
//	nvic.NVIC_IRQChannelSubPriority				= 0;
//	NVIC_Init(&nvic);
}
