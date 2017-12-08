#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f4xx.h"
#define init_time_ms 2000//��ʼ��б��
#define friction_countmax 200//Ħ����б��

#define CmSPeedR_countmax 500//ң�������ٶ�б��
#define CmSPeedK_countmax 700//���������ٶ�б��
#define CMRotateR_countmax 500//���������ٶ�б��
#define CMRotateK_countmax 700//���������ٶ�б��
void GMPitch_ControlLoop(void);
void GMYaw_ControlLoop(void);
void CMControlLoop(void);
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed);
void shooting_ControlLoop(void);
void control_loop(void);
#endif
