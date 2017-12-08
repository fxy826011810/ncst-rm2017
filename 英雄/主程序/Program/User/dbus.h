#ifndef __DBUS_H
#define __DBUS_H
#include "stm32f4xx.h"
/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN	((uint16_t)364 )
#define RC_CH_VALUE_OFFSET	((uint16_t)1024)
#define RC_CH_VALUE_MAX	((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP	((uint16_t)1)
#define RC_SW_MID	((uint16_t)3)
#define RC_SW_DOWN	((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_W	((uint16_t)0x01<<0)
#define KEY_S	((uint16_t)0x01<<1)
#define KEY_A	((uint16_t)0x01<<2)
#define KEY_D	((uint16_t)0x01<<3)
#define KEY_Q	((uint16_t)0x01<<4)
#define KEY_E	((uint16_t)0x01<<5)
#define KEY_SHIFT	((uint16_t)0x01<<6)
#define KEY_CTRL	((uint16_t)0x01<<7)
#define KEY_V		0x4000
#define KEY_C		0x2000
#define KEY_X		0x1000
#define KEY_Z		0x0800
#define KEY_G		0x0400
#define KEY_F		0x0200
#define KEY_R		0x0100
/* ----------------------- Data Struct ------------------------------------- */
 typedef struct
 {
	 struct
	 {
		int16_t ch0; 
		int16_t ch1; 
		int16_t ch2; 
		int16_t ch3; 
		uint8_t s1; 
		uint8_t s2;
	 }rc;
	 struct
	 {
		 int16_t x; 
		 int16_t y; 
		 int16_t z; 
		 uint8_t press_l; 
		 uint8_t press_r;
	 }mouse;

	 struct
	 {
		 uint16_t v;
	 }key;
 }DBUS_TypeDef;//ң�ؽ����Ľṹ��
typedef enum control_mode_t
{
	stop=0,//ֹͣ״̬
	MechArm=5,//��е�ֱ�
	keyboard=1,//����״̬
	noremote=2,//ң�ض�ʧ״̬
	remote=3,//ң��״̬
	prepare=4,//׼��״̬
}control_mode_t;//����ģʽ
typedef struct GMRemoteSend_t
{
	float pitch_angle;
	float yaw_angle;
}GMRemoteSend_t;//��̨�Ƕ�
typedef struct ChassisSend_t
{
	float X;
	float LAST_X;
	float XMax;
	float Y;
	float LAST_Y;
	float YMax;
	float rotate;
	float LAST_rotate;
}ChassisSend_t;//�����ٶ�
typedef enum GUNMode_t
{
	FRION=1,//Ħ����״̬
	FRIOFF=2,
  POKON=3,//�������״̬
	POKOFF=4,
	FRIWAIT=5,
	SGON=6,
	SGOFF=7,
}GUNMode_t;//ǹ״̬
extern ChassisSend_t ChassisSend;
extern GMRemoteSend_t GMRemoteSend;
extern uint8_t DBUS_BUFFER[19];
extern control_mode_t control_mode;
extern DBUS_TypeDef DBUS;

void dbus_reset(DBUS_TypeDef *dbus);
void dbus_calc(DBUS_TypeDef *dbus);
void Control_Mode_Change(DBUS_TypeDef *dbus);
void remote_s2_check(DBUS_TypeDef *dbus);

void remote_control(DBUS_TypeDef *dbus);//ң��ģʽ
void keyboard_control(DBUS_TypeDef *dbus);//����ģʽ
void keyboard_k_check(DBUS_TypeDef *dbus);

#endif


    