#include "usart.h"
#include "stm32f10x.h"
#include "judge.h"
#include "delay.h"
#include "control.h"
#include <stdio.h>

int fputc(int ch,FILE*f)//printf函数重定义
{
	USART_SendData(USART3,(unsigned char)ch);
	while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));
	return ch;
	
}

void Bsp_Usart_Init(void)
{
	
  USART_InitTypeDef								usart;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_DeInit(USART1);
	USART_StructInit(&usart);
	usart.USART_BaudRate							= 115200;
	usart.USART_HardwareFlowControl					= USART_HardwareFlowControl_None;
	usart.USART_Mode								= USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_Parity								= USART_Parity_No;
	usart.USART_StopBits							= USART_StopBits_1;
	usart.USART_WordLength							= USART_WordLength_8b;
	USART_Init(USART1, &usart);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART1, ENABLE);

	USART_ClearFlag(USART1,USART_FLAG_IDLE);//防止接受不到第一个字符


}
uint16_t MaxA=0,ARate=1000;
uint16_t JudgeFrameCounter=0;
uint16_t JudgeFrameRate=0;
void USART1_IRQHandler(void)//裁判系统中断
{

    if (USART_GetITStatus(USART1, USART_IT_IDLE) == 1)
      {
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
				USART_ReceiveData(USART1);
				DMA_Cmd(DMA1_Channel5, DISABLE);
				  if((DMA1_Channel5->CNDTR == 0)&&(JudgeDataBuffer[5] == 1)&&(JudgeDataBuffer[0] == JudgeFrameHeader)&&Verify_CRC16_Check_Sum(JudgeDataBuffer,47))
        {
					JudgeFrameCounter++;
				//时间	
				FT.U[3] = JudgeDataBuffer[10];
        FT.U[2] = JudgeDataBuffer[9];
        FT.U[1] = JudgeDataBuffer[8];
        FT.U[0] = JudgeDataBuffer[7];	
				game.remainTime=FT.F;
				//血量	
				game.remainLifeValue=JudgeDataBuffer[11]|(JudgeDataBuffer[12]<<8);	
				//60焦耳	
				FT.U[3] = JudgeDataBuffer[41];
        FT.U[2] = JudgeDataBuffer[40];
        FT.U[1] = JudgeDataBuffer[39];
        FT.U[0] = JudgeDataBuffer[38];	
				game.remainPower=FT.F;
				//读取电压
				FT.U[3] = JudgeDataBuffer[16];
        FT.U[2] = JudgeDataBuffer[15];
        FT.U[1] = JudgeDataBuffer[14];
        FT.U[0] = JudgeDataBuffer[13];
        game.realChassisOutV=FT.F;
        //读取电流
        FT.U[3] = JudgeDataBuffer[20];
        FT.U[2] = JudgeDataBuffer[19];
        FT.U[1] = JudgeDataBuffer[18];
        FT.U[0] = JudgeDataBuffer[17];
				game.realChassisOutA=FT.F;
				MaxA=(MaxWheelPower/game.realChassisOutV)*3500;
				game.realChassisOutW=game.realChassisOutA*game.realChassisOutV;	
				
					
        }
					else  if((DMA1_Channel5->CNDTR == 0)&&(JudgeDataBuffer[5] == 2)&&(JudgeDataBuffer[0] == JudgeFrameHeader)&&Verify_CRC16_Check_Sum(JudgeDataBuffer,12) )
        {
					JudgeFrameCounter++;
        }
//					else  if((DMA1_Channel5->CNDTR == 0)&&(JudgeDataBuffer[5] == 3)&&(JudgeDataBuffer[0] == JudgeFrameHeader)&&Verify_CRC16_Check_Sum(JudgeDataBuffer,25))
					else  if((DMA1_Channel5->CNDTR == 0)&&(JudgeDataBuffer[5] == 3)&&(JudgeDataBuffer[0] == JudgeFrameHeader))

        {
					JudgeFrameCounter++;
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
        //重启DMA
        DMA_ClearFlag(DMA1_FLAG_TC5 | DMA1_FLAG_HT5);
        while((DMA1_Channel5->CCR&DMA_CCR5_EN) != DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel5, 44);
        DMA_Cmd(DMA1_Channel5, ENABLE);         
      }
}
		


