#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f10x.h"

#define init_time_ms 6000//初始化斜坡
#define MaxWheelSpeed 50
#define MaxWheelPower 75
//#define speed 15

typedef enum System_mode_t
{
	prepare=2,//系统启动状态
	checkon=0,//触发光电开关后
  normal=1,//平时状态
	stop=3,//即被轰炸或者系统出现故障
}System_mode_t;//控制模式

extern System_mode_t System_mode;

void HWControlLoop(void);
void CMControlLoop(void);
void Control_Loop(void);
#endif
