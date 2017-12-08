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
control_mode_t control_mode=prepare;
uint32_t DBUS_Count=0,DBUS_Rate=0,DBUS_LastRate=0,DBUS_Err=0;

void dbus_reset(DBUS_TypeDef *dbus)
{
//	dbus->key.v=0;
//	dbus->mouse.press_l=0;
//	dbus->mouse.press_r=0;
//	dbus->mouse.x=0;
//	dbus->mouse.y=0;
//	dbus->mouse.z=0;
//	dbus->rc.ch0=0;
//	dbus->rc.ch1=0;
//	dbus->rc.ch2=0;
//	dbus->rc.ch3=0;

	DBUS.mouse.x = 0;
	DBUS.mouse.y = 0;
	DBUS.mouse.z = 0;
	DBUS.mouse.press_l = 0;
	DBUS.mouse.press_r =0;
	DBUS.key.v = 0;
	
	DBUS.rc.ch0	=	0;
	DBUS.rc.ch1	= 0;
	DBUS.rc.ch2 = 0;
	DBUS.rc.ch3 = 0;

}

void dbus_getdata(void)//ң�����ݽ���
{

	DBUS.rc.ch0	=	(DBUS_BUFFER[0]|(int16_t)DBUS_BUFFER[1]<<8)&0x07FF;
	DBUS.rc.ch1	= (DBUS_BUFFER[1]>>3|(int16_t)DBUS_BUFFER[2]<<5)&0x07FF;
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
				
	if(GMPRamp.count<init_time_ms-1)
	{
		control_mode=prepare;
	}
	else 
	{
		if(DBUS.rc.s1==1)
		{
			control_mode=remote;
		}
		else if(DBUS.rc.s1==2)
		{
			control_mode=stop;
		}
		else if(DBUS.rc.s1==3)
		{
			control_mode=keyboard;
		}
		if(0)//�˴�дң�ض�ʧ���ж�����
		{
			control_mode=noremote;
		}
	}			
}


void status_check(DBUS_TypeDef *dbus)//������ģʽ
{	
	switch (control_mode)
	{
		case prepare:
		break;
		case remote:
		remote_control(&DBUS);
		remote_s2_check(&DBUS);//Ħ���ֵ������͹ر�
		break;
		case keyboard:
		keyboard_control(&DBUS);
		keyboard_k_check(&DBUS);
		break;
		case stop:
		break;
	}
	
}


ChassisSend_t ChassisSend={0};
GMRemoteSend_t GMRemoteSend={0,0};

GUNMode_t FRIStatus=FRIOFF,POKStatus=POKOFF;
uint8_t last_l,l,last_r,r;
uint16_t GUN_Count=0,GUN_Lock=0;
void keyboard_k_check(DBUS_TypeDef *dbus)
{
	static uint8_t last_l,l,last_r,r;
	
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
		if(GUN_Count==50)
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
			if(GUN_Count>50)
		{
			GUN_Count=0;
			FRIStatus=FRIOFF;
			POKStatus=POKOFF;
		}
		if(l==1)
		{
			POKStatus=POKON;
		}
		else
		{
			POKStatus=POKOFF;
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
	
	if(FRIStatus==FRIOFF&&(s2-last_s2)==-2)//Ħ���ֿ����ж�
	{
		FRIStatus=FRION;
	}
	else if(FRIStatus==FRION&&(s2-last_s2)==-2)//�ص��ж�
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


void keyboard_control(DBUS_TypeDef *dbus)//���̿���
{
	static int8_t X_Speed=0,Y_Speed=0;
	static uint16_t X_Speed_Default = 0,Y_Speed_Default = 0;

	GMRemoteSend.pitch_angle+=-(float)DBUS.mouse.y*GMPSKRate;
	GMRemoteSend.yaw_angle+=(float)DBUS.mouse.x*GMYSKRate;

	ChassisSend.Y=(float)dbus->rc.ch3*ChassisYSRRate;
	ChassisSend.X=(float)dbus->rc.ch2*ChassisXSRRate;
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);
	VAL_LIMIT(GMRemoteSend.yaw_angle,GMYPositionPID.realdata[0]-30 , GMYPositionPID.realdata[0]+30);//Ӱ��������̨����ס���贫������
}

void remote_control(DBUS_TypeDef *dbus)//ң�ؿ���
{
	GMRemoteSend.pitch_angle+=(float)dbus->rc.ch3*GMPSRRate;
	GMRemoteSend.yaw_angle+=(float)dbus->rc.ch2*GMYSRRate;
	
	ChassisSend.Y=(float)dbus->rc.ch1*ChassisYSRRate;
	ChassisSend.X=(float)dbus->rc.ch0*ChassisXSRRate;
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);
	VAL_LIMIT(GMRemoteSend.yaw_angle,GMYPositionPID.realdata[0]-30 , GMYPositionPID.realdata[0]+30);
}
