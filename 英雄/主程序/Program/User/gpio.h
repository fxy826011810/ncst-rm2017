#ifndef __GPIO_H
#define __GPIO_H
#define	LED_HEAT() GPIOF->ODR^=GPIO_Pin_14
#define LASER_ON()  GPIO_SetBits(GPIOG, GPIO_Pin_13)
#define LASER_OFF()  GPIO_ResetBits(GPIOG, GPIO_Pin_13)
void Bsp_GPIO_Init(void);

#endif
