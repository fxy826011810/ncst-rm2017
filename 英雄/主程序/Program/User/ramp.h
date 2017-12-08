#ifndef __RAMP_H
#define __RAMP_H
#include "stm32f4xx.h"


typedef struct ramp_t
{
int count;
int countmax;
float out;
void (*init)(struct ramp_t *ramp,int16_t countmax);
float (*calc)(struct ramp_t *ramp,uint8_t mode);
void (*setcount)(struct ramp_t *ramp,int16_t count);
void (*resetcount)(struct ramp_t *ramp);
void (*setcountmax)(struct ramp_t *ramp,int16_t countmax);
uint8_t (*IsOverflow)(struct ramp_t *ramp);
}ramp_t;

extern ramp_t GMPRamp;
extern ramp_t GMYRamp;

extern ramp_t FRIRamp;
extern ramp_t FRIBRamp;

extern ramp_t POKONRamp;
extern ramp_t POKOFFRamp;
extern ramp_t SGONRamp;//Õðµ¯¿ªÊ¼
extern ramp_t SGOFFRamp;//Õðµ°½áÊø

extern ramp_t CMXRampR;
extern ramp_t CMYRampR;

extern ramp_t CMXRampK;
extern ramp_t CMYRampK;

extern ramp_t CMRotateR;
extern ramp_t CMRotateK;

extern ramp_t CMRotateS;
extern ramp_t CMXRampS;
extern ramp_t CMYRampS;
#define Ramp_Default \
{\
 0,\
 0,\
 0,\
 &ramp_init, \
 &ramp_calc, \
 &ramp_setcount, \
 &ramp_resetcount, \
 &ramp_setcountmax, \
 &ramp_IsOverflow, \
}\
  
void ramp_init(ramp_t *ramp,int16_t countmax);
float ramp_calc( ramp_t* ramp,uint8_t mode);
void ramp_setcount(ramp_t *ramp,int16_t count);
void ramp_resetcount(ramp_t *ramp);
void ramp_setcountmax(ramp_t *ramp,int16_t countmax);
uint8_t ramp_IsOverflow(ramp_t *ramp);
void ramp_config(void);

#endif
