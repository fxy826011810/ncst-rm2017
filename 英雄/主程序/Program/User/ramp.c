#include "ramp.h"
#include "control.h"
#include "stm32f4xx.h"
#include "Infantry.h"
ramp_t GMPRamp=Ramp_Default;
ramp_t GMYRamp=Ramp_Default;
ramp_t FRIRamp=Ramp_Default;
ramp_t FRIBRamp=Ramp_Default;

ramp_t POKONRamp=Ramp_Default;
ramp_t POKOFFRamp=Ramp_Default;

ramp_t SGONRamp=Ramp_Default;//震弹开始
ramp_t SGOFFRamp=Ramp_Default;//震蛋结束

ramp_t CMRotateR=Ramp_Default;
ramp_t CMRotateK=Ramp_Default;
ramp_t CMRotateS=Ramp_Default;

ramp_t CMXRampR=Ramp_Default;
ramp_t CMXRampK=Ramp_Default;
ramp_t CMXRampS=Ramp_Default;

ramp_t CMYRampR=Ramp_Default;
ramp_t CMYRampK=Ramp_Default;
ramp_t CMYRampS=Ramp_Default;


void ramp_init( ramp_t *ramp,int16_t countmax)
{
ramp->count=0;
ramp->countmax=countmax;
}

void ramp_config(void)//ramp初始化
{
        GMPRamp.setcountmax(&GMPRamp,init_time_ms);
        GMYRamp.setcountmax(&GMYRamp,init_time_ms);
				FRIRamp.setcountmax(&FRIRamp,friction_countmax);
				FRIBRamp.setcountmax(&FRIBRamp,friction_countmax);
	
				POKONRamp.setcountmax(&POKONRamp,pok_startcountmax);
				POKOFFRamp.setcountmax(&POKOFFRamp,pok_stopcountmax);
	
				SGONRamp.setcountmax(&SGONRamp,sg_startcountmax);
				SGOFFRamp.setcountmax(&SGOFFRamp,sg_stopcountmax);
				
				CMXRampR.setcountmax(&CMXRampR,CmSPeedR_countmax);
				CMXRampK.setcountmax(&CMXRampK,CmSPeedK_countmax);
	
				CMYRampR.setcountmax(&CMYRampR,CmSPeedR_countmax);
				CMYRampK.setcountmax(&CMYRampK,CmSPeedK_countmax);
	
				CMRotateR.setcountmax(&CMRotateR,CMRotateR_countmax);
				CMRotateK.setcountmax(&CMRotateK,CMRotateK_countmax);
	
				CMRotateS.setcountmax(&CMRotateS,CMStop_countmax);
				CMXRampS.setcountmax(&CMXRampS,CMStop_countmax);
				CMYRampS.setcountmax(&CMYRampS,CMStop_countmax);
	
	
	
				CMRotateR.resetcount(&CMRotateR);
				CMRotateK.resetcount(&CMRotateK);
	
				CMRotateS.resetcount(&CMRotateS);
				CMXRampS.resetcount(&CMXRampS);
				CMYRampS.resetcount(&CMYRampS);
				
        CMXRampR.resetcount(&CMXRampR);
				CMYRampR.resetcount(&CMYRampR);
				
				CMXRampK.resetcount(&CMXRampK);
				CMYRampK.resetcount(&CMYRampK);
				
	
	      GMPRamp.resetcount(&GMPRamp);
				GMYRamp.resetcount(&GMYRamp);
        FRIRamp.resetcount(&FRIRamp);
}
float ramp_calc( ramp_t* ramp,uint8_t mode)//ramp计算
{
  if(ramp->countmax>0)
  {
    ramp->count++;
    if(ramp->count>ramp->countmax)
				ramp->count=ramp->countmax;
		if(mode==1)
				ramp->out=((float)ramp->count/ramp->countmax);
		else
				ramp->out=0;
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

