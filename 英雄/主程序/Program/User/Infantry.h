#ifndef __INFANTRY_H
#define __INFANTRY_H

			#define init_time_ms 2000//初始化斜坡（ms*2）
			
			#define CmSPeedR_countmax 100//遥控启动速度斜坡
			#define CmSPeedK_countmax 500//键盘启动速度斜坡
			#define CMRotateR_countmax 100//键盘启动速度斜坡
			#define CMRotateK_countmax 100//键盘启动速度斜坡
			#define CMStop_countmax 100//停止启动速度斜坡


			//云台初始化角度
			#define GMPitchAngle_Default	0xC18

			//拨弹舵机和摩擦轮速度
			#define friction_countmax 50//摩擦轮斜坡	（ms*3）
			#define FRISpeed 300//摩擦轮速度
			#define FRIBSpeed 280//摩擦轮速度
			#define pok_stopcountmax 200//拨弹舵机开始时间（ms*3）
			#define pok_startcountmax 200//拨弹舵机结束时间（ms*3）
			
			#define sg_stopcountmax 120//振弹舵机开始时间（ms*3）
			#define sg_startcountmax 120//振弹舵机结束时间（ms*3）
			//全方向运动最大值
			#define MaxWheelSpeed 200
			#define MaxWheelPower 75//功率最大值
			//遥控底盘速度
			#define ChassisYRate 0.2f  	//底盘纵向遥控和速度的比率
			#define ChassisXRate 0.03f	 	//底盘横向遥控和速度的比率
			
			#define ChassisYHRate 0.3f  	//底盘纵向遥控和速度的比率
			#define ChassisXHRate 0.10f	 	//底盘横向遥控和速度的比率
			//遥控转弯
			#define ChassisRRate 0.035f	 	//底盘横向遥控和速度的比率
			#define ChassisRHRate 0.07f	 	//底盘横向遥控和速度的比率
			#define GMPRate  		0.00025f	//遥控pitch
			//鼠标转弯
			#define ChassisKRRate 0.7f	 	//底盘横向遥控和速度的比率
			#define ChassisKRHRate 1.0f	 	//底盘横向遥控和速度的比率
			#define GMPKRate  		0.0025f	//鼠标pitch

			//------PITCH位置初始值-----//
			#define GMPPositionPID_default { 0.0f,	 0.0f, 0.0f, 	0, 	1000, 	800 , 	0, 0, 0, 0,	{0,0,0}, {0,0,0},	{0,0,0}, &Pid_Test, &Pid_Reset,}//pitch位置
			//------PITCH速度初始值-----//
			#define GMPSpeedPID_default    { 0.0f,  0.0f, 0.0f, 	0, 	1000, 	4900, 	0, 0, 0, 0, {0,0,0}, {0,0,0}, {0,0,0}, &Pid_Test, &Pid_Reset,}//pitch速度
			//------底盘电机速度初始值-----//
			#define CMSpeedPID_default     { 220.0f, 0.0f, 0.4f, 	0, 	4900, 	8000,		0, 0, 0, 0, {0,0,0}, {0,0,0}, {0,0,0}, &Pid_Test, &Pid_Reset,}//1轮速度

#endif