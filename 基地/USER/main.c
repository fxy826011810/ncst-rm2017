#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "nvic.h"
#include "can.h"
#include "pid.h"
#include "tim.h"
#include "dma.h"
#include "usart.h"
#include "ramp.h"
#include "stdio.h"
#include "control.h"

//红外任务写在HWControlLoop内，前后运动变量cmy，左右运动变量cmx，转圈运动变量cmr


void system_init(void)
{
	delay_ms(1000);
	ramp_config();
  Bsp_Pid_Init();
  Bsp_NVIC_Init();
	Bsp_GPIO_Init();
	Bsp_Usart_Init();
	Bsp_DMA_Init();
	Bsp_Pid_Init();
	Bsp_Can_Init();
	Bsp_Tim_Init();
}


int main(void)
{
	
  system_init();//系统初始化
  
	while (1)
	{		
			if(System_mode==normal)
			{
		HWControlLoop();//红外控制函数
			}
  }
  
}
