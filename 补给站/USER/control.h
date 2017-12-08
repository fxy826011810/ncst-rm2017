#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f10x.h"
#define init_time_ms 6000//��ʼ��б��
#define Pitch_ms 1000//��ʼ��б��
#define Yaw_ms 2000//��ʼ��б��
typedef enum System_mode_t
{
	prepare=2,
  normal=1,
	stop=3,
}System_mode_t;//����ģʽ

typedef enum suppy_status_t
{
	off=0,
	start=1,
	wait=2,
	end=3,
	
}suppy_status_t;//����վ״̬
extern System_mode_t System_mode;


void GMPitch_ControlLoop(void);
void GMYaw_ControlLoop(void);
void Supply_ControlLoop(void);
void control_loop(void);
#endif
