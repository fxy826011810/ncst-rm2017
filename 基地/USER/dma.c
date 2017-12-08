#include "stm32f10x.h"
#include "judge.h"
#include "dma.h"




void Bsp_DMA_Init(void)
{

//DBUS
	DMA_InitTypeDef dma;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//´ò¿ªÊ±ÖÓ

	DMA_DeInit(DMA1_Channel5);
	DMA_StructInit(&dma);
	dma.DMA_Mode = DMA_Mode_Normal;
	dma.DMA_Priority = DMA_Priority_Medium;
	dma.DMA_BufferSize = JudgeBufferLength;
	dma.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma.DMA_MemoryBaseAddr = (uint32_t)JudgeDataBuffer;
	dma.DMA_M2M = DMA_M2M_Disable;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Init(DMA1_Channel5, &dma);
	DMA_Cmd(DMA1_Channel5, ENABLE);	
}



