#include "ramp.h"
#include "control.h"
#include "stm32f10x.h"
ramp_t SystemInit_Ramp=Ramp_Default;
ramp_t GMPRamp=Ramp_Default;
ramp_t GMYRamp=Ramp_Default;

void ramp_init( ramp_t *ramp,int16_t countmax)
{
ramp->count=0;
ramp->countmax=countmax;
}

void ramp_config(void)//ramp��ʼ��
{
				SystemInit_Ramp.setcountmax(&SystemInit_Ramp,init_time_ms);
        GMPRamp.setcountmax(&GMPRamp,Pitch_ms);
        GMYRamp.setcountmax(&GMYRamp,Yaw_ms);

				SystemInit_Ramp.resetcount(&SystemInit_Ramp);
	      GMPRamp.resetcount(&GMPRamp);
				GMYRamp.resetcount(&GMYRamp);

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

