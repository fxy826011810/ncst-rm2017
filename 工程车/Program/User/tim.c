#include "tim.h"
#include "can.h"
#include "pid.h"
#include "gpio.h"
#include "dbus.h"
#include "ramp.h"
#include "stm32f4xx.h"
#include "control.h"
#include "key.h"
void Bsp_Tim_Init(void)
{
				TIM_TimeBaseInitTypeDef      tim;
				TIM_OCInitTypeDef oc;
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM2,ENABLE);
			//1ms�Ŀ��Ƽ�ʱ
				TIM_TimeBaseStructInit(&tim);
				tim.TIM_ClockDivision = TIM_CKD_DIV1;
				tim.TIM_CounterMode = TIM_CounterMode_Up;
				tim.TIM_Period = 1000-1;
				tim.TIM_Prescaler = 84-1;
				TIM_TimeBaseInit(TIM6,&tim);
				TIM_Cmd(TIM6, ENABLE);
				TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
				TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
				tim.TIM_Prescaler=84-1;//��ʱ����Ƶ
				tim.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���
				tim.TIM_Period=500-1;//�Զ���װ��ֵ
				tim.TIM_ClockDivision=TIM_CKD_DIV1; 
				TIM_TimeBaseInit(TIM2,&tim);//��ʼ����ʱ�� 
				oc.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ
				oc.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
				oc.TIM_OCPolarity = TIM_OCPolarity_Low; //������Ե� 
				oc.TIM_Pulse=0;
				TIM_OC4Init(TIM2, &oc);//��ʼ������TIM1 4OC1 
				TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ����װ�ؼĴ���
				TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ��
				TIM_Cmd(TIM2, ENABLE);//ʹ��TIM14
}

				
				
				
void Timer3Init(u32 Period)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 84; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������N*10ΪNms
	TIM_TimeBaseStructure.TIM_Prescaler =(1000-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	/*�����ж����ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}


void TIM3_IRQHandler(void)   //TIM3�жϷ������
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������ 
	{
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϱ�־λ
			if(S2==0)
		{
			if(switch2.CheckLowTimes==4)
				switch2.PressedDowTimes =1;
			else if(switch2.CheckLowTimes<4)
				switch2.CheckLowTimes+=1;
		}
		else
		{
		switch2.CheckLowTimes=0;
		switch2.PressedDowTimes =0;
		}
		if(S3==0)
		{
			if(switch3.CheckLowTimes==4)
			
				switch3.PressedDowTimes =1;
			else if(switch3.CheckLowTimes<4)
				switch3.CheckLowTimes+=1;
		}
		else
		{
		switch3.CheckLowTimes=0;
		switch3.PressedDowTimes =0;
		}
   }	


//===========================================ԭΪĦ���ֲ��������ʼ���ɸ�Ϊ�������======================================        
//        TIM_TimeBaseStructInit(&tim);
//        tim.TIM_ClockDivision=TIM_CKD_DIV1;
//        tim.TIM_CounterMode=TIM_CounterMode_Up;
//        tim.TIM_Period=2499;
//        tim.TIM_Prescaler=84-1;
//        TIM_TimeBaseInit(TIM5,&tim);
//				
//        tim.TIM_ClockDivision=TIM_CKD_DIV1;//�������
//        tim.TIM_CounterMode=TIM_CounterMode_Up;
//        tim.TIM_Period=2000;
//        tim.TIM_Prescaler=168-1;
//        TIM_TimeBaseInit(TIM9,&tim);
//				
//        oc.TIM_OCMode=TIM_OCMode_PWM2;
//        oc.TIM_OCPolarity=TIM_OCPolarity_Low;
//        oc.TIM_OutputState=TIM_OutputState_Enable;
//        oc.TIM_Pulse=1000;
//				oc.TIM_OutputNState = TIM_OutputState_Disable;
//				oc.TIM_OCNPolarity = TIM_OCPolarity_High;
//				oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
//				oc.TIM_OCNIdleState = TIM_OCIdleState_Set;
//				
//        TIM_OC2Init(TIM5,&oc);
//        TIM_OC1Init(TIM5,&oc);
//				oc.TIM_Pulse=0;
//				TIM_OC1Init(TIM9,&oc);
//        TIM_OC2PreloadConfig(TIM5,TIM_OCPreload_Enable);
//        TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable);
//				TIM_OC1PreloadConfig(TIM9,TIM_OCPreload_Enable);
//				
//        TIM_ARRPreloadConfig(TIM5,ENABLE);
//        TIM_ARRPreloadConfig(TIM9,ENABLE);
//				
//        TIM_Cmd(TIM5,ENABLE);
//				TIM_Cmd(TIM9,ENABLE);
//        TIM5->CCR1 = 1000;
//        TIM5->CCR2 = 1000;


}



void TIM6_DAC_IRQHandler(void)//��������
{
	
				if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
				{
				
						TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
						TIM_ClearFlag(TIM6, TIM_FLAG_Update);
					  control_loop();//�����ƺ���
						
				}
}

