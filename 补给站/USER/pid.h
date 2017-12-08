#ifndef __PID_H
#define __PID_H
#include "can.h"


#define __PID_EXT extern

enum
{
	NOW = 0,
	LAST = 1,
	LLAST = 2,
  OUTPUT = 3,

};

typedef struct PID_TypeDef
{	float Kp;//����ϵ��
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
	void(*test)(struct PID_TypeDef *pid);
	void(*reset)(struct PID_TypeDef *pid);

}PID_TypeDef;

void Bsp_Pid_Init(void);
void Pid_Test(PID_TypeDef* pid);
void  Pid_Reset(PID_TypeDef* pid);



#define GMPPositionPID_default \
{\
0.0f,\
0.0f,\
0.0f,\
0,\
1000,\
2000,\
0,\
0,\
0,\
0,\
{0,0,0},\
{0,0,0},\
{0,0,0},\
&Pid_Test,\
&Pid_Reset,\
}

#define GMPSpeedPID_default \
{\
0.0f,\
0.0f,\
0.0f,\
0,\
1000,\
4900,\
0,\
0,\
0,\
0,\
{0,0,0},\
{0,0,0},\
{0,0,0},\
&Pid_Test,\
&Pid_Reset,\
}

#define GMYPositionPID_default \
{\
0.0f,\
0.0f,\
0.0f,\
0,\
1000,\
4900,\
0,\
0,\
0,\
0,\
{0,0,0},\
{0,0,0},\
{0,0,0},\
&Pid_Test,\
&Pid_Reset,\
}

#define GMYSpeedPID_default \
{\
0.0f,\
0.0f,\
0.0f,\
0,\
1000,\
4900,\
0,\
0,\
0,\
0,\
{0,0,0},\
{0,0,0},\
{0,0,0},\
&Pid_Test,\
&Pid_Reset,\
}



__PID_EXT PID_TypeDef GMPPositionPID ;
__PID_EXT PID_TypeDef GMPSpeedPID ;
__PID_EXT PID_TypeDef GMYPositionPID ;
__PID_EXT PID_TypeDef GMYSpeedPID ;

#endif
