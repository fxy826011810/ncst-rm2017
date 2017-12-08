#ifndef __TIM_H
#define __TIM_H
#include "stm32f4xx.h"


#define PWM1 TIM12->CCR1
#define PWM2 TIM12->CCR2
#define PWM3 TIM5->CCR1
#define PWM4 TIM5->CCR2

#define CMPWM1	TIM2->CCR1
#define CMPWM2	TIM2->CCR4
#define CMPWM3	TIM4->CCR1

//Ä¦²ÁÂÖËÙ¶ÈÉèÖÃ
#define FRISpeed_Set(x,y)  \
	PWM1 = x;       \
	PWM2 = x;    	 	\
	PWM3 = y;				\
	PWM4 = y;	
	
#define POK_ON \
TIM8->CCR3=50 \


#define POK_OFF \
TIM8->CCR3=130\







void Bsp_Tim_Init(void);
uint32_t Get_Time_Micros(void);
void Tim_Enable(void);


#endif

