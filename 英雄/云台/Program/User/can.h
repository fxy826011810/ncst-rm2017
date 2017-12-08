#ifndef __CAN_H
#define __CAN_H
#include "stm32f4xx.h"
#include "mpu6050.h"
#define __CAN_EXT extern
#define RATE_BUF_SIZE 6
typedef enum
{
 CAN_Palstance				=0x40,

}CAN_ID;

__CAN_EXT CanTxMsg	sendmessage;
__CAN_EXT CanRxMsg	receivemessage;

void Bsp_Can_Init(void);
 //void cm_senddata(CAN_TypeDef* CANx,FormatTrans *gyrox,FormatTrans*gyroy);//µ×ÅÌcan·¢ËÍ
void Can_RecviveData(CanRxMsg * rec);

#endif

