#include "key.h"
#include "tim.h"

#include "delay.h"
 Switch  switch2,switch3;

void KeyInit(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE,ENABLE);//使能PORTA,PORTC时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PC0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //上拉
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC0
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PC0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //上拉
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOC0
 

	switch2.PressedDowTimes=0;
	switch2.CheckLowTimes  =0;
	
	switch3.PressedDowTimes=0;
	switch3.CheckLowTimes  =0;
} 