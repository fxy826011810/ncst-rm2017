#include "ramp.h"
#include "control.h"
#include "stm32f10x.h"
ramp_t SystemInit_Ramp=Ramp_Default;

void ramp_init( ramp_t *ramp,int16_t countmax)
{
ramp->count=0;
ramp->countmax=countmax;
}

void ramp_config(void)//ramp初始化
{
				SystemInit_Ramp.setcountmax(&SystemInit_Ramp,init_time_ms);
				SystemInit_Ramp.resetcount(&SystemInit_Ramp);
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

