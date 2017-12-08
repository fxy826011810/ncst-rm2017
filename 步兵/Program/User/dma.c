#include "stm32f4xx.h"
#include "dma.h"
#include "dbus.h"
#include "judge.h"


uint8_t JudgeDataBuffer[JudgeBufferLength];
void Bsp_DMA_Init(void)
{

	//DBUS
	DMA_InitTypeDef dma;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2| RCC_AHB1Periph_DMA1, ENABLE);//打开时钟
        
  DMA_DeInit(DMA2_Stream2);
  DMA_StructInit(&dma);
	dma.DMA_Mode				= DMA_Mode_Circular;
  dma.DMA_Priority			= DMA_Priority_VeryHigh;  
	dma.DMA_BufferSize			= 18;
	dma.DMA_Channel				= DMA_Channel_4;
	dma.DMA_DIR					= DMA_DIR_PeripheralToMemory; 
	dma.DMA_FIFOMode			= DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold		= DMA_FIFOThreshold_1QuarterFull; 
	dma.DMA_Memory0BaseAddr		= (uint32_t)DBUS_BUFFER;
	dma.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
	dma.DMA_MemoryDataSize		= DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc			= DMA_MemoryInc_Enable;
	dma.DMA_PeripheralBaseAddr	= (uint32_t)&(USART1->DR);
	dma.DMA_PeripheralBurst		= DMA_PeripheralBurst_Single;
	dma.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc		= DMA_PeripheralInc_Disable;
	DMA_Init(DMA2_Stream2,&dma);
	DMA_Cmd(DMA2_Stream2, ENABLE);
	//DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);//打开DMA中断
	
	
	
	DMA_DeInit(DMA1_Stream1);
	DMA_StructInit(&dma);
	dma.DMA_Mode = DMA_Mode_Normal;
	dma.DMA_Priority = DMA_Priority_Medium;
	dma.DMA_BufferSize = JudgeBufferLength;
	dma.DMA_Channel = DMA_Channel_4;
	dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_Memory0BaseAddr = (uint32_t)JudgeDataBuffer;
	dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Init(DMA1_Stream1, &dma);
	DMA_Cmd(DMA1_Stream1, ENABLE);
	
}



