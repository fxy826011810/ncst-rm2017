#include "tim.h"
#include "can.h"
#include "pid.h"
#include "gpio.h"
#include "ramp.h"
#include "stm32f10x.h"
#include "control.h"
#include "encoder.h"

void Bsp_Tim_Init(void)
{
				TIM_TimeBaseInitTypeDef      tim;
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			//1ms�Ŀ��Ƽ�ʱ
				TIM_TimeBaseStructInit(&tim);
				tim.TIM_ClockDivision = TIM_CKD_DIV1;
				tim.TIM_CounterMode = TIM_CounterMode_Up;
				tim.TIM_Period = 1000;
				tim.TIM_Prescaler = 72-1;
				TIM_TimeBaseInit(TIM6,&tim);
				TIM_Cmd(TIM6, ENABLE);
				TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
				TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}

uint8_t tim_flag=0;
void TIM6_IRQHandler(void)//��������
{
	
				if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
				{
					
						TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
						TIM_ClearFlag(TIM6, TIM_FLAG_Update);
						tim_flag=1;
				}
}
