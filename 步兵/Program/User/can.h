#ifndef __CAN_H
#define __CAN_H
#include "stm32f4xx.h"
#define __CAN_EXT extern
#define RATE_BUF_SIZE 6
typedef enum
{
 CAN_6623_ID					=0x1FF,
 CAN_6623_PITCH_ID		=0x206,
 CAN_6623_YAW_ID			=0x205,
 
 CAN_RM3510_ID				=0x200,
 CAN_RM3510_1_ID			=0x201,
 CAN_RM3510_2_ID			=0x202,
 CAN_RM3510_3_ID			=0x203,
 CAN_RM3510_4_ID			=0x204,
	
 CAN_GYRO_ID					=0x401,//单轴陀螺仪
}CAN_ID;

typedef struct
{
	int16_t Angle;
	int16_t	RollSpeed;
}CAN_RM3510_REC;


//typedef struct
//{
//	int16_t Angle;
//	int16_t Real_I;
//	int16_t Set_I;
//}CAN_6623_REC;


typedef struct{
	int32_t raw_value;   									//编码器不经处理的原始值
	int32_t last_raw_value;								//上一次的编码器原始值
	int32_t ecd_value;                       //经过处理后连续的编码器值
	int32_t diff;													//两次编码器之间的差值
	int32_t temp_count;                   //计数用
	uint8_t buf_count;								//滤波更新buf用
	int32_t ecd_bias;											//初始编码器值	
	int32_t ecd_raw_rate;									//通过编码器计算得到的速度原始值
	int32_t rate_buf[RATE_BUF_SIZE];	//buf，for filter
	int32_t round_cnt;										//圈数
	float filter_rate;											//速度
	float ecd_angle;											//角度
}Encoder;
extern volatile Encoder CM1Encoder;
extern volatile Encoder CM2Encoder;
extern volatile Encoder CM3Encoder;
extern volatile Encoder CM4Encoder;
extern volatile Encoder GMYawEncoder;
extern volatile Encoder GMPitchEncoder;

__CAN_EXT CanRxMsg		receivemessage;
__CAN_EXT CAN_RM3510_REC		cm1_rec;
__CAN_EXT CAN_RM3510_REC		cm2_rec;
__CAN_EXT CAN_RM3510_REC		cm3_rec;
__CAN_EXT CAN_RM3510_REC		cm4_rec;


 void Bsp_Can_Init(void);

 void cm_senddata(CAN_TypeDef* CANx, int num1, int num2, int num3, int num4);
 void gm_senddata(CAN_TypeDef* CANx, int num1, int num2);
void gyro_senddata(void);
 void Can_RecviveData(CanRxMsg * rec);
void EncoderProcess(volatile Encoder *v, CanRxMsg * msg);
#endif

