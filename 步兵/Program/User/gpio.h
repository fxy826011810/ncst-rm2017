#ifndef __GPIO_H
#define __GPIO_H
#define	LED_HEAT() GPIOC->ODR^=GPIO_Pin_1|GPIO_Pin_2
#define LASER_ON()  GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define LASER_OFF()  GPIO_ResetBits(GPIOA, GPIO_Pin_8)
void Bsp_GPIO_Init(void);

#endif
