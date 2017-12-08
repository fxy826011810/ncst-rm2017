#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "gpio.h"
#include "nvic.h"
#include "can.h"
#include "dbus.h"
#include "pid.h"
#include "tim.h"
#include "dma.h"
#include "ramp.h"
#include "stdio.h"
#include "control.h"
#include "key.h"
//主控制函数在tim函数下中断里的control_loop（）；
 extern uint8_t DBUS_BUFFER[19];
void system_init(void)
{	
	delay_ms(1000);
	ramp_config();
  Bsp_Pid_Init();
  Bsp_NVIC_Init();
	Bsp_GPIO_Init();
	Bsp_Pid_Init();
	Timer3Init(100);
  Bsp_Tim_Init();
	Bsp_Can_Init();
	Bsp_DMA_Init();
	Bsp_Usart_Init();
  KeyInit();
}

 u8 t=0,a=0,j=0,k=0;
	
int main(void)
{
	system_init();//系统初始化
while(1)
 {

 }

}