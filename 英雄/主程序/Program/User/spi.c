#include "spi.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "mpu6050.h"
#define mpu6500_cs(x) GPIOF->ODR=((GPIOF->ODR&(~GPIO_Pin_6))|(x <<6))

uint8_t a;
void Bsp_Spi_Init(void)
{
	SPI_InitTypeDef spi;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5,ENABLE);
	SPI_I2S_DeInit(SPI5);
	spi.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_128;
	spi.SPI_CPHA=SPI_CPHA_1Edge;
	spi.SPI_CPOL=SPI_CPOL_Low;
	spi.SPI_CRCPolynomial=10;
	spi.SPI_DataSize=SPI_DataSize_8b;
	spi.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	spi.SPI_FirstBit=SPI_FirstBit_MSB;
	spi.SPI_Mode=SPI_Mode_Master;
	spi.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(SPI5,&spi);
	SPI_Cmd(SPI5,ENABLE);
}


uint8_t MPU6500_WriteByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t pbuffer)
{
	reg=reg&0x7F;
	mpu6500_cs(0);;
	SPI_WriteReadByte(SPIx,reg);
	SPI_WriteReadByte(SPIx,pbuffer);
	mpu6500_cs(1);
	
	return 0;
}

uint8_t MPU6500_ReadByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer)
{
	 
	mpu6500_cs(0);
	
	 SPI_WriteReadByte(SPIx,reg|0x80);
	*pbuffer =(uint8_t)SPI_WriteReadByte(SPIx,0xff);
	mpu6500_cs(1);
	
	return 0;
}

uint8_t MPU6500_ReadBytes(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer,uint8_t num)
{
	uint8_t i;
	
	for(i=0;i<num;i++)
	{
	mpu6500_cs(0);
	SPI_WriteReadByte(SPIx,reg|0x80);
	*pbuffer =(uint8_t)SPI_WriteReadByte(SPIx,0xff);
	mpu6500_cs(1);
	reg++;
	pbuffer++;
	delay_us(2);		
	}
	
	return 0;
}

uint8_t IST8310_ReadByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer)
{
	u8 status = 0;
	u32 timeout = 0;
	MPU6500_WriteByte(SPIx,MPU6500_I2C_SLV4_ADDR,0x80|IST8310_ADDRESS);
	delay_us(2);
	MPU6500_WriteByte(SPIx,MPU6500_I2C_SLV4_REG,reg);
	delay_us(2);
	MPU6500_WriteByte(SPIx,MPU6500_I2C_SLV4_CTRL, 0x80);
	delay_us(2);
	do {
		if (timeout++ > 50){
			return 2;
		}
		MPU6500_ReadByte(SPIx, MPU6500_I2C_MST_STATUS,&status);
		delay_us(2);
	} while ((status & 0x40) == 0);
	MPU6500_ReadByte(SPIx, MPU6500_I2C_SLV4_DI,pbuffer);
	delay_us(2);
	return 0;
}
uint8_t IST8310_ReadBytes(SPI_TypeDef* SPIx,uint8_t reg,uint8_t *pbuffer,uint8_t num)
{
	uint8_t i;
	IST8310_WriteByte(SPIx,IST8310_R_CONFA,IST8310_ODR_MODE);
	for(i=0;i<num;i++)
	{
	IST8310_ReadByte(SPIx,reg,pbuffer);
	pbuffer++;
	reg++;
	}
	return 0;
}
uint8_t IST8310_WriteByte(SPI_TypeDef* SPIx,uint8_t reg,uint8_t pbuffer)
{
	

	u32 timeout = 0;
	uint8_t status = 0;
	MPU6500_WriteByte(SPIx,MPU6500_I2C_SLV1_ADDR,IST8310_ADDRESS);
	delay_us(2);
	MPU6500_WriteByte(SPIx,MPU6500_I2C_SLV1_REG,reg);
	delay_us(2);
	MPU6500_WriteByte(SPIx,MPU6500_I2C_SLV1_DO,pbuffer);
	delay_us(2);
	MPU6500_WriteByte(SPIx,MPU6500_I2C_SLV1_CTRL, 0x80|0x01);
	delay_us(2);
		do {
		if (timeout++ > 50)
			return 2;
		MPU6500_ReadByte(SPIx, MPU6500_I2C_MST_STATUS,&status);
		delay_us(2);
	} while ((status & 0x40) == 0);
	if (status & 0x10)
		return 3;
	return 0;
}


uint16_t SPI_WriteReadByte(SPI_TypeDef* SPIx,uint8_t reg)
{	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx, reg);
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPIx);
}

