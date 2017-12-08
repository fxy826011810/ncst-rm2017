#include "ramp.h"
#include "control.h"
#include "stm32f4xx.h"

ramp_t GMPRamp=Ramp_Default;
ramp_t GMYRamp=Ramp_Default;
ramp_t FRIRamp=Ramp_Default;

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

void ramp_config(void)//ramp��ʼ��
{
        GMPRamp.setcountmax(&GMPRamp,init_time_ms);
        GMYRamp.setcountmax(&GMYRamp,init_time_ms);
				FRIRamp.setcountmax(&FRIRamp,friction_countmax);

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
	
        CMXRampR.resetcount(&CMXRampR);
				CMYRampR.resetcount(&CMYRampR);
				
				CMXRampK.resetcount(&CMXRampK);
				CMYRampK.resetcount(&CMYRampK);
	
	      GMPRamp.resetcount(&GMPRamp);
				GMYRamp.resetcount(&GMYRamp);
        FRIRamp.resetcount(&FRIRamp);
}
float ramp_calc( ramp_t* ramp)//ramp����
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

void ramp_setcount( ramp_t* ramp,int16_t count)//���ó�ʼֵ
{
ramp->count=count;
}

void ramp_resetcount( ramp_t* ramp)//����count
{
  ramp->count=0;
}

void ramp_setcountmax( ramp_t* ramp,int16_t countmax)//����б�����ֵ
{
  ramp->countmax=countmax;
}

uint8_t ramp_IsOverflow( ramp_t* ramp)//�ж�б���Ƿ���
{
  return ramp->count>ramp->countmax;
}

