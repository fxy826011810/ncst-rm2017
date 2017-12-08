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



/*
TIM6	-������
TIM7	-������
TIM12	-СĦ����
TIM5	-��Ħ����
TIM2	-˿�ܵ�����ֱۣ��ٶ�һ��
TIM4	-˿�ܵ����צ�ӣ��ɵ���
TIM8	-�������
*/
void Bsp_Tim_Init(void)
{
				TIM_TimeBaseInitTypeDef      tim;
				TIM_OCInitTypeDef oc;
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM7|RCC_APB1Periph_TIM12, ENABLE);
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
//1ms�Ŀ��Ƽ�ʱ
				TIM_TimeBaseStructInit(&tim);
				tim.TIM_ClockDivision = TIM_CKD_DIV1;
				tim.TIM_CounterMode = TIM_CounterMode_Up;
				tim.TIM_Period = 1000-1;
				tim.TIM_Prescaler = 84-1;
				TIM_TimeBaseInit(TIM6,&tim);
//�������ںϻ���ʱ���		
				tim.TIM_Period = 0xFFFFFFFF;
				tim.TIM_Prescaler = 84 - 1;	 //1M ��ʱ��  
				tim.TIM_ClockDivision = TIM_CKD_DIV1;
				tim.TIM_CounterMode = TIM_CounterMode_Up;
				TIM_ARRPreloadConfig(TIM7, ENABLE);
				TIM_TimeBaseInit(TIM7, &tim);
				TIM_Cmd(TIM7, ENABLE);
//Ħ���� ��С����        
        TIM_TimeBaseStructInit(&tim);
        tim.TIM_ClockDivision=TIM_CKD_DIV1;
        tim.TIM_CounterMode=TIM_CounterMode_Up;
        tim.TIM_Period=2499;
        tim.TIM_Prescaler=84-1;
        TIM_TimeBaseInit(TIM12,&tim);
				
        oc.TIM_OCMode=TIM_OCMode_PWM2;
        oc.TIM_OCPolarity=TIM_OCPolarity_Low;
        oc.TIM_OutputState=TIM_OutputState_Enable;
        oc.TIM_Pulse=1000;
				
        TIM_OC2Init(TIM12,&oc);
        TIM_OC1Init(TIM12,&oc);

        TIM_OC2PreloadConfig(TIM12,TIM_OCPreload_Enable);
        TIM_OC1PreloadConfig(TIM12,TIM_OCPreload_Enable);
        TIM_ARRPreloadConfig(TIM12,ENABLE);
				
        TIM_Cmd(TIM12,ENABLE);
        TIM12->CCR1 = 1000;
        TIM12->CCR2 = 1000;
//Ħ���� �������     
        TIM_TimeBaseStructInit(&tim);
        tim.TIM_ClockDivision=TIM_CKD_DIV1;
        tim.TIM_CounterMode=TIM_CounterMode_Up;
        tim.TIM_Period=2499;
        tim.TIM_Prescaler=84-1;
        TIM_TimeBaseInit(TIM5,&tim);
				
        oc.TIM_OCMode=TIM_OCMode_PWM2;
        oc.TIM_OCPolarity=TIM_OCPolarity_Low;
        oc.TIM_OutputState=TIM_OutputState_Enable;
        oc.TIM_Pulse=1000;
				
        TIM_OC2Init(TIM5,&oc);
        TIM_OC1Init(TIM5,&oc);

        TIM_OC2PreloadConfig(TIM5,TIM_OCPreload_Enable);
        TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
        TIM_ARRPreloadConfig(TIM5,ENABLE);
				
        TIM_Cmd(TIM5,ENABLE);
        TIM5->CCR1 = 1000;
        TIM5->CCR2 = 1000;	
//����������𵯶��
				TIM_DeInit(TIM8);
        TIM_TimeBaseStructInit(&tim);
        tim.TIM_ClockDivision=TIM_CKD_DIV1;
        tim.TIM_CounterMode=TIM_CounterMode_Up;
        tim.TIM_Period=2000;
        tim.TIM_Prescaler=1680-1;//20ms
				tim.TIM_RepetitionCounter=0x00;
        TIM_TimeBaseInit(TIM8,&tim);
				
        oc.TIM_OCMode=TIM_OCMode_PWM2;
        oc.TIM_OCPolarity=TIM_OCPolarity_Low;
        oc.TIM_OutputState=TIM_OutputState_Enable;
        oc.TIM_Pulse=50;
				oc.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
				oc.TIM_OCNPolarity=TIM_OCPolarity_Low;
				oc.TIM_OutputNState=TIM_OutputNState_Disable;
				oc.TIM_OCIdleState=TIM_OCIdleState_Reset;
        TIM_OC3Init(TIM8,&oc);
				TIM_OC4Init(TIM8,&oc);
        TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);
				TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable);
        TIM_ARRPreloadConfig(TIM8,ENABLE);
				TIM_CtrlPWMOutputs(TIM8,ENABLE);
        TIM_Cmd(TIM8,ENABLE);
				TIM8->CCR3=50;
				TIM8->CCR4=130;				
//��·pwm��˿�ܵ����
	      TIM_TimeBaseStructInit(&tim);//0.5ms
        tim.TIM_ClockDivision=TIM_CKD_DIV1;
        tim.TIM_CounterMode=TIM_CounterMode_Up;
        tim.TIM_Period=750;
        tim.TIM_Prescaler=84-1;
        TIM_TimeBaseInit(TIM2,&tim);
				TIM_TimeBaseInit(TIM4,&tim);
				
        oc.TIM_OCMode=TIM_OCMode_PWM2;
        oc.TIM_OCPolarity=TIM_OCPolarity_Low;
        oc.TIM_OutputState=TIM_OutputState_Enable;
        oc.TIM_Pulse=0;
				
				TIM_OC1Init(TIM4,&oc);
        TIM_OC1Init(TIM2,&oc);
        TIM_OC4Init(TIM2,&oc);
				
				TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
				TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
				TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
        
        TIM_ARRPreloadConfig(TIM2,ENABLE);
				TIM_ARRPreloadConfig(TIM4,ENABLE);

				TIM_Cmd(TIM2,ENABLE);
				TIM_Cmd(TIM4,ENABLE);
				
}
void Tim_Enable(void)//���������񣨼�����TIM6��ʱ���жϲ�ʹ�ܣ�
{
				TIM_Cmd(TIM6, ENABLE);
				TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
				TIM_ClearFlag(TIM6, TIM_FLAG_Update);
}


void TIM6_DAC_IRQHandler(void)//��������
{
	
				if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
				{
						TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
						TIM_ClearFlag(TIM6, TIM_FLAG_Update);
					  control_loop();//������
				}
}

uint32_t Get_Time_Micros(void)//��ȡ�����ǻ���ʱ��
{
				return TIM7->CNT;
}
