#include "control.h"
#include "stm32f4xx.h"
#include "gpio.h"
#include "pid.h"
#include "dbus.h"
#include "can.h"
#include "tim.h"
#include "ramp.h"



#define MaxWheelSpeed 6270
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed)//麦克纳木轮速度解算
{
    float Buffer[4], Param, MaxSpeed;
    uint8_t index;
                                        //        |
                                        //	    _____
    Buffer[0] = Vx - Vy + Omega;        //   2||     || 1  电流正逆时针转EC60
    Buffer[1] = Vx + Vy + Omega;        //     |     |
		Buffer[2] = -Vx + Vy + Omega;        //     |     |
		Buffer[3] = -Vx - Vy + Omega;        //  3 ||     || 4
                                        //      -----

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


int16_t chassisspeed[4];

void CMControlLoop(void)//底盘控制
{

	if(control_mode==prepare)//底盘不跟随代码
	{
		MecanumCalculate(0,0,0,chassisspeed);
	}
	else 
	{
		if(control_mode==remote)
		{
			
			if(ChassisSend.X==0)
				CMXRampR.resetcount(&CMXRampR);
			if(ChassisSend.Y==0)
				CMYRampR.resetcount(&CMYRampR);
			MecanumCalculate(ChassisSend.X*CMXRampR.calc(&CMXRampR),ChassisSend.Y*CMYRampR.calc(&CMYRampR),ChassisSend.rotate*CMRotateR.calc(&CMRotateR),chassisspeed);
		}
		else if(control_mode==keyboard)
		{
			
			if(ChassisSend.X==0)
			{
				CMXRampK.resetcount(&CMXRampK);
				CMXRampR.resetcount(&CMXRampR);
			}
			if(ChassisSend.Y==0)
			{
				CMYRampK.resetcount(&CMYRampK);
				CMYRampR.resetcount(&CMYRampR);
			}
			if(ChassisSend.rotate==0)
			{
			CMRotateR.resetcount(&CMRotateR);
			CMRotateK.resetcount(&CMRotateK);
			}
			MecanumCalculate(ChassisSend.X*CMXRampK.calc(&CMXRampK),ChassisSend.Y*CMYRampK.calc(&CMYRampK),ChassisSend.rotate,chassisspeed);
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
		
	CM1SpeedPID.realdata[0]=cm1_rec.RollSpeed;//要用ec60把速度改为计算出的编码速度值
	CM2SpeedPID.realdata[0]=cm2_rec.RollSpeed;
	CM3SpeedPID.realdata[0]=cm3_rec.RollSpeed;
	CM4SpeedPID.realdata[0]=cm4_rec.RollSpeed;
	
	CM1SpeedPID.test(&CM1SpeedPID);
	CM2SpeedPID.test(&CM2SpeedPID);
	CM3SpeedPID.test(&CM3SpeedPID);
	CM4SpeedPID.test(&CM4SpeedPID);
	cm_senddata(CAN1,(int)CM1SpeedPID.output[NOW],(int)CM2SpeedPID.output[NOW],(int)CM3SpeedPID.output[NOW],(int)CM4SpeedPID.output[NOW]);
}

uint16_t heart=0;//心跳灯

void control_loop(void)
{
	
	SystemInit_Ramp.calc(&SystemInit_Ramp);
		
	heart++;
	if(control_mode!=prepare&&heart==1000)
	{
    LED_HEAT();
		heart=0;
	}
	status_check(&DBUS);//键盘和遥控的判断
	CMControlLoop();//底盘任务

}





      