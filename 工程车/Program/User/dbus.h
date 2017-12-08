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

/* ----------------------- Data Struct ------------------------------------- */
extern uint8_t DBUS_BUFFER[19];
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
 }DBUS_TypeDef;//遥控解算后的结构体
 
typedef enum control_mode_t
{
	prepare=4,
	keyboard=1,
	remote=3,
	stop=0,
}control_mode_t;//控制模式

extern control_mode_t control_mode;

typedef struct ChassisSend_t
{
	float X;
	float XMax;
	float Y;
	float YMax;
	float rotate;
}ChassisSend_t;//底盘速度
extern ChassisSend_t ChassisSend;


extern DBUS_TypeDef DBUS;
 void dbus_getdata(void);
void status_check(DBUS_TypeDef *dbus);
void remote_s2_check(DBUS_TypeDef *dbus);
void remote_s1_check(DBUS_TypeDef *dbus);
void remote_control(DBUS_TypeDef *dbus);//遥控模式
void keyboard_control(DBUS_TypeDef *dbus);//键盘模式
void keyboard_k_check(DBUS_TypeDef *dbus);
#endif


    