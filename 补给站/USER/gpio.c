#include "gpio.h"
#include "stm32f10x.h"



void Bsp_GPIO_Init(void)
{
	GPIO_InitTypeDef						gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);
gpio.GPIO_Mode=GPIO_Mode_AF_PP;
     gpio.GPIO_Speed=GPIO_Speed_50MHz;
     gpio.GPIO_Pin=GPIO_Pin_9;//按板子定
    GPIO_Init(GPIOB,&gpio);
	gpio.GPIO_Mode=GPIO_Mode_IPU;
     gpio.GPIO_Speed=GPIO_Speed_50MHz;
     gpio.GPIO_Pin=GPIO_Pin_8;//按板子定
    GPIO_Init(GPIOB,&gpio);
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
	//限位开关
	 gpio.GPIO_Mode=GPIO_Mode_IPU;
     gpio.GPIO_Speed=GPIO_Speed_50MHz;
     gpio.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_0;//按板子定
    GPIO_Init(GPIOE,&gpio);
}

#define KEY_L GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KEY_R GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
key_t key_l={0,0},key_r={0,0};
#define key_set 0
void Key_Scan(void)
{
	if(KEY_L==key_set)
		{
			if(key_l.time>4)
				key_l.status=1;
			else
				key_l.time+=1;		
		}
	else
		{
			key_l.time=0;
			key_l.status=0;
		}
	if(KEY_R==key_set)
		{
			if(key_r.time>4)
				key_r.status=1;
			else
				key_r.time+=1;		
		}
	else
		{
			key_r.time=0;
			key_r.status=0;
		}
}



