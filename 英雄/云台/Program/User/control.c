#include "control.h"
#include "stm32f4xx.h"
#include "gpio.h"
#include "can.h"
#include "tim.h"

uint16_t heart=0;//ÐÄÌøµÆ

void control_loop(void)
{
	heart++;
	if(heart==500)
	{
		heart=0;
		LED_HEAT();
	}
}

      