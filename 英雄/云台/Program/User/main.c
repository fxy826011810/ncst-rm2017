#include "stm32f4xx.h"
#include "delay.h"
#include "gpio.h"
#include "nvic.h"
#include "can.h"
#include "mpu6050.h"
#include "mpu6050_inter.h"
#include "can.h"
#include "tim.h"
//主控制函数在tim函数下中断里的control_loop（）；

void system_init(void)
{	
	delay_ms(1000);
  Bsp_NVIC_Init();
	Bsp_GPIO_Init();
	Bsp_Can_Init();
	mpu6050_init();
	delay_ms(1000);
	mpu6050_inter_init();
	Bsp_Tim_Init();
	
}


int main(void)
{
	
  system_init();//系统初始化
  
	while (1)
	{		
		IMU_getValues();
  }
  
}
