#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f4xx.h"
extern uint8_t mpubuffer[6];
extern uint8_t mpubuffer_x[6];
#define M_PI  (float)3.1415926535
//����MPU6050�ڲ���ַ
#define	SMPLRT_DIV		          0x19	//�����ǲ����� ����ֵ 0X07 125Hz
#define	CONFIG			          0x1A	//��ͨ�˲�Ƶ�� ����ֵ 0x00 
#define	GYRO_CONFIG		          0x1B	//�������Լ켰������Χ                 ����ֵ 0x18 ���Լ� 2000deg/s
#define	ACCEL_CONFIG	          0x1C	//���ٶȼ��Լ켰������Χ����ͨ�˲�Ƶ�� ����ֵ 0x01 ���Լ� 2G 5Hz
#define INT_PIN_CFG               0x37
#define INT_ENABLE                0x38
#define INT_STATUS                0x3A    //ֻ��
#define	ACCEL_XOUT_H	          0x3B
#define	ACCEL_XOUT_L	          0x3C
#define	ACCEL_YOUT_H	          0x3D
#define	ACCEL_YOUT_L	          0x3E
#define	ACCEL_ZOUT_H	          0x3F
#define	ACCEL_ZOUT_L	          0x40
#define	TEMP_OUT_H		          0x41
#define	TEMP_OUT_L		          0x42
#define	GYRO_XOUT_H		          0x43
#define	GYRO_XOUT_L		          0x44	
#define	GYRO_YOUT_H		          0x45
#define	GYRO_YOUT_L		          0x46
#define	GYRO_ZOUT_H		          0x47
#define	GYRO_ZOUT_L		          0x48
#define	PWR_MGMT_1		          0x6B	//��Դ���� ����ֵ 0x00 ��������
#define	WHO_AM_I		            0x75	//ֻ��  Ĭ�϶���Ӧ���� MPU6050_ID = 0x68
#define MPU6050_ID                0x68


#define MPU6050_DATA_START        ACCEL_XOUT_H   //�������ݴ�ŵ�ַ�������ģ�����һ������
#define MPU6050_RA_SELF_TEST_X                  0x0D
#define MPU6050_RA_SLEF_TEST_Y                  0x0E
#define MPU6050_RA_SELF_TEST_Z                  0x0F
#define MPU6050_RA_SELF_TEST_A                  0x10
#define MPU6050_RA_SMPLRT_DIV                   0x19
#define MPU6050_RA_CONFIG                       0x1A
#define MPU6050_RA_GYRO_CONFIG                  0x1B
#define MPU6050_RA_ACCEL_CONFIG                 0x1C
#define MPU6050_RA_FIFO_EN                      0x23
#define MPU6050_RA_I2C_MST_CTRL                 0x24
#define MPU6050_RA_I2C_MST_STATUS               0x36
#define MPU6050_RA_INT_PIN_CFG                  0x37
#define MPU6050_RA_INT_ENABLE                   0x38
#define MPU6050_RA_INT_STATUS                   0x3A
#define MPU6050_RA_ACCEL_XOUT_H                 0x3B
#define MPU6050_RA_ACCEL_XOUT_L                 0x3C
#define MPU6050_RA_ACCEL_YOUT_H                 0x3D
#define MPU6050_RA_ACCEL_YOUT_L                 0x3E
#define MPU6050_RA_ACCEL_ZOUT_H                 0x3F
#define MPU6050_RA_ACCEL_ZOUT_L                 0x40
#define MPU6050_RA_TEMP_OUT_H                   0x41 
#define MPU6050_RA_TEMP_OUT_L                   0x42
#define MPU6050_RA_GYRO_XOUT_H                  0x43
#define MPU6050_RA_GYRO_XOUT_L                  0x44
#define MPU6050_RA_GYRO_YOUT_H                  0x45
#define MPU6050_RA_GYRO_YOUT_L                  0x46
#define MPU6050_RA_GYRO_ZOUT_H                  0x47
#define MPU6050_RA_GYRO_ZOUT_L                  0x48       
#define MPU6050_RA_I2C_MST_DELAY_CTRL           0x67
#define MPU6050_RA_SIGNAL_PATH_RESET            0x68
#define MPU6050_RA_USER_CTRL                    0x6A
#define MPU6050_RA_PWR_MGMT_1                   0x6B
#define MPU6050_RA_PWR_MGMT_2                   0x6C
#define MPU6050_RA_FIFO_COUNTH                  0x72
#define MPU6050_RA_FIFO_COUNTL                  0x73
#define MPU6050_RA_FIFO_R_W                     0x74
#define MPU6050_RA_WHO_AM_I                     0x75 
#define MPU6050_RA_SLV0_ADDR                    0x25
#define MPU6050_RA_SLV0_REG                     0x26
#define MPU6050_RA_SLV0_CTRL                    0x27 
#define MPU6050_RA_SLV0_DO                      0x63 
#define MPU6050_RA_SLV1_ADDR                    0x28
#define MPU6050_RA_SLV1_REG                     0x29
#define MPU6050_RA_SLV1_CTRL                    0x2A
#define MPU6050_RA_SLV1_DO                      0x64 
#define MPU6050_RA_SLV4_CTRL                    0x34 
#define MPU6050_RA_ES_DATA00                    0x49
#define MPU6050_RA_ES_DATA01                    0x4A
#define MPU6050_RA_ES_DATA02                    0x4B
#define MPU6050_RA_ES_DATA03                    0x4C
#define MPU6050_RA_ES_DATA04                    0x4D
#define MPU6050_RA_ES_DATA05                    0x4E
#define MPU6050_RA_ES_DATA06                    0x4F
#define MPU6050_RA_ES_DATA07                    0x50


#define HMC5883_ID                          0x1E


// HMC58X3 register map. For details see HMC58X3 datasheet
#define HMC58X3_R_CONFA 0
#define HMC58X3_R_CONFB 1
#define HMC58X3_R_MODE 2
#define HMC58X3_R_XM 3
#define HMC58X3_R_XL 4

#define HMC58X3_R_YM (7)  //!< Register address for YM.
#define HMC58X3_R_YL (8)  //!< Register address for YL.
#define HMC58X3_R_ZM (5)  //!< Register address for ZM.
#define HMC58X3_R_ZL (6)  //!< Register address for ZL.

#define HMC58X3_R_STATUS 9
#define HMC58X3_R_IDA 10
#define HMC58X3_R_IDB 11
#define HMC58X3_R_IDC 12

//#define MPU6050_DEVICE_ID                       0x68
#define HMC5883_DEVICE_ID_A                     0x48
uint8_t mpu6050_init(void);
uint8_t mpu6050_enable(void);
uint8_t	MPU6050_WriteByte(uint8_t Addr, uint8_t reg, uint8_t Data);

uint8_t MPU6050_ReadByte(uint8_t Addr, uint8_t reg, uint8_t *pbuffer);

uint8_t	MPU6050_Read(uint8_t *pbuffer);

uint8_t HMC5883_ReadByte(uint8_t Addr, uint8_t reg, uint8_t *pbuffer);

void HMC5883_Read(uint8_t *vbuff);

uint8_t	HMC5883_WriteByte(uint8_t Addr, uint8_t reg, uint8_t Data);



float invSqrt(float x);
void IMU_AHRSupdate(void);
void IMU_getQ(float * q);
void IMU_getValues(float * values);
void MPU6050_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz,int16_t* mx, int16_t* my, int16_t* mz);
void IMU_getYawPitchRoll(float * angles);
void GetPitchYawGxGyGz(void);
typedef struct
{
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
	float mx;
	float my;
	float mz;

}MPU_DATA;
//extern MPU_DATA MPU_RAW_DATA;
extern MPU_DATA MPU_REAL_DATA;
#endif
