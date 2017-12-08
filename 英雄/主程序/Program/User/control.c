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
#include "spi.h"
#include "key.h"
#include "Infantry.h"
#include "calc.h"


#define ppkp 11
#define ppki 0
#define ppkd 0
#define pskp 13
#define pski 0
#define pskd 0
void GMPitch_ControlLoop(void)
{

	GMPitchEncoder.ecd_bias=GMPitchAngle_Default;//PITCH轴的矫正值
	
	GMPPositionPID.Kp = ppkp;
	GMPPositionPID.Ki = ppki;
	GMPPositionPID.Kd = ppkd;
					
	GMPSpeedPID.Kp = pskp;
	GMPSpeedPID.Ki = pski;
	GMPSpeedPID.Kd = pskd;


	GMPPositionPID.realdata[0]=-GMPitchEncoder.ecd_angle*GMPRamp.calc(&GMPRamp,1);//（电机的PITCH角度-矫正值）*斜坡函数
  GMPPositionPID.setdata=GMRemoteSend.pitch_angle;
  GMPPositionPID.test(&GMPPositionPID);
        
  GMPSpeedPID.setdata= -GMPPositionPID.output[0];
 // GMPSpeedPID.realdata[NOW]=-IMU.gy;//Y轴的角速度
	GMPSpeedPID.realdata[NOW]=-GYROY.F;//Y轴的角速度
  GMPSpeedPID.test(&GMPSpeedPID);
 
}


void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed)
{
    float Buffer[4], Param, MaxSpeed;
    uint8_t index;
                                        //         |
                                        //	     _____
    Buffer[0] = Vx - Vy + Omega;        //    2||     ||0  电流正逆时针转EC60
    Buffer[1] = Vx + Vy + Omega;        //      |     |
		Buffer[2] = -Vx + Vy + Omega;        //     |     |
		Buffer[3] = -Vx - Vy + Omega;        //   1||     ||3
                                        //       -----

    //限幅
    for(index = 0, MaxSpeed = 0; index < 4; index++)
    {
        if((Buffer[index] > 0 ? Buffer[index] : -Buffer[index]) > MaxSpeed)
        {
            MaxSpeed = (Buffer[index] > 0 ? Buffer[index] : -Buffer[index]);
        }
    }
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
}



void CMControlLoop(void)
{
static int16_t chassisspeed[4];
	if(control_mode==prepare||control_mode==stop||control_mode==noremote)//底盘不跟随代码
	{
		MecanumCalculate(0,0,0,chassisspeed);
	}
	else 
	{
		if(control_mode!=prepare)
		{
			if(ChassisSend.X==0)
			{
				CMXRampR.resetcount(&CMXRampR);
			}
			if(ChassisSend.Y==0)
			{
				CMYRampR.resetcount(&CMYRampR);
			}
			if(ChassisSend.rotate==0)
			{
				CMRotateS.resetcount(&CMRotateS);
			}
				MecanumCalculate(ChassisSend.X*CMXRampR.calc(&CMXRampR,1),ChassisSend.Y*CMYRampR.calc(&CMYRampR,1),ChassisSend.rotate*CMRotateR.calc(&CMRotateR,1),chassisspeed);
		}
		else
		{
			ChassisSend.rotate=0;
			ChassisSend.X=0;
			ChassisSend.Y=0;
		}
	}
			

  CM1SpeedPID.setdata=chassisspeed[0];//写入遥控器返回值
  CM2SpeedPID.setdata=chassisspeed[1];
	CM3SpeedPID.setdata=chassisspeed[2];
	CM4SpeedPID.setdata=chassisspeed[3];
		
	CM1SpeedPID.realdata[0]=CM1Encoder.filter_rate;//要用ec60把速度改为计算出的编码速度值
	CM2SpeedPID.realdata[0]=CM2Encoder.filter_rate;
	CM3SpeedPID.realdata[0]=CM3Encoder.filter_rate;
	CM4SpeedPID.realdata[0]=CM4Encoder.filter_rate;
	
	CM1SpeedPID.test(&CM1SpeedPID);
	CM2SpeedPID.test(&CM2SpeedPID);
	CM3SpeedPID.test(&CM3SpeedPID);
	CM4SpeedPID.test(&CM4SpeedPID);
	
}

extern GUNMode_t POKStatus,LastPOKStatus;//拨弹电机状态
extern GUNMode_t FRIStatus,SG_Status;//摩擦轮状态
extern uint8_t	POKKeep;
void ShakeGaoerfu_ControlLoop(void)
{
	if(SG_Status==SGOFF)
	{
		SGOFFRamp.calc(&SGOFFRamp,0);
		TIM8->CCR4=200;
		if(SGOFFRamp.count==sg_startcountmax)
		{
			SGOFFRamp.resetcount(&SGOFFRamp);
			SG_Status=SGON;
		}
	}
		if(SG_Status==SGON)
	{
		SGONRamp.calc(&SGONRamp,0);
		TIM8->CCR4=160;
		if(SGONRamp.count==sg_stopcountmax)
		{
			SGONRamp.resetcount(&SGONRamp);
			SG_Status=SGOFF;
		}
	}
}
void shooting_ControlLoop(void)
{
	
		if(FRIStatus==FRION)//摩擦轮状态开
	{
		ShakeGaoerfu_ControlLoop();
		laser_senddata(CAN1,1);
		FRISpeed_Set(1000+FRISpeed*FRIRamp.calc(&FRIRamp,1),1000+FRIBSpeed*FRIBRamp.calc(&FRIBRamp,1));//摩擦轮启动
	}
	 else if(FRIStatus==FRIOFF)//摩擦轮关闭
	{
		laser_senddata(CAN1,0);
		FRISpeed_Set(1000,1000);
		FRIRamp.resetcount(&FRIRamp);
		POKStatus=POKOFF;
	}
	
	if(POKStatus==POKON)//拨弹电机状态开
  {
		POK_ON;
		POKOFFRamp.resetcount(&POKOFFRamp);
		POKONRamp.calc(&POKONRamp,0);
		if(POKONRamp.count==pok_startcountmax)
		{
			POKOFFRamp.resetcount(&POKOFFRamp);
			POKStatus=POKOFF;
		}
  }
	else if(POKStatus==POKOFF)
	{
		POK_OFF;
		POKOFFRamp.calc(&POKOFFRamp,0);
		if(POKKeep&&POKOFFRamp.count==pok_stopcountmax)
		{
			POKONRamp.resetcount(&POKONRamp);
			POKStatus=POKON;
		}
  }
}



Stepper_t stp1up={0,0,0},stp1down={0,0,0},stp2up={0,0,0},stp2down={0,0,0},stp3up={0,0,0},stp3down={0,0,0},stp1autoon={0,0,0},stp2autoon={0,0,0},stp3autoon={0,0,0},stp1autooff={0,0,0},stp2autooff={0,0,0},stp3autooff={0,0,0};
void MechArm_ControlLoop(DBUS_TypeDef *dbus)
{
	if(dbus->rc.s1==3)
		{
			if(dbus->rc.s2==3)
		{
			stp1autooff.lock=0;
			stp2autooff.lock=0;
			stp3autooff.lock=0;
			
			stp1autoon.lock=0;
			stp2autoon.lock=0;
			stp3autoon.lock=0;
			
			/*---------------*/
			/*-------上下--------*/
			if(dbus->rc.ch3<-330)
			{
				stp1up.lock=0;
			}
			else if(dbus->rc.ch3>330)
			{
				stp1down.lock=0;
			}
			
			if(stp1up.lock==0)
			{
				stp1up.ch=dbus->rc.ch3;
			}
			else
			{
				stp1up.ch=0;
			}
			if(stp1down.lock==0)
			{
				stp1down.ch=dbus->rc.ch3;
			}
			else
			{
				stp1down.ch=0;
			}
			
			
			if(stp1up.ch>330)
			{
				CMPWM1=390;//修改占空比
				en1_on;
				if(sw1.status==0)
				{
					if(stp1up.check)
					{
						stp1up.check=0;
						stp1up.lock=1;
					}
					cw1_on;
				}
				else if(sw1.status)
				{
					cw1_off;
					stp1up.check=1;
				}
			}
					if(stp1down.ch<-330)
			{
				CMPWM1=390;//修改占空比
				en1_on;
				if(sw2.status==0)
				{
					if(stp1down.check)
					{
						stp1down.check=0;
						stp1down.lock=1;
					}
					cw1_off;
				}
				else if(sw2.status)
				{
					cw1_on;
					stp1down.check=1;
				}
			}
			if((stp1down.ch<330&&stp1down.ch>-330)||(stp1up.ch<330&&stp1up.ch>-330))
			{
				CMPWM1=0;
			}
			
	/*------------爪子-----------------*/
			if(dbus->rc.ch0<-330)
			{
				stp2up.lock=0;
			}
			else if(dbus->rc.ch0>330)
			{
				stp2down.lock=0;
			}
			
			if(stp2up.lock==0)
			{
				stp2up.ch=dbus->rc.ch0;
			}
			else
			{
				stp2up.ch=0;
			}
			if(stp2down.lock==0)
			{
				stp2down.ch=dbus->rc.ch0;
			}
			else
			{
				stp2down.ch=0;
			}
			
			if(stp2up.ch>330)
			{
				CMPWM2=390;//修改占空比
				en2_on;
				if(sw3.status==0)
				{
					if(stp2up.check)
					{
						stp2up.check=0;
						stp2up.lock=1;
					}
					cw2_on;
				}
				else if(sw3.status)
				{
					cw2_off;
					stp2up.check=1;
				}
			}
					if(stp2down.ch<-330)
			{
				CMPWM2=390;//修改占空比
				en2_on;
				if(sw4.status==0)
				{
					if(stp2down.check)
					{
						stp2down.check=0;
						stp2down.lock=1;
					}
					cw2_off;
				}
				else if(sw4.status)
				{
					cw2_on;
					stp2down.check=1;
				}
			}
			if((stp2down.ch<330&&stp2down.ch>-330)||(stp2up.ch<330&&stp2up.ch>-330))
			{
				CMPWM2=0;
			}
	/*-----------前后--------------------*/
				if(dbus->rc.ch1<-330)
			{
				stp3up.lock=0;
			}
			else if(dbus->rc.ch1>330)
			{
				stp3down.lock=0;
			}
			
			if(stp3up.lock==0)
			{
				stp3up.ch=dbus->rc.ch1;
			}
			else
			{
				stp3up.ch=0;
			}
			if(stp3down.lock==0)
			{
				stp3down.ch=dbus->rc.ch1;
			}
			else
			{
				stp3down.ch=0;
			}
			
			
			if(stp3up.ch>330)
			{
				CMPWM3=390;//修改占空比
				en3_on;
				if(sw5.status==0)
				{
					if(stp3up.check)
					{
						stp3up.check=0;
						stp3up.lock=1;
					}
					cw3_on;
				}
				else if(sw5.status)
				{
					cw3_off;
					stp3up.check=1;
				}
			}/////////
					if(stp3down.ch<-330)
			{
				CMPWM3=390;//修改占空比
				en3_on;
				if(sw6.status==0)
				{
					if(stp3down.check)
					{
						stp3down.check=0;
						stp3down.lock=1;
					}
					cw3_off;
				}
				else if(sw6.status)
				{
					cw3_on;
					stp3down.check=1;
				}
			}
			if((stp3down.ch<330&&stp3down.ch>-330)||(stp3up.ch<330&&stp3up.ch>-330))
			{
				CMPWM3=0;
			}	
		}
		if(dbus->rc.s2==1)//全张
		{
			stp1autooff.lock=0;
			stp2autooff.lock=0;
			stp3autooff.lock=0;
			
			en1_on;
			en2_on;
			en3_on;
			if(sw1.status==0)
			{
				if(stp1autoon.lock==0)
				{
					CMPWM1=390;
					cw1_on;
				}
				else
				{
					CMPWM1=0;
				}
			}
			else
			{
				cw1_off;
				stp1autoon.lock=1;
			}
//			
//			
//			if(sw3.status==0)
//			{
//				if(stp2autoon.lock==0)
//				{
//					CMPWM2=390;
//					cw2_on;
//				}
//				else
//				{
//					CMPWM2=0;
//				}
//			}
//			else
//			{
//				cw2_off;
//				stp2autoon.lock=1;
//			}
//			
//			
			if(sw5.status==0)
			{
				if(stp3autoon.lock==0)
				{
					CMPWM3=390;
					cw3_on;
				}
				else
				{
					CMPWM3=0;
				}
			}
			else
			{
				cw3_off;
				stp3autoon.lock=1;
			}

		}
		if(dbus->rc.s2==2)//全收
		{
			stp1autoon.lock=0;
			stp2autoon.lock=0;
			stp3autoon.lock=0;
			en1_on;
			en2_on;
			en3_on;
			if(sw2.status==0)
			{
				if(stp1autooff.lock==0)
				{
					CMPWM1=390;
					cw1_off;
				}
				else
				{
					CMPWM1=0;
				}
			}
			else
			{
				cw1_on;
				stp1autooff.lock=1;
			}
			
			
//			if(sw4.status==0)
//			{
//				if(stp2autooff.lock==0)
//				{
//					CMPWM2=390;
//					cw2_off;
//				}
//				else
//				{
//					CMPWM2=0;
//				}
//			}
//			else
//			{
//				cw2_on;
//				stp2autooff.lock=1;
//			}
//			
//			
			if(sw6.status==0)
			{
				if(stp3autooff.lock==0)
				{
					CMPWM3=390;
					cw3_off;
				}
				else
				{
					CMPWM3=0;
				}
			}
			else
			{
				cw3_on;
				stp3autooff.lock=1;
			}
		}
	}	
}

 uint16_t heart=0;//心跳灯
void control_loop(void)
{
	heart++;
  if((control_mode==remote||control_mode==keyboard)&&heart>=500)
	{
	LED_HEAT();
	heart=0;	
	}

	Switch_Scan();//限位状态扫描
	if((control_mode==remote||control_mode==keyboard))
	{
	//ShakeGaoerfu_ControlLoop();
	}
	Control_Mode_Change(&DBUS);//状态监测
	if(heart%2==0)//2ms一次任务
	{
		GMPitch_ControlLoop();//pitch轴任务
		gm_senddata(CAN1,-(int)GMPSpeedPID.output[NOW],0);
	}
	if(heart%4==0)//4ms一次任务
	{
		CMControlLoop();//底盘任务
		cm_senddata(CAN1,(int)CM1SpeedPID.output[NOW],(int)CM2SpeedPID.output[NOW],(int)CM3SpeedPID.output[NOW],(int)CM4SpeedPID.output[NOW]);
		
	}
	if(heart%3==0)//5ms一次任务
	{
		gyro_senddata(CAN1);//陀螺仪角速度读取任务
		shooting_ControlLoop();
	}	
	 
}





      