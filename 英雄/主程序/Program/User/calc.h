#ifndef __CALC_H
#define __CALC_H
#include "stm32f4xx.h"
typedef union
{
    uint8_t U[4];
    float F;
    int I;
}FormatTrans;
extern FormatTrans FT; 
extern FormatTrans GYROX,GYROY,ACCEX,ACCEY;
#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\


#endif
