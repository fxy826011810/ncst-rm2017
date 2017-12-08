#ifndef __JUDGE_H
#define __JUDGE_H
#include "stm32f4xx.h"

#define JudgeBufferLength       150
#define JudgeFrameLength_1      46
#define JudgeFrameLength_2      11
#define JudgeFrameLength_3      24

#define JudgeFrameHeader        0xA5        //帧头 

typedef __packed struct
{
uint8_t flag; //0 无效，1 有效
uint32_t x;
uint32_t y;
uint32_t z;
uint32_t compass;
}tLocData;

typedef __packed struct
{
uint32_t remainTime;
uint16_t remainLifeValue;
float realChassisOutV;
float realChassisOutA;
float realChassisOutW;
tLocData locData;
float remainPower;
}tGameInfo;
extern tGameInfo game;

typedef __packed struct
{
uint8_t weakId:4;
uint8_t way:4;
uint16_t value;
}tRealBloodChangedData;

typedef __packed struct
{
float realBulletShootSpeed;
float realBulletShootFreq;
float realGolfShootSpeed;
float realGolfShootFreq;
}tRealShootData;
extern tRealShootData shoot;
extern uint8_t JudgeDataBuffer[JudgeBufferLength];


unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);


#endif 