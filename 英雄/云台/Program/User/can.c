#include "stm32f4xx.h"
#include "can.h"
#include "gpio.h"
#include "mpu6050.h"

CanRxMsg		receivemessage;
CanTxMsg		sendmessage;
 void Bsp_Can_Init(void)
{
			CAN_InitTypeDef                       can;
			CAN_FilterInitTypeDef                 can_filter;
			 

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);

			//can1
			CAN_DeInit(CAN1);
			CAN_StructInit(&can);
			can.CAN_ABOM									= DISABLE;
			can.CAN_AWUM									= DISABLE;
			can.CAN_NART									= DISABLE;
			can.CAN_RFLM									= DISABLE;
			can.CAN_TTCM									= DISABLE;
			can.CAN_TXFP									= ENABLE;
			can.CAN_SJW										= CAN_SJW_1tq;
			can.CAN_BS1										= CAN_BS1_9tq;
			can.CAN_BS2										= CAN_BS2_4tq;
			can.CAN_Prescaler								= 3;
			can.CAN_Mode									= CAN_Mode_Normal;
			CAN_Init(CAN1, &can);
			
			
			can_filter.CAN_FilterActivation					= ENABLE;
			can_filter.CAN_FilterFIFOAssignment				= 0;
			can_filter.CAN_FilterIdHigh						= 0x0000;
			can_filter.CAN_FilterIdLow						= 0x0000;
			can_filter.CAN_FilterMaskIdHigh					= 0x0000;
			can_filter.CAN_FilterMaskIdLow					= 0x0000;
			can_filter.CAN_FilterMode						= CAN_FilterMode_IdMask;
			can_filter.CAN_FilterNumber						= 0;
			can_filter.CAN_FilterScale						= CAN_FilterScale_32bit;
			can_filter.CAN_FilterNumber						= 0;
			CAN_FilterInit(&can_filter);


			//can2
			CAN_DeInit(CAN2);
			CAN_StructInit(&can);
			can.CAN_ABOM									= DISABLE;
			can.CAN_AWUM									= DISABLE;
			can.CAN_NART									= DISABLE;
			can.CAN_RFLM									= DISABLE;
			can.CAN_TTCM									= DISABLE;
			can.CAN_TXFP									= ENABLE;
			can.CAN_SJW										= CAN_SJW_1tq;
			can.CAN_BS1										= CAN_BS1_9tq;
			can.CAN_BS2										= CAN_BS2_4tq;
			can.CAN_Prescaler								= 3;
			can.CAN_Mode									= CAN_Mode_Normal;
			CAN_Init(CAN2, &can);

			can_filter.CAN_FilterActivation					= ENABLE;
			can_filter.CAN_FilterFIFOAssignment				= 0;
			can_filter.CAN_FilterIdHigh						= 0x0000;
			can_filter.CAN_FilterIdLow						= 0x0000;
			can_filter.CAN_FilterMaskIdHigh					= 0x0000;
			can_filter.CAN_FilterMaskIdLow					= 0x0000;
			can_filter.CAN_FilterMode						= CAN_FilterMode_IdMask;
			can_filter.CAN_FilterNumber						= 0;
			can_filter.CAN_FilterScale						= CAN_FilterScale_32bit;
			can_filter.CAN_FilterNumber						= 14;
			CAN_FilterInit(&can_filter);

			CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
			CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}


 void gyro_senddata(CAN_TypeDef* CANx,FormatTrans *gyrox,FormatTrans *gyroy)//底盘can发送
{
		CanTxMsg  sendmessage;
		sendmessage.DLC									= 0x08;
		sendmessage.IDE									= CAN_ID_STD;
		sendmessage.RTR									= CAN_RTR_DATA;
		sendmessage.StdId								= 0x41;
		sendmessage.Data[0]							= gyrox->U[0];
		sendmessage.Data[1]							= gyrox->U[1];
		sendmessage.Data[2]							= gyrox->U[2];
		sendmessage.Data[3]							= gyrox->U[3];
		sendmessage.Data[4]							= gyroy->U[0];
		sendmessage.Data[5]							= gyroy->U[1];
		sendmessage.Data[6]							= gyroy->U[2];
		sendmessage.Data[7]							= gyroy->U[3];

		CAN_Transmit(CANx, &sendmessage);
}
 
 void Can_RecviveData(CanRxMsg * rec)//返回值解算
{
			switch (receivemessage.StdId)
			{
				case 0x40://陀螺仪数据发送ID
				{
					if(receivemessage.Data[0]==1&&receivemessage.Data[1]==2&&receivemessage.Data[2]==3&&receivemessage.Data[3]==4)
					{
					gyro_senddata(CAN1,&GYROX,&GYROY);
					}
				}
					break;
				case 0x50://陀螺仪数据发送ID
				{
					if(receivemessage.Data[0]==1&&receivemessage.Data[1]==1&&receivemessage.Data[7]==5)
					{
						LASER_ON();
					}
					else if(receivemessage.Data[0]==0&&receivemessage.Data[1]==0&&receivemessage.Data[7]==5)
					{
						LASER_OFF();
					}
				}
					break;
			}
}



void CAN2_RX0_IRQHandler(void)//底盘值解算中断
{
			if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=0)
			{
				CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
				CAN_Receive(CAN2, CAN_FIFO0, &receivemessage);
				Can_RecviveData(&receivemessage);

			}
}
void CAN1_RX0_IRQHandler(void)//没用
{
			if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != 0)
			{
				CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
				CAN_Receive(CAN1, CAN_FIFO0, &receivemessage);
				Can_RecviveData(&receivemessage);
			}
}




