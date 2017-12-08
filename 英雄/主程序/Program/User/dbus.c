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
control_mode_t control_mode=prepare;//����ģʽ���� ��ʼΪ׼��״̬

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




void Control_Mode_Change(DBUS_TypeDef *dbus)//ʲô��������ʲôģʽ
{
	switch (control_mode)
	{
		case prepare:									//׼��
		break;
		
		case remote:									//ң��
			remote_control(dbus);
			remote_s2_check(dbus); //Ħ���ֵ������͹ر�
		CMPWM1=0;
		CMPWM2=0;
		CMPWM3=0;
		break;
		
		case MechArm:								//����
			MechArm_ControlLoop(&DBUS);//��е�ֱ�����
		break;
		
		case noremote:								//ң�ض�ʧ
			dbus_reset(dbus);
		break;
		case keyboard:										//ֹͣ
			keyboard_control(dbus);
			keyboard_k_check(dbus);
		break;
		case stop:										//ֹͣ
			
		break;
		
		}
}


void dbus_calc(DBUS_TypeDef *dbus)//ң�����ݽ���
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
	if(GMPRamp.count>=init_time_ms)//����ϵͳ����ʱ���
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
	else //ϵͳʱ�䣬û�м�����ʱ
	{
		control_mode=prepare;
	}	
}


ChassisSend_t ChassisSend={0};
GMRemoteSend_t GMRemoteSend={0,0};
uint8_t	POKKeep=0;
GUNMode_t FRIStatus=FRIOFF,POKStatus=POKOFF,LastPOKStatus=POKOFF,SG_Status=SGOFF;
uint8_t last_l,l,last_r,r;//����Ҽ�
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

void keyboard_control(DBUS_TypeDef *dbus)//���̿���
{
  	GMRemoteSend.pitch_angle+=-(float)dbus->mouse.y*GMPKRate;

//-------����Ϊ����xy����ת�ٶ�ֵ��ң��ֵ���任
	if(dbus->rc.ch0<330&&dbus->rc.ch0>-330)
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYRate;//ec60����
	}
	else
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYHRate;//ec60����	
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
	
	
//-------����Ϊpitch.yaw�Ƕ�����
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);//pitch������20�ȣ�
}

void remote_control(DBUS_TypeDef *dbus)//ң�ؿ���
{
	//-------����Ϊpitch.yaw���ٶ�ֵ��ң��ֵ���任
	GMRemoteSend.pitch_angle+=(float)dbus->rc.ch3*GMPRate;
//  GMRemoteSend.yaw_angle+=(float)dbus->rc.ch2*0.004f;Ӣ��û��yaw

//-------����Ϊ����xy����ת�ٶ�ֵ��ң��ֵ���任
	if(dbus->rc.ch0<330&&dbus->rc.ch0>-330)
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYRate;//ec60����
	}
	else
	{
	ChassisSend.Y=(float)dbus->rc.ch0*ChassisYHRate;//ec60����	
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
	
//-------����Ϊpitch.yaw�Ƕ�����
	VAL_LIMIT(GMRemoteSend.pitch_angle,-20,20);//pitch������20�ȣ�

}
