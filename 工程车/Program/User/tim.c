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
			//1ms的控制计时
				TIM_TimeBaseStructInit(&tim);
				tim.TIM_ClockDivision = TIM_CKD_DIV1;
				tim.TIM_CounterMode = TIM_CounterMode_Up;
				tim.TIM_Period = 1000-1;
				tim.TIM_Prescaler = 84-1;
				TIM_TimeBaseInit(TIM6,&tim);
				TIM_Cmd(TIM6, ENABLE);
				TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
				TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
				tim.TIM_Prescaler=84-1;//定时器分频
				tim.TIM_CounterMode=TIM_CounterMode_Up; //向上计数
				tim.TIM_Period=500-1;//自动重装载值
				tim.TIM_ClockDivision=TIM_CKD_DIV1; 
				TIM_TimeBaseInit(TIM2,&tim);//初始化定时器 
				oc.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式
				oc.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
				oc.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性低 
				oc.TIM_Pulse=0;
				TIM_OC4Init(TIM2, &oc);//初始化外设TIM1 4OC1 
				TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能与装载寄存器
				TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能
				TIM_Cmd(TIM2, ENABLE);//使能TIM14
}

				
				
				
void Timer3Init(u32 Period)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 84; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到N*10为Nms
	TIM_TimeBaseStructure.TIM_Prescaler =(1000-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	/*配置中断优先级*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}


void TIM3_IRQHandler(void)   //TIM3中断服务程序
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否 
	{
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断标志位
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


//===========================================原为摩擦轮拨弹电机初始化可改为步进电机======================================        
//        TIM_TimeBaseStructInit(&tim);
//        tim.TIM_ClockDivision=TIM_CKD_DIV1;
//        tim.TIM_CounterMode=TIM_CounterMode_Up;
//        tim.TIM_Period=2499;
//        tim.TIM_Prescaler=84-1;
//        TIM_TimeBaseInit(TIM5,&tim);
//				
//        tim.TIM_ClockDivision=TIM_CKD_DIV1;//拨弹电机
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



void TIM6_DAC_IRQHandler(void)//控制任务
{
	
				if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
				{
				
						TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
						TIM_ClearFlag(TIM6, TIM_FLAG_Update);
					  control_loop();//主控制函数
						
				}
}

