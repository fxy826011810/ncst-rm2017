#include "dbus.h"
#include "tim.h"
#include "ramp.h" 
#include "gpio.h"
#include "control.h"
#include "stm32f4xx.h"
#include "calc.h"
#include "pid.h"
#include "Infantry.h"
uint8_t DBUS_BUFFER[19];
DBUS_TypeDef DBUS={0};
control_mode_t control_mode=prepare;//控制模式定义 初始为准备状态

void dbus_reset(DBUS_TypeDef *dbus)
{
			dbus->key.v=0;
			dbus->mouse.press_l=0;
			dbus->mouse.press_r=0;
			dbus->mouse.x=0;
			dbus->mouse.y=0;
			dbus->mouse.z=0;
			dbus->rc.ch0=0;
			dbus->rc.ch1=0;
			dbus->rc.ch2=0;
			dbus->rc.ch3=0;
}




void Control_Mode_Change(DBUS_TypeDef *dbus)//什么条件触发什么模式
{
	switch (control_mode)
	{
		case prepare:									//准备
		break;
		
		case remote:									//遥控
			remote_control(dbus);
			remote_s2_check(dbus); //摩擦轮的启动和关闭
		CMPWM1=0;
		CMPWM2=0;
		CMPWM3=0;
		break;
		
		case MechArm:								//键盘
			MechArm_ControlLoop(&DBUS);//机械手臂任务
		break;
		
		case noremote:								//遥控丢失
			dbus_reset(dbus);
		break;
		case keyboard:										//停止
			keyboard_control(dbus);
			keyboard_k_check(dbus);
		break;
		case stop:										//停止
			
		break;
		
		}
}


void dbus_calc(DBUS_TypeDef *dbus)//遥控数据解算
{

	dbus->rc.ch0= (DBUS_BUFFER[0]|(int16_t)DBUS_BUFFER[1]<<8)&0x07FF;
	dbus->rc.ch1= (DBUS_BUFFER[1]>>3|(int16_t)DBUS_BUFFER[2]<<5)&0x07FF;
	dbus->rc.ch2 = (DBUS_BUFFER[2] >> 6 | (int16_t)DBUS_BUFFER[3] << 2| DBUS_BUFFER[4]<<10) & 0x07FF;
	dbus->rc.ch3 = (DBUS_BUFFER[4] >> 1 | (int16_t)DBUS_BUFFER[5] << 7) & 0x07FF;
  if(dbus->rc.ch0&&dbus->rc.ch1&&dbus->rc.ch2&&dbus->rc.ch3)
  {
    dbus->rc.ch0 -= 1024;
    dbus->rc.ch1 -= 1024;
    dbus->rc.ch2 -= 1024;
    dbus->rc.ch3 -= 1024;
  }
	dbus->rc.s1 = (DBUS_BUFFER[5] >> 4) & 0x03;
	dbus->rc.s2 = (DBUS_BUFFER[5] >> 6) & 0x03;
	dbus->mouse.x = DBUS_BUFFER[6] | (int16_t)DBUS_BUFFER[7] << 8;
	dbus->mouse.y = DBUS_BUFFER[8] | (int16_t)DBUS_BUFFER[9] << 8;
	dbus->mouse.z = DBUS_BUFFER[10] | (int16_t)DBUS_BUFFER[11] << 8;
	dbus->mouse.press_l = DBUS_BUFFER[12];
	dbus->mouse.press_r = DBUS_BUFFER[13];
	dbus->key.v = DBUS_BUFFER[14] | (int16_t)DBUS_BUFFER[15] << 8;	
	if(GMPRamp.count>=init_time_ms)//过了系统启动时间后
	{		
		if(dbus->rc.s1==1)
				{
					control_mode=remote;
				}
		else if(dbus->rc.s1==3)
				{
					control_mode=MechArm;
				}
		else if(dbus->rc.s1==2)
				{
					control_mode=keyboard;
				}		
		
	}
	else //系统时间，没有加载完时
	{
		control_mode=prepare;
	}	
}


ChassisSend_t ChassisSend={0};
GMRemoteSend_t GMRemoteSend={0,0};
uint8_t	POKKeep=0;
GUNMode_t FRIStatus=FRIOFF,POKStatus=POKOFF,LastPOKStatus=POKOFF,SG_Status=SGOFF;
uint8_t last_l,l,last_r,r;//左键右键
uint16_t GUN_Count=0,GUN_Lock=0;
void keyboard_k_check(DBUS_TypeDef *dbus)
{
	
	l=dbus->mouse.press_l;
	r=dbus->mouse.press_r;

		if(FRIStatus==FRIOFF)
	{
			POKStatus=POKOFF;
			if(last_r==0&&r==1)
		{
			FRIStatus=FRIWAIT;
		}

	}
	else if(FRIStatus==FRIWAIT)
	{
		if(r==1)
		{
			GUN_Count++;
		}
		else
		{
			GUN_Count=0;
		}
		if(GUN_Count>=50*10)
		{
			GUN_Lock=1;
			GUN_Count=0;
			FRIStatus=FRION;
		}
	}
	else if(FRIStatus==FRION)
	{
		if(last_r==1&&r==0)
		{
			GUN_Lock=0;
		}
		if(r==1&&GUN_Lock==0)
		{
			GUN_Count++;
		}
		else
		{
			GUN_Count=0;
		}
			if(GUN_Count>=50*10)
		{
			GUN_Count=0;
			FRIStatus=FRIOFF;
//			POKStatus=POKOFF;
//			POKKeep=0;
//			POKONRamp.resetcount(&POKONRamp);
//			POKOFFRamp.resetcount(&POKOFFRamp);
		}
		if(l==1)
		{
			POKStatus=POKON;
		}
		else
		{
			POKKeep=0;
		}

	}

	last_l=l;
	last_r=r;
}



void remote_s2_check(DBUS_TypeDef *dbus)
{
	static uint8_t last_s2,s2;
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
		POKKeep=1;
	}
	if(FRIStatus==FRION&&DBUS.rc.s2==3)
	{
		POKStatus=POKOFF;
		POKKeep=0;
		POKONRamp.resetcount(&POKONRamp);
		POKOFFRamp.resetcount(&POKOFFRamp);
	}
}

void keyboard_control(DBUS_TypeDef *dbus)//键盘控制
{
  	GMRemoteSend.pitch_angle+=-(float)dbus->mouse.y*GMPKRate;

//-------以下为底盘xy轴旋转速度值和遥控值做变换
	if(dbus->rc.ch0<330&&dbus->rc.ch0>-330)
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYRate;//ec60比例
	}
	else
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYHRate;//ec60比例	
	}
	
	if(dbus->rc.ch1<330&&dbus->rc.ch1>-330)
	{
	ChassisSend.X=(float)dbus->rc.ch1*ChassisXRate;
	}
	else
	{
	ChassisSend.X=(float)dbus->rc.ch1*ChassisXHRate;
	}
	if(dbus->mouse.x<25&&dbus->mouse.x>-25)
	{
	ChassisSend.rotate=(float)dbus->mouse.x*ChassisKRRate;
	}
	else
	{
	ChassisSend.rotate=(float)dbus->mouse.x*ChassisKRHRate;
	}
	
	
//-------以下为pitch.yaw角度限制
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);//pitch（上下20度）
}

void remote_control(DBUS_TypeDef *dbus)//遥控控制
{
	//-------以下为pitch.yaw轴速度值和遥控值做变换
	GMRemoteSend.pitch_angle+=(float)dbus->rc.ch3*GMPRate;
//  GMRemoteSend.yaw_angle+=(float)dbus->rc.ch2*0.004f;英雄没有yaw

//-------以下为底盘xy轴旋转速度值和遥控值做变换
	if(dbus->rc.ch0<330&&dbus->rc.ch0>-330)
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYRate;//ec60比例
	}
	else
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYHRate;//ec60比例	
	}
	if(dbus->rc.ch1<330&&dbus->rc.ch1>-330)
	{
	ChassisSend.X=(float)dbus->rc.ch1*ChassisXRate;
	}
	else
	{
	ChassisSend.X=(float)dbus->rc.ch1*ChassisXHRate;
	}
	if(dbus->rc.ch2<330&&dbus->rc.ch2>-330)
	{
	ChassisSend.rotate=(float)dbus->rc.ch2*ChassisRRate;
	}
	else
	{
	ChassisSend.rotate=(float)dbus->rc.ch2*ChassisRHRate;
	}
	
//-------以下为pitch.yaw角度限制
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);//pitch（上下20度）

}
