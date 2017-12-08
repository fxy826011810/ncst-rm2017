#include "stm32f10x.h"
#include "can.h"
#include "pid.h"


CanRxMsg		receivemessage;

volatile Encoder CM1Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM2Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM3Encoder = {0,0,0,0,0,0,0,0,0};
volatile Encoder CM4Encoder = {0,0,0,0,0,0,0,0,0};


 void Bsp_Can_Init(void)
{
			CAN_InitTypeDef                       can;
			CAN_FilterInitTypeDef                 can_filter;
			 

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

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
			can.CAN_BS1										= CAN_BS1_3tq;
			can.CAN_BS2										= CAN_BS2_5tq;
			can.CAN_Prescaler								= 4;
			can.CAN_Mode									= CAN_Mode_Normal;
			CAN_Init(CAN1, &can);
			
			
			can_filter.CAN_FilterActivation					= ENABLE;
			can_filter.CAN_FilterFIFOAssignment				= CAN_Filter_FIFO0;
			can_filter.CAN_FilterIdHigh						= 0x0000;
			can_filter.CAN_FilterIdLow						= 0x0000;
			can_filter.CAN_FilterMaskIdHigh					= 0x0000;
			can_filter.CAN_FilterMaskIdLow					= 0x0000;
			can_filter.CAN_FilterMode						= CAN_FilterMode_IdMask;
			can_filter.CAN_FilterNumber						= 0;
			can_filter.CAN_FilterScale						= CAN_FilterScale_32bit;
			can_filter.CAN_FilterNumber						= 0;
			CAN_FilterInit(&can_filter);
			
			CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);

}


	
 void cm_senddata(CAN_TypeDef* CANx, int16_t num1, int16_t num2, int16_t num3, int16_t num4)//底盘can发送
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

 

 void Can_RecviveData(CanRxMsg * rec)//返回值解算
{
			switch (receivemessage.StdId)
			{
			case CAN_EC60_1_ID:
			{
				EncoderProcess(&CM1Encoder,rec);
			}
				break;
			case CAN_EC60_2_ID:
			{
				EncoderProcess(&CM2Encoder,rec);
			}
				break;
			case CAN_EC60_3_ID:
			{
				EncoderProcess(&CM3Encoder,rec);
			}
				break;
			case CAN_EC60_4_ID:
			{

				EncoderProcess(&CM4Encoder,rec);
			}
				break;
			}
}



void USB_LP_CAN1_RX0_IRQHandler(void)//底盘云台值解算中断
{
			if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=0)
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
			else if(v-> diff>7500)
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



