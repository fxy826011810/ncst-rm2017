#ifndef __PID_H
#define __PID_H
#include "can.h"


#define __PID_EXT extern

enum
{
	NOW = 0,
	LAST = 1,
};

typedef struct PID_TypeDef
{	float Kp;//����ϵ��
	float Ki;//����ϵ��
	float Kd;//΢��ϵ��
        
	float setdata;//�趨ֵ
	float setimax;//�趨�������ֵ
  float setomax;//������ֵ
//	float setmin;
	 
	float Pout;//�������
	float Iout;//�������
	float Dout;//΢�����
	float error[2];//ƫ��ֵ
	float realdata;//ʵ���ٶ�ֵ
	float output;//PID���
	void(*calc)(struct PID_TypeDef *pid);
	void(*reset)(struct PID_TypeDef *pid);

}PID_TypeDef;

void Bsp_Pid_Init(void);
void Pid_Calc(PID_TypeDef* pid);
void  Pid_Reset(PID_TypeDef* pid);


#define CMSpeedPID_default \
{\
200.0f,\
0.0f,\
0.0f,\
0.0f,\
1500,\
3000,\
0.0f,\
0.0f,\
0.0f,\
{0.0f,0.0f},\
0.0f,\
0.0f,\
&Pid_Calc,\
&Pid_Reset,\
}


__PID_EXT PID_TypeDef CM1SpeedPID;
__PID_EXT PID_TypeDef CM2SpeedPID;
__PID_EXT PID_TypeDef CM3SpeedPID;
__PID_EXT PID_TypeDef CM4SpeedPID;
#endif
