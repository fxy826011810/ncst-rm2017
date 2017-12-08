#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "nvic.h"
#include "can.h"
#include "pid.h"
#include "tim.h"
#include "dma.h"
#include "ramp.h"
#include "stdio.h"
#include "control.h"

//本次实现触发一次加一次弹
//更改8000角度为360°角度（PID没调！！！）
//未来实现中途退出后直接带弹反向转回





void system_init(void)
{
	DJ_status=_24v;//定义电机的电压值
	ramp_config();
  Bsp_Pid_Init();
  Bsp_NVIC_Init();
	Bsp_GPIO_Init();
	Bsp_Pid_Init();
	Bsp_Can_Init();
	Bsp_Tim_Init();
}
extern uint8_t tim_flag;
int main(void)
{
	
  system_init();//系统初始化
  
	while (1)
	{		
		if(tim_flag)
		{
			control_loop();
			tim_flag=0;
		}
  }
  
}
