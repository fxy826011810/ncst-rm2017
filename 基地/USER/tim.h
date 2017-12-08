#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

void Bsp_Tim_Init(void);
uint32_t Get_Time_Micros(void);
void Pwm_Enable(void);


#endif

