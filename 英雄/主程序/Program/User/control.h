#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f4xx.h"
#include "dbus.h"

#define GPIOcw1 GPIOA
#define GPIOcw2	GPIOI
#define GPIOcw3	GPIOD

#define GPIO_Pin_cw1 	GPIO_Pin_2
#define GPIO_Pin_cw2	GPIO_Pin_6
#define GPIO_Pin_cw3	GPIO_Pin_14

#define GPIOen1 GPIOA
#define GPIOen2	GPIOI
#define GPIOen3	GPIOD

#define GPIO_Pin_en1 	GPIO_Pin_1
#define GPIO_Pin_en2	GPIO_Pin_5
#define GPIO_Pin_en3	GPIO_Pin_13

//丝杠电机
#define cw1_on		GPIO_ResetBits(GPIOcw1, GPIO_Pin_cw1)
#define en1_on		GPIO_ResetBits(GPIOen1, GPIO_Pin_en1)
#define cw1_off		GPIO_SetBits(GPIOcw1, GPIO_Pin_cw1)
#define en1_off		GPIO_SetBits(GPIOen1, GPIO_Pin_en1)

#define cw2_on		GPIO_ResetBits(GPIOcw2, GPIO_Pin_cw2)
#define en2_on		GPIO_ResetBits(GPIOen2, GPIO_Pin_en2)
#define cw2_off		GPIO_SetBits(GPIOcw2, GPIO_Pin_cw2)
#define en2_off		GPIO_SetBits(GPIOen2, GPIO_Pin_en2)


#define cw3_on		GPIO_ResetBits(GPIOcw3, GPIO_Pin_cw3)
#define en3_on		GPIO_ResetBits(GPIOen3, GPIO_Pin_en3)
#define cw3_off		GPIO_SetBits(GPIOcw3, GPIO_Pin_cw3)
#define en3_off		GPIO_SetBits(GPIOen3, GPIO_Pin_en3)

typedef struct MechArm_t//机械手臂结构体
{
	uint8_t lock;
	
}MechArm_t;


void MechArm_ControlLoop(DBUS_TypeDef *dbus);
void GMPitch_ControlLoop(void);
void GMYaw_ControlLoop(void);
void CMControlLoop(void);
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed);
void shooting_ControlLoop(void);
void control_loop(void);
#endif
