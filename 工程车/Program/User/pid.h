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
{	float Kp;//比例系数
	float Ki;//积分系数
	float Kd;//微分系数
        
	float setdata;//设定值
	float setimax;
  float setmax;
	float setmin;
	 
	float Pout;//比例输出
	float Iout;//积分输出
	float Dout;//微分输出
	float error[3];//偏差值
	float realdata[3];//实际速度值
	float output[3];//位置PID输出
	void(*test)(struct PID_TypeDef *pid);
	void(*reset)(struct PID_TypeDef *pid);

}PID_TypeDef;

void Bsp_Pid_Init(void);
void Pid_Test(PID_TypeDef* pid);
void  Pid_Reset(PID_TypeDef* pid);

#define CM1SpeedPID_default \
{\
3.0f,\
0.16f,\
10.0f,\
0,\
10000,\
10000,\
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

#define CM2SpeedPID_default \
{\
3.0f,\
0.16f,\
10.0f,\
0,\
10000,\
10000,\
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

#define CM3SpeedPID_default \
{\
3.0f,\
0.16f,\
10.0f,\
0,\
10000,\
10000,\
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

#define CM4SpeedPID_default \
{\
3.0f,\
0.16f,\
10.0f,\
0,\
10000,\
10000,\
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
__PID_EXT PID_TypeDef CM1SpeedPID ;
__PID_EXT PID_TypeDef CM2SpeedPID ;
__PID_EXT PID_TypeDef CM3SpeedPID ;
__PID_EXT PID_TypeDef CM4SpeedPID ;

#endif
