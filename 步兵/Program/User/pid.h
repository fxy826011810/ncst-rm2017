#ifndef __PID_H
#define __PID_H
#include "Infantry.h"
#include "can.h"


#define __PID_EXT extern

enum //ʱ��״̬
{
	NOW = 0,
	LAST = 1,
	LLAST = 2,
};

typedef struct PID_TypeDef
{	
	float Kp;//����ϵ��
	float Ki;//����ϵ��
	float Kd;//΢��ϵ��
	
	float setdata;//�趨ֵ
	float setimax;
  float setmax;
	float setmin;
	
	float Pout;//�������
	float Iout;//�������
	float Dout;//΢�����
	float error[3];//ƫ��ֵ
	float realdata[3];//ʵ���ٶ�ֵ
	float output[3];//λ��PID���
	
	void(*test)(struct PID_TypeDef *pid);//pid���㺯��
	void(*reset)(struct PID_TypeDef *pid);//pid���ú���
}PID_TypeDef;

void Bsp_Pid_Init(void);
void Pid_Test(PID_TypeDef* pid);
void  Pid_Reset(PID_TypeDef* pid);




__PID_EXT PID_TypeDef GMPPositionPID ;
__PID_EXT PID_TypeDef GMPSpeedPID ;
__PID_EXT PID_TypeDef GMYPositionPID ;
__PID_EXT PID_TypeDef GMYSpeedPID ;
__PID_EXT PID_TypeDef CM1SpeedPID ;
__PID_EXT PID_TypeDef CM2SpeedPID ;
__PID_EXT PID_TypeDef CM3SpeedPID ;
__PID_EXT PID_TypeDef CM4SpeedPID ;
__PID_EXT PID_TypeDef CMRotatePID ;
__PID_EXT PID_TypeDef POKSpeedPID ;
__PID_EXT PID_TypeDef PowerPID;
#endif
