#include "stm32f4xx.h"
#include "can.h"
#include "dbus.h"
#include "pid.h"
#include "calc.h"

CanRxMsg		receivemessage;

volatile Encoder CM1Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM2Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM3Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM4Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder GMYawEncoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder GMPitchEncoder = {0,0,0,0,0,0,0,0,0};


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


 void cm_senddata(CAN_TypeDef* CANx, int num1, int num2, int num3, int num4)//底盘can发送
{
		CanTxMsg  sendmessage;
		sendmessage.DLC									= 0x08;
		sendmessage.IDE									= CAN_ID_STD;
		sendmessage.RTR									= CAN_RTR_DATA;
		sendmessage.StdId								= 0x200;
		sendmessage.Data[0]								= ((num1) >> 8);
		sendmessage.Data[1]								= (num1);
		sendmessage.Data[2]								= ((num2) >> 8);
		sendmessage.Data[3]								= (num2);
		sendmessage.Data[4]								= ((num3) >> 8);
		sendmessage.Data[5]								= (num3);
		sendmessage.Data[6]								= ((num4) >> 8);
		sendmessage.Data[7]								= (num4);

		CAN_Transmit(CANx, &sendmessage);
}
 void gm_senddata(CAN_TypeDef* CANx, int num1, int num2)//云台can发送
 {
		 CanTxMsg  sendmessage;
		 sendmessage.DLC = 0x08;
		 sendmessage.IDE = CAN_ID_STD;
		 sendmessage.RTR = CAN_RTR_DATA;
		 sendmessage.StdId = 0x1FF;
		 sendmessage.Data[0] = ((num1) >> 8);
		 sendmessage.Data[1] = (num1);
		 sendmessage.Data[2] = ((num2) >> 8);
		 sendmessage.Data[3] = (num2);
		 sendmessage.Data[4] = 0;
		 sendmessage.Data[5] = 0;
		 sendmessage.Data[6] = 0;
		 sendmessage.Data[7] = 0;

		 CAN_Transmit(CANx, &sendmessage);
 }
void gyro_senddata(CAN_TypeDef* CANx)//云台can发送
 {
		 CanTxMsg  sendmessage;
		 sendmessage.DLC = 0x08;
		 sendmessage.IDE = CAN_ID_STD;
		 sendmessage.RTR = CAN_RTR_DATA;
		 sendmessage.StdId = 0x40;
		 sendmessage.Data[0] = 1;
		 sendmessage.Data[1] = 2;
		 sendmessage.Data[2] = 3;
		 sendmessage.Data[3] = 4;
		 sendmessage.Data[4] = 5;
		 sendmessage.Data[5] = 6;
		 sendmessage.Data[6] = 7;
		 sendmessage.Data[7] = 8;

		 CAN_Transmit(CANx, &sendmessage);
 }
void laser_senddata(CAN_TypeDef* CANx,uint8_t mode)//激光can发送
 {
		 CanTxMsg  sendmessage;
		 sendmessage.DLC = 0x08;
		 sendmessage.IDE = CAN_ID_STD;
		 sendmessage.RTR = CAN_RTR_DATA;
		 sendmessage.StdId = 0x50;
	 if(mode)
	 {
		 sendmessage.Data[0] = 1;
		 sendmessage.Data[1] = 1;
	 }
	 else
	 {
		 sendmessage.Data[0] = 0;
		 sendmessage.Data[1] = 0;
	 }
		 sendmessage.Data[2] = 2;
		 sendmessage.Data[3] = 6;
		 sendmessage.Data[4] = 4;
		 sendmessage.Data[5] = 5;
		 sendmessage.Data[6] = 6;
		 sendmessage.Data[7] = 5;

		 CAN_Transmit(CANx, &sendmessage);
 }
 

 void Can_RecviveData(CanRxMsg * rec)//返回值解算
{
			switch (receivemessage.StdId)
			{
			case CAN_RM3510_1_ID:
			{
				EncoderProcess(&CM1Encoder,rec);
			}
				break;
			case CAN_RM3510_2_ID:
			{
				EncoderProcess(&CM2Encoder,rec);
			}
				break;
			case CAN_RM3510_3_ID:
			{
				EncoderProcess(&CM3Encoder,rec);
			}
				break;
			case CAN_RM3510_4_ID:
			{
				EncoderProcess(&CM4Encoder,rec);
			}
				break;
			case CAN_6623_PITCH_ID:
			{
				EncoderProcess(&GMPitchEncoder,rec);
			}
				break;
			case CAN_6623_YAW_ID:
			{
				EncoderProcess(&GMYawEncoder,rec);
			}
				break;
			case CAN_GYRO_ID:
			{
				GYROX.U[0]=rec->Data[0];
				GYROX.U[1]=rec->Data[1];
				GYROX.U[2]=rec->Data[2];
				GYROX.U[3]=rec->Data[3];
				GYROY.U[0]=rec->Data[4];
				GYROY.U[1]=rec->Data[5];
				GYROY.U[2]=rec->Data[6];
				GYROY.U[3]=rec->Data[7];
			}
				break;
			case CAN_ACCE_ID:
			{
				GYROX.U[0]=rec->Data[0];
				GYROX.U[1]=rec->Data[1];
				GYROX.U[2]=rec->Data[2];
				GYROX.U[3]=rec->Data[3];
				GYROY.U[0]=rec->Data[4];
				GYROY.U[1]=rec->Data[5];
				GYROY.U[2]=rec->Data[6];
				GYROY.U[3]=rec->Data[7];
			}
				break;
			case CAN_LASER_RID:
			{

			}
				break;
			}
}



void CAN2_RX0_IRQHandler(void)//底盘云台值解算中断
{
			if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=0)
			{
				CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
				CAN_Receive(CAN2, CAN_FIFO0, &receivemessage);
				Can_RecviveData(&receivemessage);

			}
}
void CAN1_RX0_IRQHandler(void)//单轴陀螺仪值解算中断
{
			if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != 0)
			{
				CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
				CAN_Receive(CAN1, CAN_FIFO0, &receivemessage);
				Can_RecviveData(&receivemessage);
			}
}




void EncoderProcess(volatile Encoder *v, CanRxMsg * msg)//云台值解算函数
{
			int i=0;
			int32_t temp_sum = 0;    
			v->last_raw_value = v->raw_value;
			v->raw_value = (msg->Data[0]<<8)|msg->Data[1];
			v->diff = v->raw_value - v->last_raw_value;
			if(v->diff < -7500)    //两次编码器的反馈值差别太大，表示圈数发生了改变
			{
				v->round_cnt++;
				v->ecd_raw_rate = v->diff + 8192;
			}
			else if(v->diff>7500)
			{
				v->round_cnt--;
				v->ecd_raw_rate = v->diff- 8192;
			}		
			else
			{
				v->ecd_raw_rate = v->diff;
			}
			//计算得到连续的编码器输出值
			v->ecd_value = v->raw_value + v->round_cnt * 8192;
			//计算得到角度值，范围正负无穷大
			v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*360/8192 + v->round_cnt * 360;
			v->rate_buf[v->buf_count++] = v->ecd_raw_rate;
			if(v->buf_count == RATE_BUF_SIZE)
			{
				v->buf_count = 0;
			}
			//计算速度平均值
			for(i = 0;i < RATE_BUF_SIZE; i++)
			{
				temp_sum += v->rate_buf[i];
			}
			v->filter_rate = (int32_t)(temp_sum/RATE_BUF_SIZE);					
}



