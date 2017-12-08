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
{	float Kp;//比例系数
	float Ki;//积分系数
	float Kd;//微分系数
        
	float setdata;//设定值
	float setimax;//设定积分最大值
  float setomax;//输出最大值
//	float setmin;
	 
	float Pout;//比例输出
	float Iout;//积分输出
	float Dout;//微分输出
	float error[2];//偏差值
	float realdata;//实际速度值
	float output;//PID输出
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
