#include "mpu6050.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "spi.h"
#include "math.h"
#include "tim.h"
#include <stdio.h>
#include "mpu6050_inter.h"
#define MPU_SPI  SPI5

IMUDataTypedef IMUData;
IMUDataTypedef IMUData_offset;
IMUDataTypedef IMUDataR;
IMUDataTypedef_t IMU;



uint8_t mpu6500_init(SPI_TypeDef* SPIx)
{
	uint8_t mpu6500_id;
	
	uint8_t data,state;
	MPU6500_WriteByte(SPIx,MPU6500_PWR_MGMT_1, 0x80);
	delay_ms(100);
	MPU6500_ReadByte(SPIx,MPU6500_WHO_AM_I,&mpu6500_id);
		if (mpu6500_id != MPU6500_ID)
		return 1; //校验失败，返回0xff
	delay_ms(1);
	MPU6500_WriteByte(SPIx,MPU6500_PWR_MGMT_1, 0x03);
	delay_ms(1);
	MPU6500_WriteByte(SPIx,MPU6500_INT_PIN_CFG, 0x10);
	delay_ms(1);
	MPU6500_WriteByte(SPIx,MPU6500_INT_ENABLE, 0x01);
	delay_ms(1);
	MPU6500_WriteByte(SPIx,MPU6500_PWR_MGMT_2, 0x00);
	delay_ms(1);
	MPU6500_WriteByte(SPIx,MPU6500_SMPLRT_DIV, 0x00);
	delay_ms(1);
	MPU6500_WriteByte(SPIx, MPU6500_GYRO_CONFIG, 0x18);
	delay_ms(1);
	MPU6500_WriteByte(SPIx, MPU6500_ACCEL_CONFIG, 0x10);
	delay_ms(1);
	MPU6500_ReadByte(SPIx,MPU6500_ACCEL_CONFIG_2,&data);	
	data|=0x02;
	delay_ms(1);
	MPU6500_WriteByte(SPIx, MPU6500_ACCEL_CONFIG_2, data);
	delay_ms(1);
	MPU6500_WriteByte(SPIx, MPU6500_CONFIG, 0x02);
	delay_ms(1);
	MPU6500_ReadByte(SPIx, MPU6500_USER_CTRL, &state);
	delay_ms(1);
	MPU6500_WriteByte(SPIx, MPU6500_USER_CTRL, state|0x10);
	delay_ms(1);
	MPU6500_ReadByte(SPIx, MPU6500_USER_CTRL, &state);
	delay_ms(1);
	MPU6500_WriteByte(SPIx, MPU6500_USER_CTRL, state|0x20);
	delay_ms(1);
	MPU6500_WriteByte(SPIx,MPU6500_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
  delay_ms(100);
	return 0;
}
uint8_t ist8310_init(SPI_TypeDef* SPIx)
{
	uint8_t ist8310_id;
	uint8_t a,b,c,d;
	MPU6500_WriteByte(SPIx,MPU6500_I2C_MST_CTRL,0x0D);
	delay_ms(2);
	IST8310_WriteByte(SPIx,IST8310_R_CONFB,0x01);
	delay_ms(2);
	//GPIO_SetBits(GPIOE,GPIO_Pin_2);
	IST8310_ReadByte(SPIx,IST8310_WHO_AM_I,&ist8310_id);
	if(IST8310_DEVICE_ID_A != ist8310_id)
    return 1;
	delay_ms(2);
	IST8310_WriteByte(SPIx,IST8310_R_CONFA, 0x00);
  delay_ms(2);
	IST8310_ReadByte(SPIx,IST8310_R_CONFA,&a);
	delay_ms(2);
	if(a!= 0x00)
    return 2;
  IST8310_WriteByte(SPIx,IST8310_R_CONFB, 0x00);
	delay_ms(2);
	IST8310_ReadByte(SPIx,IST8310_R_CONFB,&b);
	delay_ms(2);
	if(b!= 0x00)
    return 3;
	IST8310_WriteByte(SPIx,IST8310_AVGCNTL, 0x24);
	delay_ms(2);
	IST8310_ReadByte(SPIx,IST8310_AVGCNTL,&c);
	delay_ms(2);
	if(c!= 0x24)
    return 4;
	IST8310_WriteByte(SPIx,IST8310_PDCNTL, 0xc0);
	delay_ms(2);
	IST8310_ReadByte(SPIx,IST8310_PDCNTL,&d);
  if(d!= 0xc0)
  return 5;
	delay_ms(2);
	IST8310_WriteByte(SPIx,IST8310_R_CONFA,IST8310_ODR_MODE);
  delay_ms(100);
	return 0;
}

uint8_t mpu_buff[20];	
int16_t gxset=0,gyset=0,gzset=0,mxset=46,myset=-111,mzset=244,axset=0,ayset=0,azset=0;
void mpu6500_cali(SPI_TypeDef* SPIx)
{
	uint8_t i,cali_time=200;
	for(i=0;i<cali_time;i++)
	{
	MPU6500_ReadBytes(SPIx,MPU6500_ACCEL_XOUT_H, mpu_buff, 14);	
	gxset	+= mpu_buff[8]<<8 |mpu_buff[9];
	gyset	+= mpu_buff[10]<<8 |mpu_buff[11];
	gzset	+= mpu_buff[12]<<8 |mpu_buff[13];

	//axset	+= mpu_buff[0]<<8 |mpu_buff[1];
	//ayset += mpu_buff[2]<<8 |mpu_buff[3];
	//azset	+= mpu_buff[4]<<8 |mpu_buff[5];
	delay_ms(2);
	}
	gxset/=cali_time;
	gyset/=cali_time;
	gzset/=cali_time;
	//axset/=cali_time;
	//ayset/=cali_time;
	//azset/=cali_time;
	//azset/=4096-cali_time;
}


void IMU_Get_Data(SPI_TypeDef* SPIx)
{
  MPU6500_ReadBytes(SPIx,MPU6500_ACCEL_XOUT_H, mpu_buff, 14);	
	IST8310_ReadBytes(SPIx,IST8310_R_XL,&mpu_buff[14],6);
  IMUData.ax = mpu_buff[0]<<8 |mpu_buff[1];
  IMUData.ay = mpu_buff[2]<<8 |mpu_buff[3];
  IMUData.az = mpu_buff[4]<<8 |mpu_buff[5];
  
  IMUData.temp = mpu_buff[6]<<8 |mpu_buff[7];
  
  IMUData.gx = mpu_buff[8]<<8 |mpu_buff[9];
  IMUData.gy = mpu_buff[10]<<8 |mpu_buff[11];
  IMUData.gz = mpu_buff[12]<<8 |mpu_buff[13];
	
	IMUData.mx = mpu_buff[15]<<8 |mpu_buff[14];
  IMUData.my = mpu_buff[17]<<8 |mpu_buff[16];
  IMUData.mz = mpu_buff[19]<<8 |mpu_buff[18];
}




typedef __packed struct
{
	int16_t MaxMagX;
	int16_t MaxMagY;
	int16_t MaxMagZ;
	int16_t MinMagX;
	int16_t MinMagY;
	int16_t MinMagZ;
}MagMaxMinData_t;
MagMaxMinData_t MagMaxMinData;
int16_t MPU6500_FIFO[10][11];//第一个代表寄存器第二个代表采样
uint8_t isMPU6500_is_DRY=0;

void MPU6500_getMotion(IMUDataTypedef *IMU)
{
	uint8_t i = 0;
	int32_t sum = 0;
	if (isMPU6500_is_DRY)
	{
		isMPU6500_is_DRY = 0;
		IMU_Get_Data(SPI5);

		for (i = 1; i<10; i++)
		{
			MPU6500_FIFO[0][i - 1] = MPU6500_FIFO[0][i];
			MPU6500_FIFO[1][i - 1] = MPU6500_FIFO[1][i];
			MPU6500_FIFO[2][i - 1] = MPU6500_FIFO[2][i];
			MPU6500_FIFO[3][i - 1] = MPU6500_FIFO[3][i];
			MPU6500_FIFO[4][i - 1] = MPU6500_FIFO[4][i];
			MPU6500_FIFO[5][i - 1] = MPU6500_FIFO[5][i];
      MPU6500_FIFO[6][i - 1] = MPU6500_FIFO[6][i];
			MPU6500_FIFO[7][i - 1] = MPU6500_FIFO[7][i];
			MPU6500_FIFO[8][i - 1] = MPU6500_FIFO[8][i];
			MPU6500_FIFO[9][i - 1] = MPU6500_FIFO[9][i];
		}

		MPU6500_FIFO[0][9] = IMUData.ax-(int16_t)axset;//实际值减去修正值
		MPU6500_FIFO[1][9] = IMUData.ay-(int16_t)ayset;//实际值减去修正值
		MPU6500_FIFO[2][9] = IMUData.az-(int16_t)ayset;//实际值减去修正值
		MPU6500_FIFO[3][9] = IMUData.gx-(int16_t)gxset;//实际值减去修正值
		MPU6500_FIFO[4][9] = IMUData.gy-(int16_t)gyset;//实际值减去修正值
		MPU6500_FIFO[5][9] = IMUData.gz-(int16_t)gzset;//实际值减去修正值
    MPU6500_FIFO[6][9] = IMUData.mx-(int16_t)mxset;//实际值减去修正值
    MPU6500_FIFO[7][9] = IMUData.my-(int16_t)myset;//实际值减去修正值
    MPU6500_FIFO[8][9] = IMUData.mz-(int16_t)mzset;//实际值减去修正值
		MPU6500_FIFO[9][9] = IMUData.temp;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[0][i];
		}
		MPU6500_FIFO[0][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[1][i];
		}
		MPU6500_FIFO[1][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[2][i];
		}
		MPU6500_FIFO[2][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[3][i];
		}
		MPU6500_FIFO[3][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[4][i];
		}
		MPU6500_FIFO[4][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[5][i];
		}
    MPU6500_FIFO[5][10] = sum / 10;
		sum = 0;
    for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[6][i];
		}
		MPU6500_FIFO[6][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[7][i];
		}
		MPU6500_FIFO[7][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[8][i];
		}
		MPU6500_FIFO[8][10] = sum / 10;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6500_FIFO[9][i];
		}
		MPU6500_FIFO[9][10] = sum / 10;
		sum = 0;
  if(MagMaxMinData.MinMagX>MPU6500_FIFO[6][10])
	{
		MagMaxMinData.MinMagX=(int16_t)MPU6500_FIFO[6][10];
	}
	if(MagMaxMinData.MinMagY>MPU6500_FIFO[7][10])
	{
		MagMaxMinData.MinMagY=(int16_t)MPU6500_FIFO[7][10];
	}
	if(MagMaxMinData.MinMagZ>MPU6500_FIFO[8][10])
	{
		MagMaxMinData.MinMagZ=(int16_t)MPU6500_FIFO[8][10];
	}

	if(MagMaxMinData.MaxMagX<MPU6500_FIFO[6][10])
	{
		MagMaxMinData.MaxMagX=(int16_t)MPU6500_FIFO[6][10];		
	}
	if(MagMaxMinData.MaxMagY<MPU6500_FIFO[7][10])
	{
		MagMaxMinData.MaxMagY = MPU6500_FIFO[7][10];
	}
	if(MagMaxMinData.MaxMagZ<MPU6500_FIFO[8][10])
	{
		MagMaxMinData.MaxMagZ=(int16_t)MPU6500_FIFO[8][10];
	}	
		IMU->ax = MPU6500_FIFO[0][10];
		IMU->ay = MPU6500_FIFO[1][10];
		IMU->az = MPU6500_FIFO[2][10];
		IMU->gx = MPU6500_FIFO[3][10];
		IMU->gy = MPU6500_FIFO[4][10];
		IMU->gz = MPU6500_FIFO[5][10];
    IMU->mx = MPU6500_FIFO[6][10];
		IMU->my = MPU6500_FIFO[7][10];
		IMU->mz = MPU6500_FIFO[8][10];
		IMU->temp = MPU6500_FIFO[9][10];
	}
	else
	{
		IMU->ax = MPU6500_FIFO[0][10];
		IMU->ay = MPU6500_FIFO[1][10];
		IMU->az = MPU6500_FIFO[2][10];
		IMU->gx = MPU6500_FIFO[3][10];
		IMU->gy = MPU6500_FIFO[4][10];
		IMU->gz = MPU6500_FIFO[5][10];
    IMU->mx = MPU6500_FIFO[6][10];
		IMU->my = MPU6500_FIFO[7][10];
		IMU->mz = MPU6500_FIFO[8][10];
		IMU->temp = MPU6500_FIFO[9][10];
	}
}



void IMU_getValues(IMUDataTypedef_t *IMU) 
{	
	MPU6500_getMotion(&IMUDataR);
	IMU->ax=IMUDataR.ax/4096.0f;//
	IMU->ay=IMUDataR.ay/4096.0f;
  IMU->az=IMUDataR.az/4096.0f;//	
	IMU->gx=IMUDataR.gx/16.384f;//
	IMU->gy=IMUDataR.gy/16.384f;
	IMU->gz=IMUDataR.gz/16.384f;//
//	32.8f
//	IMU->gx=IMUDataR.gx/32.8f;//
//	IMU->gy=IMUDataR.gy/32.8f;
//	IMU->gz=IMUDataR.gz/32.8f;//

	IMU->mx=IMUDataR.mx;	
	IMU->my=IMUDataR.my;
  IMU->mz=IMUDataR.mz;
	IMU->temp=21+IMUDataR.temp/333.87f;
}

float q0 = 1.0f;
float q1 = 0.0f;
float q2 = 0.0f;
float q3 = 0.0f;
static  float q[4]; //　四元数
void IMU_getQ( float * q) {

	IMU_getValues(&IMU);	 //获取原始数据,加速度计和磁力计是原始值，陀螺仪转换成了deg/s
	IMU_AHRSupdate(&IMU);
	q[0] = q0; //返回当前值
	q[1] = q1;
	q[2] = q2;
	q[3] = q3;
}

#define __MAG
#define M_PI  (float)3.1415926535
uint32_t lastUpdate, now; // 采样周期计数 单位 us  
float exInt, eyInt, ezInt;  // 误差积分
float gx, gy, gz, ax, ay, az, mx, my, mz,halfT;   //作用域仅在此文件中
void IMU_AHRSupdate(IMUDataTypedef_t *IMU) 
	{
		float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;
    float tempq0,tempq1,tempq2,tempq3;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;   
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;   

    gx = IMU->gx/57.3f;
    gy = IMU->gy/57.3f;
    gz = IMU->gz/57.3f;
    ax = IMU->ax;
    ay = IMU->ay;
    az = IMU->az;
    mx = IMU->mx;
    my = IMU->my;
    mz = IMU->mz;		

    now = Get_Time_Micros();  //读取时间 单位是us   

		if(now>=lastUpdate)			
    {
        halfT =  ((float)(now - lastUpdate) / 2000000.0f);
    }
    lastUpdate = now;	//更新时间
    //快速求平方根算法
    norm = invSqrt(ax*ax + ay*ay + az*az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
#ifdef __MAG
		#define Kp 20.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
		#define Ki 0.001f   // integral gain governs rate of convergence of gyroscope biases
    //把加计的三维向量转成单位向量。
    norm = invSqrt(mx*mx + my*my + mz*mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm; 
    // compute reference direction of flux
    hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
    hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
    //hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2); 
    // estimated direction of gravity and flux (v and w)
    vx = 2.0f*(q1q3 - q0q2);
    vy = 2.0f*(q0q1 + q2q3);
		vz = q0q0 - 0.5f + q3q3;
    //vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
    wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
    wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  
    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);
#else
		#define Kp 2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
		#define Ki 0.001f   // integral gain governs rate of convergence of gyroscope biases
		vx = 2.0f*(q1q3 - q0q2);
    vy = 2.0f*(q0q1 + q2q3);
		vz = q0q0 - 0.5f + q3q3;
    //vz = q0q0 - q1q1 - q2q2 + q3q3;
		ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);
#endif
    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;	
        ezInt = ezInt + ez * Ki * halfT;
        // 用叉积误差来做PI修正陀螺零偏
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;
    }
    // 四元数微分方程
    tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

    // 四元数规范化
    norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
    q0 = tempq0 * norm;
    q1 = tempq1 * norm;
    q2 = tempq2 * norm;
    q3 = tempq3 * norm;

}
	

float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


void IMU_getYawPitchRoll(float * angles)
{
	IMU_getQ(q); //更新全局四元数
							 //四元数转换成欧拉角，经过三角函数计算即可
	angles[0] = -atan2(2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2] * q[2] - 2 * q[3] * q[3] + 1) * 180 / M_PI; // yaw        -pi----pi
	angles[1] = -asin(-2 * q[1] * q[3] + 2 * q[0] * q[2]) * 180 / M_PI; // pitch    -pi/2    --- pi/2 
	angles[2] = atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1) * 180 / M_PI; // roll       -pi-----pi  
}



static int yaw_count = 0;
float yaw_temp, pitch_temp, roll_temp;
float last_yaw_temp, last_pitch_temp, last_roll_temp;
float yaw_angle, pitch_angle, roll_angle; //使用到的角度值
float angle[3] = { 0 };


void GetPitchYawGxGyGz(void)
{
	last_yaw_temp = yaw_temp;
	yaw_temp = angle[0];
	if (yaw_temp - last_yaw_temp >= 330)  //yaw轴角度经过处理后变成连续的
	{
		yaw_count--;
	}
	else if (yaw_temp - last_yaw_temp <= -330)
	{
		yaw_count++;
	}
	yaw_angle = yaw_temp + yaw_count * 360;  //yaw轴角度
	pitch_angle = angle[1];
	roll_angle = angle[2];
}