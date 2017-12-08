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

//����ʵ�ִ���һ�μ�һ�ε�
//����8000�Ƕ�Ϊ360��Ƕȣ�PIDû����������
//δ��ʵ����;�˳���ֱ�Ӵ�������ת��





void system_init(void)
{
	DJ_status=_24v;//�������ĵ�ѹֵ
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
	
  system_init();//ϵͳ��ʼ��
  
	while (1)
	{		
		if(tim_flag)
		{
			control_loop();
			tim_flag=0;
		}
  }
  
}
