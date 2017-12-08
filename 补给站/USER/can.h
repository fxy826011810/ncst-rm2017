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
	int32_t raw_value;   									//���������������ԭʼֵ
	int32_t last_raw_value;								//��һ�εı�����ԭʼֵ
	int32_t ecd_value;                       //��������������ı�����ֵ
	int32_t diff;													//���α�����֮��Ĳ�ֵ
	int32_t temp_count;                   //������
	uint8_t buf_count;								//�˲�����buf��
	int32_t ecd_bias;											//��ʼ������ֵ	
	int32_t ecd_raw_rate;									//ͨ������������õ����ٶ�ԭʼֵ
	int32_t rate_buf[RATE_BUF_SIZE];	//buf��for filter
	int32_t round_cnt;										//Ȧ��
	float filter_rate;											//�ٶ�
	float ecd_angle;											//�Ƕ�
}Encoder;

extern volatile Encoder GMYawEncoder;
extern volatile Encoder GMPitchEncoder;

__CAN_EXT CanRxMsg		receivemessage;


 void Bsp_Can_Init(void);
 void gm_senddata(CAN_TypeDef* CANx, int num);
 void Can_RecviveData(CanRxMsg * rec);//����ֵ����
 void EncoderProcess(volatile Encoder *v, CanRxMsg * msg);

#endif

