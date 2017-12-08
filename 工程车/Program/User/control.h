#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f4xx.h"
#define init_time_ms 2000//初始化斜坡
#define friction_countmax 200//摩擦轮斜坡

#define CmSPeedR_countmax 500//遥控启动速度斜坡
#define CmSPeedK_countmax 700//键盘启动速度斜坡
#define CMRotateR_countmax 500//键盘启动速度斜坡
#define CMRotateK_countmax 700//键盘启动速度斜坡
void GMPitch_ControlLoop(void);
void GMYaw_ControlLoop(void);
void CMControlLoop(void);
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed);
void shooting_ControlLoop(void);
void control_loop(void);
#endif
