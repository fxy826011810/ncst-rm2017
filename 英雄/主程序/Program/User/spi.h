#ifndef __SPI_H
#define __SPI_H
#include "stm32f4xx.h"


void Bsp_Spi_Init(void);
uint8_t MPU6500_WriteByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t pbuffer);
uint8_t MPU6500_ReadBytes(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer,uint8_t num);
uint8_t MPU6500_ReadByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer);

uint8_t IST8310_ReadByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer);
uint8_t IST8310_ReadBytes(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer,uint8_t num);
uint8_t IST8310_WriteByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t pbuffer);

uint16_t SPI_WriteReadByte(SPI_TypeDef* SPIx,uint8_t reg);

#endif
