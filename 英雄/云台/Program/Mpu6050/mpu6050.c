#include "mpu6050.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "i2c.h"
#include "can.h"
#include "tim.h"
#include <stdio.h>

MPU_DATA MPU_REAL_DATA;

int16_t gxset=0,gyset=0,gzset=0,axset=0,ayset=0,azset=0;uint8_t mpu_buf[14];
uint8_t mpu6050_init(void)
{
	int i;
	uint8_t mpu6050_id;
	if (!MPU6050_ReadByte(MPU6050_ID, WHO_AM_I, &mpu6050_id))
	{
		if (mpu6050_id != MPU6050_ID)
		{
			printf("MPU_6050 ID check error\r\n");
			return 1; //校验失败，返回0xff
		}
	}
	else
	{ 
		printf("MPU_6050 ID read error\r\n");
		return 1;
	}
	if (MPU6050_WriteByte(MPU6050_ID,PWR_MGMT_1, 0x01)==1)
	{
		printf("PWR_MGMT_1 error\r\n");
	return 1; 
	}
	if (MPU6050_WriteByte(MPU6050_ID, CONFIG, 0x03))
	{
		printf("CONFIG error\r\n");
	return 1; 
	}
	if (MPU6050_WriteByte(MPU6050_ID, GYRO_CONFIG, 0x10))
	{
		printf("GYRO_CONFIG error\r\n");
	return 1; 
	}
	if (MPU6050_WriteByte(MPU6050_ID, ACCEL_CONFIG, 0x00))
	{
		printf("ACCEL_CONFIG error\r\n");
	return 1; 
	}
	if (MPU6050_WriteByte(MPU6050_ID, INT_PIN_CFG, 0x02))
	{
		printf("INT_PIN_CFG error\r\n");
	return 1; 
	}	
	if (MPU6050_WriteByte(MPU6050_ID, INT_ENABLE, 0x00))
	{
		printf("INT_ENABLE error\r\n");
		return 1;
	}
	if (MPU6050_WriteByte(MPU6050_ID, MPU6050_RA_USER_CTRL, 0x00))
	{
		printf("MPU6050_RA_USER_CTRL error\r\n");
	return 1; 
	}
	  	if (MPU6050_WriteByte(MPU6050_ID, SMPLRT_DIV, 0x01))
	{
		printf("SMPLRT_DIV error\r\n");
		return 1;
	}

	if (MPU6050_WriteByte(MPU6050_ID, INT_ENABLE, 0x01))
	{
		printf("INT_ENABLE error\r\n");
		return 1;
	}

	delay_ms(500);
		for(i=0;i<200;i++)
	{
	MPU6050_Read(mpu_buf);
	gxset	+= mpu_buf[8]<<8 |mpu_buf[9];
	gyset	+= mpu_buf[10]<<8 |mpu_buf[11];
	gzset	+= mpu_buf[12]<<8 |mpu_buf[13];
	delay_ms(5);
	}
	gxset/=200;
	gyset/=200;
	gzset/=200;
	return 0;
}

uint8_t	MPU6050_WriteByte(uint8_t Addr, uint8_t reg, uint8_t Data)
{
	if (IIC_WriteByte(Addr, reg, Data))
	{
		printf("MPU6050_WriteByte error");
		return 1;
	}
	return 0;
}
uint8_t MPU6050_ReadByte(uint8_t Addr, uint8_t reg, uint8_t *pbuffer)
{
	
	if (IIC_ReadByte(Addr, reg, pbuffer))
	{
		printf("MPU6050_ReadByte error");
		return 1;
	}
	return 0;
}
uint8_t	MPU6050_Read(uint8_t *pbuffer)
{
	
	if (IIC_Read(MPU6050_ID, ACCEL_XOUT_H, pbuffer, 14))
	{
		printf("MPU6050_Read error");
		return 1;
	}
	return 0;
}







int16_t MPU6050_FIFO[6][11];//第一个代表寄存器第二个代表采样
uint8_t isMPU6050_is_DRY=0;
uint8_t Can_Send_Flag=0;
static void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{
	uint8_t i = 0;
	int32_t sum = 0;
	if (isMPU6050_is_DRY)
	{
		isMPU6050_is_DRY = 0;
		Can_Send_Flag=1;
		MPU6050_Read(mpu_buf);
		for (i = 1; i<10; i++)
		{
			MPU6050_FIFO[0][i - 1] = MPU6050_FIFO[0][i];
			MPU6050_FIFO[1][i - 1] = MPU6050_FIFO[1][i];
			MPU6050_FIFO[2][i - 1] = MPU6050_FIFO[2][i];
			MPU6050_FIFO[3][i - 1] = MPU6050_FIFO[3][i];
			MPU6050_FIFO[4][i - 1] = MPU6050_FIFO[4][i];
			MPU6050_FIFO[5][i - 1] = MPU6050_FIFO[5][i];
		}

		MPU6050_FIFO[0][9] = ((int16_t)mpu_buf[0] << 8)  | mpu_buf[1];
		MPU6050_FIFO[1][9] = ((int16_t)mpu_buf[2] << 8)  | mpu_buf[3];
		MPU6050_FIFO[2][9] = ((int16_t)mpu_buf[4] << 8)  | mpu_buf[5];
		MPU6050_FIFO[3][9] = ((int16_t)mpu_buf[8] << 8)  | mpu_buf[9];
		MPU6050_FIFO[4][9] = ((int16_t)mpu_buf[10] << 8) | mpu_buf[11];
		MPU6050_FIFO[5][9] = ((int16_t)mpu_buf[12] << 8) | mpu_buf[13];

		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[0][i];
		}
		MPU6050_FIFO[0][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[1][i];
		}
		MPU6050_FIFO[1][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[2][i];
		}
		MPU6050_FIFO[2][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[3][i];
		}
		MPU6050_FIFO[3][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[4][i];
		}
		MPU6050_FIFO[4][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[5][i];
		}
    MPU6050_FIFO[5][10] = sum / 10;
		sum = 0;

		*ax = MPU6050_FIFO[0][10]-axset;//=MPU6050_FIFO[0][10];
		*ay = MPU6050_FIFO[1][10]-ayset;//=MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10]-azset;//=MPU6050_FIFO[2][10];
		*gx = MPU6050_FIFO[3][10]-gxset ;//=MPU6050_FIFO[3][10];
		*gy = MPU6050_FIFO[4][10]-gyset;//=MPU6050_FIFO[4][10];
		*gz = MPU6050_FIFO[5][10]-gzset;//=MPU6050_FIFO[5][10];
	}
	else
	{
		*ax = MPU6050_FIFO[0][10]-axset;//=MPU6050_FIFO[0][10];
		*ay = MPU6050_FIFO[1][10]-ayset;//=MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10]-azset;//=MPU6050_FIFO[2][10];
		*gx = MPU6050_FIFO[3][10]-gxset ;//=MPU6050_FIFO[3][10];
		*gy = MPU6050_FIFO[4][10]-gyset;//=MPU6050_FIFO[4][10];
		*gz = MPU6050_FIFO[5][10]-gzset;//=MPU6050_FIFO[5][10]
	}
	
	
}
FormatTrans GYROX,GYROY;
int16_t accgyroval[6];
void IMU_getValues(void) 
{
	MPU6050_getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], &accgyroval[3], &accgyroval[4], &accgyroval[5]);
	
	GYROX.F=(float)accgyroval[3]/32.8f;
	GYROY.F=(float)accgyroval[4]/32.8f;
}














