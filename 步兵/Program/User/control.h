#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f4xx.h"
#include "Infantry.h"

void GMPitch_ControlLoop(void);
void GMYaw_ControlLoop(void);
void CMControlLoop(void);
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed);
void shooting_ControlLoop(void);
void control_loop(void);
#endif
