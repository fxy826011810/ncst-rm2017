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
 }DBUS_TypeDef;//Ò£¿Ø½âËãºóµÄ½á¹¹Ìå
typedef enum control_mode_t
{
	stop=0,//Í£Ö¹×´Ì¬
	MechArm=5,//»úÐµÊÖ±Û
	keyboard=1,//¼üÅÌ×´Ì¬
	noremote=2,//Ò£¿Ø¶ªÊ§×´Ì¬
	remote=3,//Ò£¿Ø×´Ì¬
	prepare=4,//×¼±¸×´Ì¬
}control_mode_t;//¿ØÖÆÄ£Ê½
typedef struct GMRemoteSend_t
{
	float pitch_angle;
	float yaw_angle;
}GMRemoteSend_t;//ÔÆÌ¨½Ç¶È
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
}ChassisSend_t;//µ×ÅÌËÙ¶È
typedef enum GUNMode_t
{
	FRION=1,//Ä¦²ÁÂÖ×´Ì¬
	FRIOFF=2,
  POKON=3,//²¦µ¯µç»ú×´Ì¬
	POKOFF=4,
	FRIWAIT=5,
	SGON=6,
	SGOFF=7,
}GUNMode_t;//Ç¹×´Ì¬
extern ChassisSend_t ChassisSend;
extern GMRemoteSend_t GMRemoteSend;
extern uint8_t DBUS_BUFFER[19];
extern control_mode_t control_mode;
extern DBUS_TypeDef DBUS;

void dbus_reset(DBUS_TypeDef *dbus);
void dbus_calc(DBUS_TypeDef *dbus);
void Control_Mode_Change(DBUS_TypeDef *dbus);
void remote_s2_check(DBUS_TypeDef *dbus);

void remote_control(DBUS_TypeDef *dbus);//Ò£¿ØÄ£Ê½
void keyboard_control(DBUS_TypeDef *dbus);//¼üÅÌÄ£Ê½
void keyboard_k_check(DBUS_TypeDef *dbus);

#endif


    