#include "tim.h"
#include "can.h"
#include "gpio.h"
#include "control.h"
#include "stm32f4xx.h"
void Bsp_Tim_Init(void)
{
	TIM_TimeBaseInitTypeDef      tim;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseStructInit(&tim);
	tim.TIM_ClockDivision 	= TIM_CKD_DIV1;
	tim.TIM_CounterMode 		= TIM_CounterMode_Up;
	tim.TIM_Period 					= 1000;
	tim.TIM_Prescaler 			= 84-1;
	TIM_TimeBaseInit(TIM6,&tim);
	TIM_Cmd(TIM6, ENABLE);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}



void TIM6_DAC_IRQHandler(void)//¿ØÖÆÈÎÎñ
{
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		TIM_ClearFlag(TIM6, TIM_FLAG_Update);
		control_loop();
	}
}



