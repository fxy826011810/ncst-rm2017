#ifndef __GPIO_H
#define __GPIO_H
#define	LED_HEAT() GPIOC->ODR^=GPIO_Pin_1|GPIO_Pin_2
#define	LED1 GPIOA->ODR ^=GPIO_Pin_6
#define	LED2 GPIOC->ODR ^=GPIO_Pin_6
void Bsp_GPIO_Init(void);

#endif
