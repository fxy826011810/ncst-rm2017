#include "dbus.h"
#include "tim.h"
#include "ramp.h" 
#include "gpio.h"
#include "control.h"
#include "stm32f4xx.h"
#include "calc.h"
#include "pid.h"
#include "key.h"
 uint8_t DBUS_BUFFER[19];
 DBUS_TypeDef DBUS={0};
control_mode_t control_mode=prepare;
void dbus_getdata(void)//遥控数据解算
{
	
	

		DBUS.rc.ch0= (DBUS_BUFFER[0]|(int16_t)DBUS_BUFFER[1]<<8)&0x07FF;
		DBUS.rc.ch1= (DBUS_BUFFER[1]>>3|(int16_t)DBUS_BUFFER[2]<<5)&0x07FF;
		DBUS.rc.ch2 = (DBUS_BUFFER[2] >> 6 | (int16_t)DBUS_BUFFER[3] << 2| DBUS_BUFFER[4]<<10) & 0x07FF;
		DBUS.rc.ch3 = (DBUS_BUFFER[4] >> 1 | (int16_t)DBUS_BUFFER[5] << 7) & 0x07FF;
		if(DBUS.rc.ch0&&DBUS.rc.ch1&&DBUS.rc.ch2&&DBUS.rc.ch3)
			{
			DBUS.rc.ch0 -= 1024;
			DBUS.rc.ch1 -= 1024;
			DBUS.rc.ch2 -= 1024;
			DBUS.rc.ch3 -= 1024;
			}
		DBUS.rc.s1 = (DBUS_BUFFER[5] >> 4) & 0x03;
		DBUS.rc.s2 = (DBUS_BUFFER[5] >> 6) & 0x03;
		DBUS.mouse.x = DBUS_BUFFER[6] | (int16_t)DBUS_BUFFER[7] << 8;
		DBUS.mouse.y = DBUS_BUFFER[8] | (int16_t)DBUS_BUFFER[9] << 8;
		DBUS.mouse.z = DBUS_BUFFER[10] | (int16_t)DBUS_BUFFER[11] << 8;
		DBUS.mouse.press_l = DBUS_BUFFER[12];
		DBUS.mouse.press_r = DBUS_BUFFER[13];
		DBUS.key.v = DBUS_BUFFER[14] | (int16_t)DBUS_BUFFER[15] << 8;
				
				
				switch (control_mode)
				{
					case prepare:
						break;
					case remote:
						remote_control(&DBUS);
						break;
					case keyboard:
						keyboard_control(&DBUS);
						break;
					case stop:
						break;
				}
}

void status_check(DBUS_TypeDef *dbus)//检测控制模式
{
	if(SystemInit_Ramp.count<init_time_ms)
	{
		control_mode=prepare;
	}
	else 
	{
		if(dbus->rc.s1==1)
				{
					control_mode=remote;
				}

		else if(dbus->rc.s1==3)
				{
					control_mode=keyboard;
				}

	}
}

uint8_t last_s2,s2;
void remote_s2_check(DBUS_TypeDef *dbus)//
{
	last_s2=s2;
	s2=dbus->rc.s2;
	
	if((s2-last_s2)==-2)//上拨
	{

	}
	
	if(last_s2==3&&last_s2-s2==1)//下拨
	{

	}
	if(last_s2-s2==-1)//下拨到中
	{

	}
	
}


ChassisSend_t ChassisSend={0,660,0,660};//max速度没用
void keyboard_control(DBUS_TypeDef *dbus)//键盘控制
{
	static int8_t X_Speed=0,Y_Speed=0;
	static uint16_t X_Speed_Default = 0,Y_Speed_Default = 0;

	if(dbus->key.v&KEY_CTRL)//ctrl变速
	{
		X_Speed_Default=2000;
		Y_Speed_Default=4000;
	}
	else
 {
	X_Speed_Default=1500;
	Y_Speed_Default=3000;
 }

	if(dbus->key.v&KEY_W)//w 前后走
	{
		Y_Speed=1;
	}
	else if(dbus->key.v&KEY_S)//s
	{
		Y_Speed=-1;
	}
	else
	{
		Y_Speed=0;
		
	}
	
	if(dbus->key.v&KEY_A)//a 左右走
	{
		X_Speed=1;
	}
	else if(dbus->key.v&KEY_D)//d
	{
		X_Speed=-1;
	}
	else 
	{
		X_Speed=0;
	}
	
	
	
	if(dbus->key.v&KEY_Q)//q
	{

	}
	if(dbus->key.v&KEY_E)//e
	{

	}
	
	if(dbus->key.v&KEY_SHIFT)//shift
	{
		
	}
	if(dbus->mouse.press_l)//左键
	{

	}
		if(dbus->mouse.press_r)//右键
	{
		
	}

	ChassisSend.Y=(float)Y_Speed*Y_Speed_Default*1.0f;
	ChassisSend.X=-(float)X_Speed*X_Speed_Default*1.0f;
	ChassisSend.rotate=DBUS.mouse.x*30;
}

extern  u8 j,k;
u8 f=0,e=0;
uint8_t m=1,n=1;
void remote_control(DBUS_TypeDef *dbus)//遥控控制
{
	static uint8_t s2=0;
	static int16_t ch3_up,ch3_down;
	s2=dbus->rc.s2;
	
	if(dbus->rc.ch3>330)
		 j=0;
	
  if(j==0)
	   ch3_up=-dbus->rc.ch3;
	else if(j==1)
	   ch3_up=0;
											 
  if(dbus->rc.ch3<-330)
		 k=0;
	
	if(k==0)
	   ch3_down=-dbus->rc.ch3;
  else if(k==1)
	   ch3_down=0;
	
	if(s2==3)
	{	
		m=1;
		n=1;
	if(ch3_up>330)
	{
	TIM2->CCR4=450;
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);//使能正转en
	if(switch2.PressedDowTimes==0)
	{
  if(f==1)
		{
		j=1;
		f=0;
		}
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);//cw
	}
	else if(switch2.PressedDowTimes==1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
		f=1;
		
	}
  }

		if(ch3_down<-330)
	{
	TIM2->CCR4=450;
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);//使能反转
		if(switch3.PressedDowTimes==0)
	{
  if(e==1)
		{
		k=1;
		e=0;
		}
		GPIO_SetBits(GPIOC, GPIO_Pin_6);//cw
	}	
		else if(switch3.PressedDowTimes==1)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		 e=1;
	}
	
	}
	
	
		if((ch3_down<330&&ch3_down>-330)||(ch3_up<330&&ch3_up>-330))
	{
	TIM2->CCR4=0;
	GPIO_SetBits(GPIOA, GPIO_Pin_6);//失能
	}

}
	if(s2==2)
{ 	
	n=1;
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);//使能正转en
	if(switch2.PressedDowTimes==0)
	{
  if(m==1)
		{
			TIM2->CCR4=450;
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);//cw
		}
		if(m==0)
		{
			TIM2->CCR4=0;
		}
	}
	else if(switch2.PressedDowTimes==1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
		m=0;
		
	}

}	 
	if(s2==1)
{ 
		m=1;
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);//使能反转
		
	if(switch3.PressedDowTimes==0)
	{
  if(n==1)
		{
			TIM2->CCR4=450;
			GPIO_SetBits(GPIOC, GPIO_Pin_6);//cw
		}	
		if(n==0)
		{
			TIM2->CCR4=0;
		//GPIO_SetBits(GPIOA, GPIO_Pin_6);//失能
		}	
	}
		else if(switch3.PressedDowTimes==1)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		n=0;
	}
	
}	 
if(DBUS.rc.ch1<330&&DBUS.rc.ch1>-330)
{
	ChassisSend.Y=(float)DBUS.rc.ch1*7.5f;
}
else
{
	ChassisSend.Y=(float)DBUS.rc.ch1*9.5f;
}
if(DBUS.rc.ch0<330&&DBUS.rc.ch0>-330)
{
	ChassisSend.X=(float)DBUS.rc.ch0*5.5f;
}
else
{
	ChassisSend.X=(float)DBUS.rc.ch0*8.5f;
}
if(DBUS.rc.ch2<330&&DBUS.rc.ch2>-330)
{
	ChassisSend.rotate=DBUS.rc.ch2*5.5f;
}
else
{
	ChassisSend.rotate=DBUS.rc.ch2*6.5f;
}
}
	

