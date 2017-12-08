#include "usart.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "dbus.h"
#include <stdio.h>

int fputc(int ch,FILE*f)//printf函数重定义
{
	USART_SendData(USART3,(unsigned char)ch);
	while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));
	return ch;
	
}

void Bsp_Usart_Init(void)
{


}


		


