#include "control.h"
#include "stm32f10x.h"
#include "judge.h"
#include "gpio.h"
#include "pid.h"
#include "can.h"
#include "tim.h"
#include "ramp.h"






void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Buffer)
{
  // float Buffer[4], ParamS, MaxSpeed;
	float ParamS, MaxSpeed;
   uint8_t index;
                                        //       |
                                        //	   _____
   Buffer[0] = -Vx -Vy + Omega;         //  3||     || 1  电流正逆时针转EC60
   Buffer[1] = Vx - Vy + Omega;         //    |     |
   Buffer[2] = -Vx + Vy + Omega;        //    |     |
   Buffer[3] = Vx + Vy + Omega;        //  4 ||     || 2
                                        //     -----

    //限幅
//    for(index = 0, MaxSpeed = 0; index < 4; index++)
//    {
//        if((Buffer[index] > 0 ? Buffer[index] : -Buffer[index]) > MaxSpeed)
//        {
//            MaxSpeed = (Buffer[index] > 0 ? Buffer[index] : -Buffer[index]);
//        }
//    }
//    if(MaxWheelSpeed < MaxSpeed)
//    {
//        ParamS = (float)MaxWheelSpeed / MaxSpeed;
//        Speed[0] = Buffer[0] * ParamS;
//        Speed[1] = Buffer[1] * ParamS;
//        Speed[2] = Buffer[2] * ParamS;
//        Speed[3] = Buffer[3] * ParamS; 
//    }
//    else
//    {
//        Speed[0] = Buffer[0];
//        Speed[1] = Buffer[1];
//        Speed[2] = Buffer[2];
//        Speed[3] = Buffer[3];
//    }

	}

extern uint16_t MaxA;
uint16_t PowerMax,ABSSpeed[4];
int16_t ALimit[4];
	
float cmx=10,cmy=10,cmr=10;//速度接口提供给HWControlLoop使用
extern uint8_t judge_status;
void CMControlLoop(void)
{
static int16_t chassisspeed[4];


	MecanumCalculate(cmx,cmy,cmr,chassisspeed);
	
	CM1SpeedPID.setdata=chassisspeed[0];//写入遥控器返回值
  CM2SpeedPID.setdata=chassisspeed[1];
	CM3SpeedPID.setdata=chassisspeed[2];
	CM4SpeedPID.setdata=chassisspeed[3];
	
	CM1SpeedPID.realdata=CM1Encoder.filter_rate;//要用ec60把速度改为计算出的编码速度值
	CM2SpeedPID.realdata=CM2Encoder.filter_rate;
	CM3SpeedPID.realdata=CM3Encoder.filter_rate;
	CM4SpeedPID.realdata=CM4Encoder.filter_rate;
	
	CM1SpeedPID.calc(&CM1SpeedPID);
	CM2SpeedPID.calc(&CM2SpeedPID);
	CM3SpeedPID.calc(&CM3SpeedPID);
	CM4SpeedPID.calc(&CM4SpeedPID);
	
	
	ABSSpeed[0] = (CM1SpeedPID.output > 0 ? CM1SpeedPID.output : -CM1SpeedPID.output);
  ABSSpeed[1] = (CM2SpeedPID.output > 0 ? CM2SpeedPID.output : -CM2SpeedPID.output);
  ABSSpeed[2] = (CM3SpeedPID.output > 0 ? CM3SpeedPID.output : -CM3SpeedPID.output);
  ABSSpeed[3] = (CM4SpeedPID.output > 0 ? CM4SpeedPID.output : -CM4SpeedPID.output);
	
	PowerMax=ABSSpeed[0]+ABSSpeed[1]+ABSSpeed[2]+ABSSpeed[3];
	if(judge_status==ENABLE)
	{
			if(MaxA<PowerMax)
		{
			ALimit[0]=CM1SpeedPID.output*MaxA/PowerMax;
			ALimit[1]=CM2SpeedPID.output*MaxA/PowerMax;
			ALimit[2]=CM3SpeedPID.output*MaxA/PowerMax;
			ALimit[3]=CM4SpeedPID.output*MaxA/PowerMax;
		}
		else
		{
			ALimit[0]=CM1SpeedPID.output;
			ALimit[1]=CM2SpeedPID.output;
			ALimit[2]=CM3SpeedPID.output;
			ALimit[3]=CM4SpeedPID.output;
		}
	}
	else if(judge_status==DISABLE)
	{
			if(MaxA<PowerMax)
		{
			ALimit[0]=CM1SpeedPID.output*6000/PowerMax;
			ALimit[1]=CM2SpeedPID.output*6000/PowerMax;
			ALimit[2]=CM3SpeedPID.output*6000/PowerMax;
			ALimit[3]=CM4SpeedPID.output*6000/PowerMax;
		}
		else
		{
			ALimit[0]=CM1SpeedPID.output;
			ALimit[1]=CM2SpeedPID.output;
			ALimit[2]=CM3SpeedPID.output;
			ALimit[3]=CM4SpeedPID.output;
		}
	}
	
	
	cm_senddata(CAN1,ALimit[0],ALimit[1],ALimit[2],ALimit[3]);
}
uint8_t sp=26,rsp=23;
void HWControlLoop(void)
{
	
	if(hw1.status&&hw2.status)
	{
		cmx=0;
		cmy=-sp;
		cmr=rsp;
	}
  else if(hw2.status&&hw3.status)
	{
		cmx=-sp/2;
		cmy=-sp/2;
		cmr=rsp;
	}
	else if(hw3.status&&hw4.status)
	{
		cmx=-sp;
		cmy=0;
		cmr=rsp;
	}
	else if(hw4.status&&hw5.status)
	{
		cmx=-sp/2;
		cmy=sp/2;
		cmr=rsp;
	}
	else if(hw5.status&&hw6.status)
	{
		cmx=0;
		cmy=sp;
		cmr=rsp;
	}
	else if(hw6.status&&hw7.status)
	{
		cmx=sp/2;
		cmy=sp/2;
		cmr=rsp;
	}
	else if(hw7.status&&hw8.status)
	{
		cmx=sp;
		cmy=0;
		cmr=rsp;
	}
	else if(hw8.status&&hw1.status)
	{
		cmx=sp/2;
		cmy=-sp/2;
		cmr=rsp;
	}
}


System_mode_t System_mode=prepare;
uint16_t time=0,Monitor_Rate=0;
extern uint16_t JudgeFrameCounter;
extern uint16_t JudgeFrameRate;
uint8_t judge_status=ENABLE;
void Control_Loop(void)
{
	time++;
	Monitor_Rate++;
	SystemInit_Ramp.calc(&SystemInit_Ramp);
	whole_Scan();//红外扫描
	if(System_mode==normal)
	{
		//HWControlLoop();//红外控制函数
		if(time>500)
		{
		LED_HEAT();
		time=0;
		}
		if(time%5)
		{
		CMControlLoop();//底盘函数
		}
		
	}
	
	if(Monitor_Rate%1000==0)
	{
		JudgeFrameRate=JudgeFrameCounter;
		JudgeFrameCounter=0;
		if(JudgeFrameRate<10)
		{
			judge_status=DISABLE;
		}
		else
		{
			judge_status=ENABLE;
		}
	}
	if(Monitor_Rate==2000)
	{
		Monitor_Rate=0;
	}
	if(SystemInit_Ramp.count==init_time_ms&&System_mode==prepare)
	{
		System_mode=normal;
	}
	
}     
