#ifndef __CALC_H
#include "stm32f4xx.h"
typedef union
{
    uint8_t U[4];
    float F;
    int I;
}FormatTrans;


#endif