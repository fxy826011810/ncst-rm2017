#include "ramp.h"
#include "control.h"
#include "stm32f4xx.h"

ramp_t SystemInit_Ramp=Ramp_Default;

ramp_t CMRotateR=Ramp_Default;//转向斜坡
ramp_t CMRotateK=Ramp_Default;

ramp_t CMXRampR=Ramp_Default;//平移
ramp_t CMXRampK=Ramp_Default;

ramp_t CMYRampR=Ramp_Default;//前走
ramp_t CMYRampK=Ramp_Default;


void ramp_init( ramp_t *ramp,int16_t countmax)
{
ramp->count=0;
ramp->countmax=countmax;
}

void ramp_config(void)//ramp初始化
{

				SystemInit_Ramp.setcountmax(&SystemInit_Ramp,init_time_ms);
	
				CMXRampR.setcountmax(&CMXRampR,CmSPeedR_countmax);//设置初值
				CMXRampK.setcountmax(&CMXRampK,CmSPeedK_countmax);
	
				CMYRampR.setcountmax(&CMYRampR,CmSPeedR_countmax);
				CMYRampK.setcountmax(&CMYRampK,CmSPeedK_countmax);
	
				CMRotateR.setcountmax(&CMRotateR,CMRotateR_countmax);
				CMRotateK.setcountmax(&CMRotateK,CMRotateK_countmax);
	
	
				CMRotateR.resetcount(&CMRotateR);
				CMRotateK.resetcount(&CMRotateK);
	
        CMXRampR.resetcount(&CMXRampR);
				CMYRampR.resetcount(&CMYRampR);
				
				CMXRampK.resetcount(&CMXRampK);
				CMYRampK.resetcount(&CMYRampK);

}
float ramp_calc( ramp_t* ramp)//ramp计算
{
  if(ramp->countmax>0)
  {
    ramp->count++;
    if(ramp->count>ramp->countmax)
      ramp->count=ramp->countmax;
    ramp->out=((float)ramp->count/ramp->countmax);
    return ramp->out;
  }
  return 0.0f;
}

void ramp_setcount( ramp_t* ramp,int16_t count)//设置初始值
{
ramp->count=count;
}

void ramp_resetcount( ramp_t* ramp)//重置count
{
  ramp->count=0;
}

void ramp_setcountmax( ramp_t* ramp,int16_t countmax)//设置斜坡最大值
{
  ramp->countmax=countmax;
}

uint8_t ramp_IsOverflow( ramp_t* ramp)//判断斜坡是否超量
{
  return ramp->count>ramp->countmax;
}

