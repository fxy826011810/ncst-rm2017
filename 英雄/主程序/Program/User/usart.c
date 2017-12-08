#include "usart.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "dbus.h"
#include "judge.h"
#include <stdio.h>
#include "calc.h"
#include "Infantry.h"
int fputc(int ch,FILE*f)//printf�����ض���
{
	USART_SendData(USART3,(unsigned char)ch);
	while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));
	return ch;
	
}

void Bsp_Usart_Init(void)
{
	
    USART_InitTypeDef								usart;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//usart1(ң��)
	USART_DeInit(USART1);
	USART_StructInit(&usart);
	usart.USART_BaudRate							= 100000;
	usart.USART_HardwareFlowControl					= USART_HardwareFlowControl_None;
	usart.USART_Mode								= USART_Mode_Rx;
	usart.USART_Parity								= USART_Parity_Even;
	usart.USART_StopBits							= USART_StopBits_1;
	usart.USART_WordLength							= USART_WordLength_8b;
	USART_Init(USART1, &usart);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
  USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART1, ENABLE);

	//usart3
	USART_DeInit(USART3);
	USART_StructInit(&usart);
	usart.USART_BaudRate							= 115200;
	usart.USART_HardwareFlowControl					= USART_HardwareFlowControl_None;
	usart.USART_Mode								= USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity								= USART_Parity_No;
	usart.USART_StopBits							= USART_StopBits_1;
	usart.USART_WordLength							= USART_WordLength_8b;
	USART_Init(USART3, &usart);
	//USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	USART_Cmd(USART3, ENABLE);
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);


	USART_ClearFlag(USART1,USART_FLAG_TC);//��ֹ���ܲ�����һ���ַ�
}

void USART1_IRQHandler(void)//����ң����ֵ
{

    if (USART_GetITStatus(USART1, USART_IT_IDLE) == 1)
      {
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
				USART_ReceiveData(USART1);
				DMA_Cmd(DMA2_Stream2, DISABLE);
        //��������ȷ
        if(DMA2_Stream2->NDTR == 1)
        {
          dbus_calc(&DBUS);
        }
        //����DMA
        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
        while(DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);
        DMA_SetCurrDataCounter(DMA2_Stream2, 19);
        DMA_Cmd(DMA2_Stream2, ENABLE);         
      }
}

uint16_t MaxA=0,ARate=1000;
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_IDLE) == 1)
      {
        USART_ClearITPendingBit(USART3, USART_IT_IDLE);
				USART_ReceiveData(USART3);
				DMA_Cmd(DMA1_Stream1, DISABLE);
        //��������ȷ
				if(DMA1_Stream1->NDTR == 0)
				{
        if((JudgeDataBuffer[5] == 1)&&Verify_CRC8_Check_Sum(JudgeDataBuffer,4)&&Verify_CRC16_Check_Sum(JudgeDataBuffer, (uint32_t)((JudgeDataBuffer[2]<<8)|JudgeDataBuffer[1]+4+8)))
        {
				//ʱ��
				FT.U[3] = JudgeDataBuffer[10];
        FT.U[2] = JudgeDataBuffer[9];
        FT.U[1] = JudgeDataBuffer[8];
        FT.U[0] = JudgeDataBuffer[7];	
				game.remainTime=FT.I;
				//Ѫ��	
				game.remainLifeValue=JudgeDataBuffer[11]|(JudgeDataBuffer[12]<<8);	
				//60����	
				FT.U[3] = JudgeDataBuffer[41];
        FT.U[2] = JudgeDataBuffer[40];
        FT.U[1] = JudgeDataBuffer[39];
        FT.U[0] = JudgeDataBuffer[38];	
				game.remainPower=FT.F;
					//��ȡ��ѹ
				FT.U[3] = JudgeDataBuffer[16];
        FT.U[2] = JudgeDataBuffer[15];
        FT.U[1] = JudgeDataBuffer[14];
        FT.U[0] = JudgeDataBuffer[13];
        game.realChassisOutV=FT.F;
        //��ȡ��ѹ
        FT.U[3] = JudgeDataBuffer[20];
        FT.U[2] = JudgeDataBuffer[19];
        FT.U[1] = JudgeDataBuffer[18];
        FT.U[0] = JudgeDataBuffer[17];
				game.realChassisOutA=FT.F;
				MaxA=(MaxWheelPower/game.realChassisOutV)*2800;
				game.realChassisOutW=game.realChassisOutA*game.realChassisOutV;
        }
				else if((JudgeDataBuffer[5] == 2)&&Verify_CRC16_Check_Sum(JudgeDataBuffer, (uint32_t)((JudgeDataBuffer[2]<<8)|JudgeDataBuffer[1]+4+8)))
        {
					
					
        }
				else if((JudgeDataBuffer[5] == 3)&&Verify_CRC16_Check_Sum(JudgeDataBuffer, (uint32_t)((JudgeDataBuffer[2]<<8)|JudgeDataBuffer[1]+4+8)))        
				{
				FT.U[3] = JudgeDataBuffer[10];
        FT.U[2] = JudgeDataBuffer[9];
        FT.U[1] = JudgeDataBuffer[8];
        FT.U[0] = JudgeDataBuffer[7];
				shoot.realBulletShootSpeed=FT.F;	
					
        FT.U[3] = JudgeDataBuffer[14];
        FT.U[2] = JudgeDataBuffer[13];
        FT.U[1] = JudgeDataBuffer[12];
        FT.U[0] = JudgeDataBuffer[11];
				shoot.realBulletShootFreq=FT.F;
        }
			}
        //����DMA
        DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
        while(DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);
        DMA_SetCurrDataCounter(DMA1_Stream1, 44);
        DMA_Cmd(DMA1_Stream1, ENABLE);         
      }
}

		


