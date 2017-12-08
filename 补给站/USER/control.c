#include "control.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "pid.h"
#include "can.h"
#include "tim.h"
#include "ramp.h"





float ppkp=0 ,ppki=0,ppkd=0,pskp=0,pski=0,pskd=0,realpangle=0,last_realpangle,normal_angle=0;
void GMPitch_ControlLoop(void)
{
	
	GMPitchEncoder.ecd_bias=0x410;//PITCH轴的矫正值
	GMPPositionPID.Kp = ppkp;
	GMPPositionPID.Ki = ppki;
	GMPPositionPID.Kd = ppkd;
					
	GMPSpeedPID.Kp = pskp;
	GMPSpeedPID.Ki = pski;
	GMPSpeedPID.Kd = pskd;
if(System_mode==prepare)
{
	ppkp=10;ppki=0;ppkd=0;pskp=1;pski=0;pskd=0;
	GMPPositionPID.realdata[0] = (GMPitchEncoder.ecd_value-GMPitchEncoder.ecd_bias)*360/8192*SystemInit_Ramp.calc(&SystemInit_Ramp);//
  GMPPositionPID.setdata=0;
}
else if(System_mode==normal)
{	ppkp=10;ppki=0;ppkd=0;pskp=5;pski=0;pskd=0;
	if((GMPRamp.count==Pitch_ms)&&(last_realpangle!=realpangle))
	{
	normal_angle+=last_realpangle;
	GMPRamp.resetcount(&GMPRamp);
	}
	
	//GMPPositionPID.realdata[0] = (GMPitchEncoder.ecd_value-GMPitchEncoder.ecd_bias)+normal_angle+realpangle*GMPRamp.calc(&GMPRamp);//*GMPRamp.calc(&GMPRamp);//
		GMPPositionPID.realdata[0] = (GMPitchEncoder.ecd_value-GMPitchEncoder.ecd_bias)*360/8192+normal_angle+realpangle*GMPRamp.calc(&GMPRamp);//*GMPRamp.calc(&GMPRamp);//

  GMPPositionPID.setdata=0;

}
  GMPPositionPID.test(&GMPPositionPID);
        
  GMPSpeedPID.setdata= GMPPositionPID.output[0];
  GMPSpeedPID.realdata[NOW]=GMPitchEncoder.filter_rate;//Y轴的角速度
  GMPSpeedPID.test(&GMPSpeedPID);
  
}


#define half_angle 180 //电机旋转角度


suppy_status_t Suplly_status=off,last_Suplly_status=off; //补给站补弹状态
//#define half_angle 4200 //电机旋转角度
uint16_t wait_time=0; //等待时间
#define WRITE_TIME 1000 //等待设定时间

uint8_t supply_time=0;
void Supply_ControlLoop(void)
{
last_Suplly_status=Suplly_status;
last_realpangle=realpangle;
	
			if(((key_l.status==0)&&(key_r.status==0)))
	{
		if((Suplly_status==start||Suplly_status==wait))//中途出补给站情况
		{
    wait_time=1000;
		}
		if(Suplly_status==off)
		{
			supply_time=0;//防止进入补给站多次加弹
			}
	}
	if(Suplly_status!=off)
	{
		key_l.status=0;
		key_r.status=0;
	}
	if(Suplly_status==end)
	{
		if(GMPRamp.count==Pitch_ms)
		{
			Suplly_status=off;
			supply_time=1;
		}
	}
	if(Suplly_status==wait)
	{
		wait_time++;
		if(wait_time>WRITE_TIME)
		{
			wait_time=0;
			Suplly_status=end;
			realpangle=-half_angle;
		}
	}
	if(Suplly_status==start)
	{
		if(GMPRamp.count==Pitch_ms)
		{
		Suplly_status=wait;
		}
	}	
	if(key_l.status&&key_r.status&&Suplly_status==off&&supply_time==0)
	{
		realpangle=half_angle;
		Suplly_status=start;
	}	
}












System_mode_t System_mode=prepare;
void control_loop(void)
{
		SystemInit_Ramp.calc(&SystemInit_Ramp);
	
		if(System_mode==normal)
	{
		Key_Scan();
		Supply_ControlLoop();
	}
	
		if(SystemInit_Ramp.count==init_time_ms)
	{
		System_mode=normal;
	}
		GMPitch_ControlLoop();
    gm_senddata(CAN1,(int)GMPSpeedPID.output[NOW]);
}






      