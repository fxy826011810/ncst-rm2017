#include "delay.h"
#include "stm32f10x.h"
void delay_ms(uint16_t t)
{
  u32 i;
	SysTick->LOAD=8000*t;
        SysTick->VAL=0;
	SysTick->CTRL=0x01;
        do
        {
          i=SysTick->CTRL;
        }
	while((i&0x01)&&!(i&0x10000));
	SysTick->CTRL=0;
        SysTick->VAL=0;
	
}

void delay_us(uint16_t t)
{
  u32 i;
	SysTick->LOAD=8*t;
        SysTick->VAL=0;
	SysTick->CTRL=0x01;
        do
        {
          i=SysTick->CTRL;
        }
	while((i&0x01)&&!(i&0x10000));
	SysTick->CTRL=0;
        SysTick->VAL=0;
	
}

