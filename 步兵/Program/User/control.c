#include "control.h"
#include "stm32f4xx.h"
#include "mpu6050.h"
#include "encoder.h"
#include "gpio.h"
#include "pid.h"
#include "dbus.h"
#include "can.h"
#include "tim.h"
#include "ramp.h"
#include "gpio.h"
#include "Infantry.h"
#include "judge.h"


float ppkp=19 ,ppki=0,ppkd=0,pskp=20,pski=0,pskd=0,realpangle;

void GMPitch_ControlLoop(void)
{

	GMPitchEncoder.ecd_bias=GMPitchAngle_Default;//PITCH��Ľ���ֵ 3��

	GMPPositionPID.Kp = ppkp;
	GMPPositionPID.Ki = ppki;
	GMPPositionPID.Kd = ppkd;
					
	GMPSpeedPID.Kp = pskp;
	GMPSpeedPID.Ki = pski;
	GMPSpeedPID.Kd = pskd;

	realpangle=GMPitchEncoder.ecd_angle;
	GMPPositionPID.realdata[0] = -realpangle*GMPRamp.calc(&GMPRamp);//�������ǵ�PITCH�Ƕ�-����ֵ��*б�º���
  GMPPositionPID.setdata=GMRemoteSend.pitch_angle;//ң���������PITCH��ĽǶ�
  GMPPositionPID.test(&GMPPositionPID);
        
  GMPSpeedPID.setdata= GMPPositionPID.output[0];
  GMPSpeedPID.realdata[NOW]=MPU_REAL_DATA.gy;//Y��Ľ��ٶ�
  GMPSpeedPID.test(&GMPSpeedPID);
}

float ypkp=19,ypki=0,ypkd=0,yskp=20,yski=0,yskd=0,yaw_stop_angle=0;
float ZGyroModuleAngle_Bias=0.0f;
extern float ZGyroModuleAngle;
void GMYaw_ControlLoop(void)
{
	GMYawEncoder.ecd_bias=GMYawAngle_Default;//yaw��Ľ���ֵ 3�ų�

	GMYPositionPID.Kp = ypkp;
	GMYPositionPID.Ki = ypki;
	GMYPositionPID.Kd = ypkd;
	GMYSpeedPID.Kp = yskp;
	GMYSpeedPID.Ki = yski;
	GMYSpeedPID.Kd = yskd;

	if(control_mode==prepare)
	{
		GMYPositionPID.setdata=0;
		GMYPositionPID.realdata[0]=-GMYawEncoder.ecd_angle*GMYRamp.calc(&GMYRamp);//yaw��Ƕ�ֵ*б�º���
		ZGyroModuleAngle_Bias=ZGyroModuleAngle;
	}
	else 
	{	
		if(control_mode==remote)
		{
			GMYPositionPID.setdata=GMRemoteSend.yaw_angle;
			GMYPositionPID.realdata[0]=ZGyroModuleAngle-ZGyroModuleAngle_Bias;//�趨ֵд������z�������
			yaw_stop_angle=GMRemoteSend.yaw_angle;//��ֹ�л�ģʽyaw������
		}
		else if(control_mode==keyboard)
		{
			GMYPositionPID.setdata=0;
			GMYPositionPID.setdata=GMRemoteSend.yaw_angle;//ң�������������ֵ  �Ժ���������
			GMYPositionPID.realdata[0]=ZGyroModuleAngle-ZGyroModuleAngle_Bias;
			yaw_stop_angle=GMRemoteSend.yaw_angle;//��ֹ�л�ģʽyaw������
		}
		else
		{
			GMYPositionPID.setdata=yaw_stop_angle;
			GMYPositionPID.realdata[0]=ZGyroModuleAngle-ZGyroModuleAngle_Bias;
		}
	}
  GMYPositionPID.test(&GMYPositionPID);
        
  GMYSpeedPID.setdata= GMYPositionPID.output[NOW];
  GMYSpeedPID.realdata[NOW]=MPU_REAL_DATA.gz;//������z��Ľ��ٶ�
  GMYSpeedPID.test(&GMYSpeedPID);
        
}
extern uint16_t MaxA;
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed)
{
    float Buffer[4], Param, MaxSpeed;
    uint8_t index;
                                         //        |
                                         //	     _____
    Buffer[0] = Vx - Vy + Omega;         //   2||     || 1  ��������ʱ��תEC60
    Buffer[1] = Vx + Vy + Omega;         //     |     |
		Buffer[2] = -Vx + Vy + Omega;        //     |     |
		Buffer[3] = -Vx - Vy + Omega;        //  3 ||     || 4
                                         //      -----

    //�޷�
    for(index = 0, MaxSpeed = 0; index < 4; index++)
    {
        if((Buffer[index] > 0 ? Buffer[index] : -Buffer[index]) > MaxSpeed)
        {
            MaxSpeed = (Buffer[index] > 0 ? Buffer[index] : -Buffer[index]);
        }
    }
//		if(game.realChassisOutW<75)
//		{
    if(MaxWheelSpeed < MaxSpeed)
    {
        Param = (float)MaxWheelSpeed / MaxSpeed;
        Speed[0] = Buffer[0] * Param;
        Speed[1] = Buffer[1] * Param;
        Speed[2] = Buffer[2] * Param;
        Speed[3] = Buffer[3] * Param; 
    }
    else
    {
        Speed[0] = Buffer[0];
        Speed[1] = Buffer[1];
        Speed[2] = Buffer[2];
        Speed[3] = Buffer[3];
    }
//		}
//		else
//		{
//			if((MaxWheelSpeed*75/game.realChassisOutW) < MaxSpeed)
//    {
//        Param = (float)MaxWheelSpeed*75/game.realChassisOutW / MaxSpeed;
//        Speed[0] = Buffer[0] * Param;
//        Speed[1] = Buffer[1] * Param;
//        Speed[2] = Buffer[2] * Param;
//        Speed[3] = Buffer[3] * Param; 
//    }
//    else
//    {
//        Speed[0] = Buffer[0];
//        Speed[1] = Buffer[1];
//        Speed[2] = Buffer[2];
//        Speed[3] = Buffer[3];
//    }
//		}
}


int16_t chassisspeed[4];
uint8_t CMStop=DISABLE,i,PowerCount=0;
int16_t CMXSpeedS,CMYSpeedS,CMRSpeedS;
//float crkp=80 ,crki=0,crkd=0;
uint16_t SumA,abs1,abs2,abs3,abs4;
void CMControlLoop(void)
{
	if(control_mode==prepare)//���̲��������
	{
		MecanumCalculate(0,0,0,chassisspeed);
	}
	else 
	{
		CMRotatePID.Kp=crkp;
		CMRotatePID.Ki=crki;
		CMRotatePID.Kd=crkd;		 
		CMRotatePID.setdata = 0;//���̸������
		CMRotatePID.realdata[0] = GMYawEncoder.ecd_angle;
		CMRotatePID.test(&CMRotatePID);
		if(control_mode==remote||control_mode==keyboard)
		{
			if(ChassisSend.X==0)
			{
				CMXRampR.resetcount(&CMXRampR);
			}
			if(ChassisSend.Y==0)
			{
				CMYRampR.resetcount(&CMYRampR);
			}
				MecanumCalculate(ChassisSend.X*CMXRampR.calc(&CMXRampR),ChassisSend.Y*CMYRampR.calc(&CMYRampR),CMRotatePID.output[0],chassisspeed);
		}
		else
		{
			ChassisSend.rotate=0;
			ChassisSend.X=0;
			ChassisSend.Y=0;
			CMRotatePID.setdata = 0;//���̸������
			CMRotatePID.realdata[0] = 0;
		}
	}
	

  CM1SpeedPID.setdata=chassisspeed[0];//д��ң��������ֵ
  CM2SpeedPID.setdata=chassisspeed[1];
	CM3SpeedPID.setdata=chassisspeed[2];
	CM4SpeedPID.setdata=chassisspeed[3];
		
	CM1SpeedPID.realdata[0]=cm1_rec.RollSpeed;//Ҫ��ec60���ٶȸ�Ϊ������ı����ٶ�ֵ
	CM2SpeedPID.realdata[0]=cm2_rec.RollSpeed;
	CM3SpeedPID.realdata[0]=cm3_rec.RollSpeed;
	CM4SpeedPID.realdata[0]=cm4_rec.RollSpeed;
	
	CM1SpeedPID.test(&CM1SpeedPID);
	CM2SpeedPID.test(&CM2SpeedPID);
	CM3SpeedPID.test(&CM3SpeedPID);
	CM4SpeedPID.test(&CM4SpeedPID);
	cm_senddata(CAN2,(int)CM1SpeedPID.output[NOW],(int)CM2SpeedPID.output[NOW],(int)CM3SpeedPID.output[NOW],(int)CM4SpeedPID.output[NOW]);
//	abs1=CM1SpeedPID.output[NOW]>0?CM1SpeedPID.output[NOW]:(-CM1SpeedPID.output[NOW]);
//	abs2=CM2SpeedPID.output[NOW]>0?CM2SpeedPID.output[NOW]:(-CM2SpeedPID.output[NOW]);
//	abs3=CM3SpeedPID.output[NOW]>0?CM3SpeedPID.output[NOW]:(-CM3SpeedPID.output[NOW]);
//	abs4=CM4SpeedPID.output[NOW]>0?CM4SpeedPID.output[NOW]:(-CM4SpeedPID.output[NOW]);
//	SumA=abs1+abs2+abs3+abs4;
//	
//		PowerPID.setdata=75;
//		if(game.realChassisOutW>75)
//	{
//		PowerCount++;
//		PowerPID.realdata[0]=game.realChassisOutW;
//	}
//	else if(game.remainPower==60&&(game.realChassisOutW<75))
//	{
//		PowerPID.realdata[0]=75;
//	}
//	else if((game.realChassisOutW<75))
//	{
//		PowerPID.realdata[0]=80;
//	}
//	
//		PowerPID.test(&PowerPID);
//		cm_senddata(CAN2,(int)CM1SpeedPID.output[NOW]+(int)CM1SpeedPID.output[NOW]/(int)SumA*(int)PowerPID.output,\
//			(int)CM2SpeedPID.output[NOW]+(int)CM2SpeedPID.output[NOW]/(int)SumA*(int)PowerPID.output,\
//				(int)CM3SpeedPID.output[NOW]+(int)CM3SpeedPID.output[NOW]/(int)SumA*(int)PowerPID.output,\
//					(int)CM4SpeedPID.output[NOW]+(int)CM4SpeedPID.output[NOW]/(int)SumA*(int)PowerPID.output);
}

extern GUNMode_t POKStatus,FRIStatus;//�������״̬ Ħ����״̬
void shooting_ControlLoop(void)
{
		if(FRIStatus==FRION)//Ħ����״̬��
	{
		LASER_ON();
		FRISpeed_Set(1000+FRISpeed*FRIRamp.calc(&FRIRamp));//Ħ��������
	}
	 else if(FRIStatus==FRIOFF)//Ħ���ֹر�
	{
		LASER_OFF();
		FRISpeed_Set(1000);
		FRIRamp.resetcount(&FRIRamp);
		POK_E_OFF;
		POKStatus=POKOFF;
	}
	
	if(POKStatus==POKON)//�������״̬��
  {
		POK_E_ON;//�������������Դ
		POKSpeedPID.setdata=POKSpeed;
  }
	else if(POKStatus==POKOFF)
	{
		POK_E_OFF;//�رղ��������Դ
		POKSpeedPID.setdata=0;
		POKSpeedPID.reset(&POKSpeedPID);
  }
	
		POKSpeedPID.realdata[0]=GetQuadEncoderDiff();
		POKSpeedPID.test(&POKSpeedPID);
		TIM9->CCR1=POKSpeedPID.output[0];
	
}

void Monitor_ControlLoop(void)
{
	DBUS_Rate=DBUS_Count;
	DBUS_Count=0;
	if(DBUS_Rate==0&&DBUS_LastRate==0)
	{
		DBUS_Err++;
	}
	if(DBUS_Err==4)
	{
		DBUS_Err=0;
		dbus_reset(&DBUS);
	}
	DBUS_LastRate=DBUS_Rate;
}


uint16_t heart=0;//������
void control_loop(void)
{
		heart++;
	if(control_mode!=prepare&&heart>500)
	{
	LED_HEAT();
	}
	if(heart>500)//��ʼ����������0.5����һ��
	{
		heart=0;
	}
	
		
	if(GMPRamp.count==(init_time_ms/2))//��ʼ����һ����������������
	{
		gyro_senddata();
	}	
		
		GMPitch_ControlLoop();
		GMYaw_ControlLoop();//��̨����
		
		gm_senddata(CAN2,-(int)GMYSpeedPID.output[NOW],-(int)GMPSpeedPID.output[NOW]);
		if(heart%8==0)
		{	
			status_check(&DBUS);//���̺�ң�ص��ж�
		}
		if(heart%32==0)
		{
			Monitor_ControlLoop();
		}
		
		if(heart%4==0)
		{
			shooting_ControlLoop();//�������
	    CMControlLoop();//��������
		}
}





      