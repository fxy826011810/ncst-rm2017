#ifndef __CAN_H
#define __CAN_H
#include "stm32f10x.h"
#define __CAN_EXT extern
#define RATE_BUF_SIZE 6
typedef enum
{
 CAN_EC60_ID				=0x200,
 CAN_EC60_1_ID			=0x201,
 CAN_EC60_2_ID			=0x202,
 CAN_EC60_3_ID			=0x203,
 CAN_EC60_4_ID			=0x204,
}CAN_ID;

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

extern volatile Encoder CM1Encoder;
extern volatile Encoder CM2Encoder;
extern volatile Encoder CM3Encoder;
extern volatile Encoder CM4Encoder;
__CAN_EXT CanRxMsg		receivemessage;


void Bsp_Can_Init(void);
void cm_senddata(CAN_TypeDef* CANx, int16_t num1, int16_t num2, int16_t num3, int16_t num4);
void Can_RecviveData(CanRxMsg * rec);//����ֵ����
void EncoderProcess(volatile Encoder *v, CanRxMsg * msg);

#endif

