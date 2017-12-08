#include "tim.h"
#include "can.h"
#include "pid.h"
#include "gpio.h"
#include "dbus.h"
#include "i2c.h"
#include "ramp.h"
#include "mpu6050.h"
#include "stm32f4xx.h"
#include "control.h"
#include "encoder.h"
void Bsp_Tim_Init(void)
{
				TIM_TimeBaseInitTypeDef      tim;
				TIM_OCInitTypeDef oc;
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM2, ENABLE);
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
//1ms的控制计时
				TIM_TimeBaseStructInit(&tim);
				tim.TIM_ClockDivision = TIM_CKD_DIV1;
				tim.TIM_CounterMode = TIM_CounterMode_Up;
				tim.TIM_Period = 1000;
				tim.TIM_Prescaler = 84-1;
				TIM_TimeBaseInit(TIM6,&tim);
						
				tim.TIM_Period = 0xFFFFFFFF;
				tim.TIM_Prescaler = 84 - 1;	 //1M 的时钟  
				tim.TIM_ClockDivision = TIM_CKD_DIV1;
				tim.TIM_CounterMode = TIM_CounterMode_Up;
				TIM_ARRPreloadConfig(TIM2, ENABLE);
				TIM_TimeBaseInit(TIM2, &tim);
				TIM_Cmd(TIM2, ENABLE);
							
//摩擦轮        
        TIM_TimeBaseStructInit(&tim);
        tim.TIM_ClockDivision=TIM_CKD_DIV1;
        tim.TIM_CounterMode=TIM_CounterMode_Up;
        tim.TIM_Period=2499;
        tim.TIM_Prescaler=84-1;
        TIM_TimeBaseInit(TIM5,&tim);
				
        tim.TIM_ClockDivision=TIM_CKD_DIV1;//拨弹电机
        tim.TIM_CounterMode=TIM_CounterMode_Up;
        tim.TIM_Period=1000;
        tim.TIM_Prescaler=168-1;
        TIM_TimeBaseInit(TIM9,&tim);
				
        oc.TIM_OCMode=TIM_OCMode_PWM2;
        oc.TIM_OCPolarity=TIM_OCPolarity_Low;
        oc.TIM_OutputState=TIM_OutputState_Enable;
        oc.TIM_Pulse=1000;
				oc.TIM_OutputNState = TIM_OutputState_Disable;
				oc.TIM_OCNPolarity = TIM_OCPolarity_High;
				oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
				oc.TIM_OCNIdleState = TIM_OCIdleState_Set;
				
        TIM_OC2Init(TIM5,&oc);
        TIM_OC1Init(TIM5,&oc);
				oc.TIM_Pulse=0;
				TIM_OC1Init(TIM9,&oc);
        TIM_OC2PreloadConfig(TIM5,TIM_OCPreload_Enable);
        TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
				TIM_OC1PreloadConfig(TIM9,TIM_OCPreload_Enable);
				
        TIM_ARRPreloadConfig(TIM5,ENABLE);
        TIM_ARRPreloadConfig(TIM9,ENABLE);
				
        TIM_Cmd(TIM5,ENABLE);
				TIM_Cmd(TIM9,ENABLE);
        TIM5->CCR1 = 1000;
        TIM5->CCR2 = 1000;


}
void Pwm_Enable(void)
{
				TIM_Cmd(TIM6, ENABLE);
				TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
				TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}


void TIM6_DAC_IRQHandler(void)//控制任务
{
	
				if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
				{
				
						TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
						TIM_ClearFlag(TIM6, TIM_FLAG_Update);
					  control_loop();
						
				}
}

uint32_t Get_Time_Micros(void)//读取陀螺仪积分时间
{
				return TIM2->CNT;
}
