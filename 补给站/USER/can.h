#ifndef __CAN_H
#define __CAN_H
#include "stm32f10x.h"
#define __CAN_EXT extern
#define RATE_BUF_SIZE 6
typedef enum
{
	CAN_6623_24vID					=0x1FF,
	CAN_6623_12vID  				=0x200,
	CAN_6623_PITCH12v_ID		=0x201,
	CAN_6623_PITCH24v_ID		=0x206,
}CAN_ID;

typedef enum 
	{
		_12v,
		_24v,
	} DJ_status_t;
	
extern DJ_status_t DJ_status;

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

extern volatile Encoder GMYawEncoder;
extern volatile Encoder GMPitchEncoder;

__CAN_EXT CanRxMsg		receivemessage;


 void Bsp_Can_Init(void);
 void gm_senddata(CAN_TypeDef* CANx, int num);
 void Can_RecviveData(CanRxMsg * rec);//返回值解算
 void EncoderProcess(volatile Encoder *v, CanRxMsg * msg);

#endif

