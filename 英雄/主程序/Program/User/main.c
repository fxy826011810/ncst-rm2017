#include "mpu6050_inter.h"
#include "stm32f4xx.h"
#include "mpu6050.h"
#include "usart.h"
#include "delay.h"
#include "gpio.h"
#include "nvic.h"
#include "can.h"
#include "pid.h"
#include "tim.h"
#include "dma.h"
#include "spi.h" 
#include "calc.h" 
#include "ramp.h"
#include <stdio.h>
#include "control.h"
#include "encoder.h"

//因为加了t型板所以取消了所有的信仰版陀螺仪
void system_init(void)
{
	delay_ms(1000);
	ramp_config();
  Bsp_Pid_Init();
  Bsp_NVIC_Init();
	Bsp_GPIO_Init();
	//Bsp_Spi_Init();
	Bsp_Tim_Init();
	Bsp_Can_Init();
	Bsp_DMA_Init();
	Bsp_Usart_Init();
  //mpu6500_init(SPI5);
	//ist8310_init(SPI5);
	//mpu6500_inter_init();
	//mpu6500_cali(SPI5);
  Tim_Enable();
}

extern float angle[3];

int main(void)
{
  system_init();//系统初始化
	while (1)
	{		
		//IMU_getYawPitchRoll(angle);//姿态解算任务
  }
  
}
