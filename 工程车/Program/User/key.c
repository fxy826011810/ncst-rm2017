#include "key.h"
#include "tim.h"

#include "delay.h"
 Switch  switch2,switch3;

void KeyInit(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTCʱ��

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PC0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //����
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC0
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PC0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP; //����
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOC0
 

	switch2.PressedDowTimes=0;
	switch2.CheckLowTimes  =0;
	
	switch3.PressedDowTimes=0;
	switch3.CheckLowTimes  =0;
} 