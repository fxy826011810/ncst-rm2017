#include "dbus.h"
#include "tim.h"
#include "ramp.h" 
#include "gpio.h"
#include "control.h"
#include "stm32f4xx.h"
#include "calc.h"
#include "pid.h"
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
					  remote_s2_check(&DBUS);//摩擦轮的启动和关闭
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
	if(GMPRamp.count<init_time_ms-1)
	{
		control_mode=prepare;
	}
	else 
	{
		if(dbus->rc.s1==1)
				{
					control_mode=remote;
				}
//				else if(dbus->rc.s1==2)
//				{
//					control_mode=stop;
//				}
				else if(dbus->rc.s1==3)
				{
					control_mode=keyboard;
				}
//				else if(dbus->rc.s1==0noremote)
//				{
//					control_mode=noremote;
//				}
	}
}


ChassisSend_t ChassisSend={0,660,0,660};
GMRemoteSend_t GMRemoteSend={0,0};

GUNMode_t FRIStatus=FRIOFF,POKStatus=POKOFF;
uint8_t last_l,l,last_r,r,ls_check=0;
uint16_t FRI_Count=0;
void keyboard_k_check(DBUS_TypeDef *dbus)
{
	last_l=l;
	last_r=r;
	l=dbus->mouse.press_l;
	r=dbus->mouse.press_r;

	if(FRIStatus==FRION)
	{
		if(r==1&&last_r==1)
		{
			FRI_Count++;
		}
		else
		{
			FRI_Count=0;
		}
			if(FRI_Count>=50)
		{
			FRIStatus=FRIOFF;
			POKStatus=POKOFF;
		}
		else if(l==1)
		{
			POKStatus=POKON;
		}
		else
		{
			POKStatus=POKOFF;
		}

	}
	else if(FRIStatus==FRIOFF)
	{
			POKStatus=POKOFF;
			//FRI_Count=0;
			if(last_r==0&&r==1)
		{
			FRIStatus=FRION;
		}

	}
}

uint8_t last_s2,s2;
void remote_s2_check(DBUS_TypeDef *dbus)
{
	 //last_s2,s2;
	last_s2=s2;
	s2=dbus->rc.s2;
	
	if(FRIStatus==FRIOFF&&(s2-last_s2)==-2)//摩擦轮开的判断
	{
		FRIStatus=FRION;
	}
	else if(FRIStatus==FRION&&(s2-last_s2)==-2)//关的判断
	{
		FRIStatus=FRIOFF;
	}

	
	if(FRIStatus==FRION&&last_s2==3&&last_s2-s2==1)
	{
		POKStatus=POKON;
	}
	if(POKStatus==POKON&&last_s2-s2==-1)
	{
		POKStatus=POKOFF;
	}
	
}
//MouseMode_t MouseMode;
uint8_t mouse_check(DBUS_TypeDef *dbus)
{
	
}

void keyboard_control(DBUS_TypeDef *dbus)//键盘控制
{
	static int8_t X_Speed=0,Y_Speed=0;
	static uint16_t X_Speed_Default = 0,Y_Speed_Default = 0;

	if(dbus->key.v&KEY_CTRL)//ctrl
	{
		X_Speed_Default=2000;
		Y_Speed_Default=4000;
		
	}
	else
{
	X_Speed_Default=1500;
	Y_Speed_Default=3000;
}
	if(dbus->key.v&KEY_W)//w
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
	if(dbus->key.v&KEY_A)//a
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
	else
	{

	}
	if(dbus->key.v&KEY_SHIFT)//shift
	{
		
	}
	if(dbus->mouse.press_l)
	{

	}
		if(dbus->mouse.press_r)
	{
		
	}
	else
	GMRemoteSend.pitch_angle+=-(float)DBUS.mouse.y*0.04f;
	GMRemoteSend.yaw_angle+=(float)DBUS.mouse.x*0.04f;
	ChassisSend.Y=(float)Y_Speed*Y_Speed_Default*1.0f;
	ChassisSend.X=-(float)X_Speed*X_Speed_Default*1.0f;
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);
	VAL_LIMIT(GMRemoteSend.yaw_angle,GMYPositionPID.realdata[0]-30 , GMYPositionPID.realdata[0]+30);
}

void remote_control(DBUS_TypeDef *dbus)//遥控控制
{
	GMRemoteSend.pitch_angle+=(float)dbus->rc.ch3*0.005f;
	GMRemoteSend.yaw_angle+=(float)dbus->rc.ch2*0.004f;
	
	//GMRemoteSend.pitch_angle+=(float)dbus->rc.ch3*0.004;//yaw位置PID输入值
	//GMRemoteSend.yaw_angle+=(float)dbus->rc.ch2*0.004;

//	ChassisSend.Y=(float)dbus->rc.ch1*0.07f;
//	ChassisSend.X=(float)dbus->rc.ch0*0.7f;
	
	
		ChassisSend.Y=(float)dbus->rc.ch1*6.5f;
	ChassisSend.X=(float)dbus->rc.ch0*5.5f;
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);
	VAL_LIMIT(GMRemoteSend.yaw_angle,GMYPositionPID.realdata[0]-30 , GMYPositionPID.realdata[0]+30);
}
