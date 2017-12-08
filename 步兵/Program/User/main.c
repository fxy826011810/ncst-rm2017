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
#include "i2c.h" 
#include "ramp.h"
#include "stdio.h"
#include "control.h"
#include "encoder.h"
extern float angle[3];
void system_init(void)
{
	ramp_config();
  Bsp_Pid_Init();
	delay_ms(1000);
	delay_ms(1000);
  Bsp_NVIC_Init();
	Bsp_GPIO_Init();
	Quad_Encoder_Configuration();
	Bsp_Pid_Init();
	Bsp_Tim_Init();
	Bsp_Can_Init();
	Bsp_DMA_Init();
	Bsp_Usart_Init();
	Bsp_IIC_Init();
  mpu6050_init();
  delay_ms(200);
  Pwm_Enable();    
  mpu6050_inter_init();
  mpu6050_enable();
	Encoder_Start();
}

extern float mygetqval[9];
int main(void)
{
	
  system_init();//系统初始化
  
	while (1)
	{		
		IMU_getValues(mygetqval);
		//IMU_getYawPitchRoll(angle);//陀螺仪角度值获取


  }
  
}
