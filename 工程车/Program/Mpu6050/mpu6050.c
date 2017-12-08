#include "mpu6050.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "i2c.h"
#include "math.h"
#include "tim.h"
#include <stdio.h>


MPU_DATA MPU_RAW_DATA;
MPU_DATA MPU_REAL_DATA;
uint8_t mpu6050_id;
uint8_t hmc5883_addr;

uint8_t mpu6050_init(void)
{
	
	

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



	if (!HMC5883_ReadByte(HMC5883_ID, HMC58X3_R_IDA,&hmc5883_addr))
	{
		if (hmc5883_addr != HMC5883_DEVICE_ID_A)
		{
			printf("HMC5883_DEVICE_ID_A error\r\n");
			return 1; //校验失败，返回0xff
		}
	}
	else
	{
		printf("HMC5883_DEVICE_ID_A read error\r\n");
		return 1;
	}
	if (HMC5883_WriteByte(HMC5883_ID, HMC58X3_R_CONFA, 0x70))
	{
		printf("HMC58X3_R_CONFA error\r\n");
		return 1;
	}
	delay_ms(5);
	if (HMC5883_WriteByte(HMC5883_ID, HMC58X3_R_CONFB, 0xA0))
	{
		printf("HMC58X3_R_CONFB error\r\n");
		return 1;
	}
	delay_ms(5);
	if (HMC5883_WriteByte(HMC5883_ID, HMC58X3_R_MODE, 0x00))
	{
		printf("HMC58X3_R_MODE error\r\n");
		return 1;
	}
	delay_ms(6);
	if (HMC5883_WriteByte(HMC5883_ID, HMC58X3_R_CONFA, 6 << 2))
	{
		printf("HMC58X3_R_CONFA error\r\n");
		return 1;
	}
	delay_ms(500);
	return 0;
}
uint8_t mpu6050_enable(void)
{
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
uint8_t HMC5883_ReadByte(uint8_t Addr, uint8_t reg, uint8_t *pbuffer)
{
	if (IIC_ReadByte(Addr, reg, pbuffer))
	{
		printf("HMC5883_ReadByte error");
		return 1;
	}
	return 0;
}
void HMC5883_Read(uint8_t *vbuff)
{
	IIC_Read(HMC5883_ID, HMC58X3_R_XM, vbuff, 6);
}
uint8_t	HMC5883_WriteByte(uint8_t Addr, uint8_t reg, uint8_t Data)
{
	if (IIC_WriteByte(Addr, reg, Data))
	{
		printf("HMC5883_WriteByte error");
		return 1;
	}
	return 0;
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
int16_t MPU6050_FIFO[9][11];//第一个代表寄存器第二个代表采样
uint8_t mpu_buf[20],isMPU6050_is_DRY=0;
int16_t gxset=-107,gyset=36,gzset=52;
void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz,int16_t* mx, int16_t* my, int16_t* mz)
{
	uint8_t i = 0;
	int32_t sum = 0;
	if (isMPU6050_is_DRY)
	{
		isMPU6050_is_DRY = 0;
		MPU6050_Read(mpu_buf);
		HMC5883_Read(&mpu_buf[14]);

		for (i = 1; i<10; i++)
		{
			MPU6050_FIFO[0][i - 1] = MPU6050_FIFO[0][i];
			MPU6050_FIFO[1][i - 1] = MPU6050_FIFO[1][i];
			MPU6050_FIFO[2][i - 1] = MPU6050_FIFO[2][i];
			MPU6050_FIFO[3][i - 1] = MPU6050_FIFO[3][i];
			MPU6050_FIFO[4][i - 1] = MPU6050_FIFO[4][i];
			MPU6050_FIFO[5][i - 1] = MPU6050_FIFO[5][i];
                        MPU6050_FIFO[6][i - 1] = MPU6050_FIFO[6][i];
			MPU6050_FIFO[7][i - 1] = MPU6050_FIFO[7][i];
			MPU6050_FIFO[8][i - 1] = MPU6050_FIFO[8][i];
		}

		MPU6050_FIFO[0][9] = ((int16_t)mpu_buf[0] << 8)  | mpu_buf[1];
		MPU6050_FIFO[1][9] = ((int16_t)mpu_buf[2] << 8)  | mpu_buf[3];
		MPU6050_FIFO[2][9] = ((int16_t)mpu_buf[4] << 8)  | mpu_buf[5];
		MPU6050_FIFO[3][9] = ((int16_t)mpu_buf[8] << 8)  | mpu_buf[9];
		MPU6050_FIFO[4][9] = ((int16_t)mpu_buf[10] << 8) | mpu_buf[11];
		MPU6050_FIFO[5][9] = ((int16_t)mpu_buf[12] << 8) | mpu_buf[13];
                MPU6050_FIFO[6][9] = ((int16_t)mpu_buf[14] << 8) | mpu_buf[15];
                MPU6050_FIFO[7][9] = ((int16_t)mpu_buf[16] << 8) | mpu_buf[17];
                MPU6050_FIFO[8][9] = ((int16_t)mpu_buf[18] << 8) | mpu_buf[19];
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
                for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[6][i];
		}
		MPU6050_FIFO[6][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[7][i];
		}
		MPU6050_FIFO[7][10] = sum / 10;
		sum = 0;
		for (i = 0; i<10; i++)//求当前数组的合，再取平均值
		{
			sum += MPU6050_FIFO[8][i];
		}
		MPU6050_FIFO[8][10] = sum / 10;
		sum = 0;
                	if(MagMaxMinData.MinMagX>MPU6050_FIFO[6][10])
	{
		MagMaxMinData.MinMagX=(int16_t)MPU6050_FIFO[6][10];
	}
	if(MagMaxMinData.MinMagY>MPU6050_FIFO[7][10])
	{
		MagMaxMinData.MinMagY=(int16_t)MPU6050_FIFO[7][10];
	}
	if(MagMaxMinData.MinMagZ>MPU6050_FIFO[8][10])
	{
		MagMaxMinData.MinMagZ=(int16_t)MPU6050_FIFO[8][10];
	}

	if(MagMaxMinData.MaxMagX<MPU6050_FIFO[6][10])
	{
		MagMaxMinData.MaxMagX=(int16_t)MPU6050_FIFO[6][10];		
	}
	if(MagMaxMinData.MaxMagY<MPU6050_FIFO[7][10])
	{
		MagMaxMinData.MaxMagY = MPU6050_FIFO[7][10];
	}
	if(MagMaxMinData.MaxMagZ<MPU6050_FIFO[8][10])
	{
		MagMaxMinData.MaxMagZ=(int16_t)MPU6050_FIFO[8][10];
	}	
		*ax = MPU6050_FIFO[0][10];//=MPU6050_FIFO[0][10];
		*ay = MPU6050_FIFO[1][10];//=MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10];//=MPU6050_FIFO[2][10];
		*gx = MPU6050_FIFO[3][10]-gxset ;//=MPU6050_FIFO[3][10];
		*gy = MPU6050_FIFO[4][10]-gyset;//=MPU6050_FIFO[4][10];
		*gz = MPU6050_FIFO[5][10]-gzset;//=MPU6050_FIFO[5][10];
                *mx = MPU6050_FIFO[6][10] ;//=MPU6050_FIFO[3][10];
		*my = MPU6050_FIFO[7][10];//=MPU6050_FIFO[4][10];
		*mz = MPU6050_FIFO[8][10];//=MPU6050_FIFO[5][10];
	}
	else
	{
		*ax = MPU6050_FIFO[0][10];//=MPU6050_FIFO[0][10];
		*ay = MPU6050_FIFO[1][10];//=MPU6050_FIFO[1][10];
		*az = MPU6050_FIFO[2][10];//=MPU6050_FIFO[2][10];
		*gx = MPU6050_FIFO[3][10]-gxset ;//=MPU6050_FIFO[3][10];
		*gy = MPU6050_FIFO[4][10]-gyset;//=MPU6050_FIFO[4][10];
		*gz = MPU6050_FIFO[5][10]-gzset;//=MPU6050_FIFO[5][10]
                *mx = MPU6050_FIFO[6][10] ;//=MPU6050_FIFO[3][10];
		*my = MPU6050_FIFO[7][10];//=MPU6050_FIFO[4][10];
		*mz = MPU6050_FIFO[8][10];//=MPU6050_FIFO[5][10];
	}
}
	int16_t accgyroval[9];



void IMU_getValues(float * values) {
	
	uint8_t i;

	//读取加速度和陀螺仪的当前ADC
	MPU6050_getMotion6(&accgyroval[0], &accgyroval[1], &accgyroval[2], &accgyroval[3], &accgyroval[4], &accgyroval[5],&accgyroval[6], &accgyroval[7], &accgyroval[8]);
	
	MPU_RAW_DATA.ax = accgyroval[0];
	MPU_RAW_DATA.ay = accgyroval[1];
	MPU_RAW_DATA.az = accgyroval[2];
	MPU_RAW_DATA.gx = accgyroval[3];
	MPU_RAW_DATA.gy = accgyroval[4];
	MPU_RAW_DATA.gz = accgyroval[5];
        MPU_RAW_DATA.mx = accgyroval[6];
	MPU_RAW_DATA.my = accgyroval[7];
	MPU_RAW_DATA.mz = accgyroval[8];

	for (i = 0; i<6; i++) 
       {
		if (i < 3) {
			values[i] = (float)accgyroval[i];
		}
		else {
			values[i] = ((float)accgyroval[i]) / 32.8f; //转成度每秒
														//这里已经将量程改成了 1000度每秒  32.8 对应 1度每秒
		}
	}
        values[6]=(float)accgyroval[6];;
	values[7]=(float)accgyroval[7];;
	values[8]=(float)accgyroval[8];;
}



float mygetqval[9];
float q0 = 1.0f;
float q1 = 0.0f;
 float q2 = 0.0f;
 float q3 = 0.0f;
static  float q[4]; //　四元数




void IMU_getQ( float * q) {

	IMU_getValues(mygetqval);	 //获取原始数据,加速度计和磁力计是原始值，陀螺仪转换成了deg/s
	IMU_AHRSupdate();
	q[0] = q0; //返回当前值
	q[1] = q1;
	q[2] = q2;
	q[3] = q3;
}



#define Kp 2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.01f   // integral gain governs rate of convergence of gyroscope biases
uint32_t lastUpdate, now; // 采样周期计数 单位 us  
float exInt, eyInt, ezInt;  // 误差积分
static  float gx, gy, gz, ax, ay, az, mx, my, mz;   //作用域仅在此文件中




void IMU_AHRSupdate(void) {


	float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez,halfT;
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

    gx = mygetqval[3] * M_PI/180;
    gy = mygetqval[4] * M_PI/180;
    gz = mygetqval[5] * M_PI/180;
    ax = mygetqval[0];
    ay = mygetqval[1];
    az = mygetqval[2];
    mx = mygetqval[6];
    my = mygetqval[7];
    mz = mygetqval[8];		

    now = Get_Time_Micros();  //读取时间 单位是us   
    if(now<lastUpdate)
    {
    //halfT =  ((float)(now + (0xffffffff- lastUpdate)) / 2000000.0f);   //  uint 0.5s
    }
    else	
    {
        halfT =  ((float)(now - lastUpdate) / 2000000.0f);
    }
    lastUpdate = now;	//更新时间
    //快速求平方根算法
    norm = invSqrt(ax*ax + ay*ay + az*az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    //把加计的三维向量转成单位向量。
    norm = invSqrt(mx*mx + my*my + mz*mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm; 
    // compute reference direction of flux
    hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
    hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
    hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz; 
    // estimated direction of gravity and flux (v and w)
    vx = 2.0f*(q1q3 - q0q2);
    vy = 2.0f*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
    wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
    wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  
    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

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



float invSqrt(float x) {
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
	// volatile float gx=0.0, gy=0.0, gz=0.0; //估计重力方向
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
	MPU_REAL_DATA.gx = mygetqval[3];
	MPU_REAL_DATA.gy = -mygetqval[4];
	MPU_REAL_DATA.gz = mygetqval[5];

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
