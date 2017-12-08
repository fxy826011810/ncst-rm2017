#ifndef __INFANTRY_H
#define __INFANTRY_H

#define Infantry_2
//#define Infantry_3


#ifdef Infantry_3
			//跟随pid
			#define crkp	80 
			#define	crki	0
			#define	crkd	0
			
			#define init_time_ms 8000//初始化斜坡
			#define friction_countmax 50//摩擦轮斜坡

			#define CmSPeedR_countmax 150//遥控启动速度斜坡
			#define CmSPeedK_countmax 150//键盘启动速度斜坡
			#define CMRotateR_countmax 80//键盘启动速度斜坡
			#define CMRotateK_countmax 80//键盘启动速度斜坡
			#define CMStop_countmax 150//停止启动速度斜坡


			#define MaxWheelPower 75
			//云台初始化角度
			#define GMPitchAngle_Default	0xC9F
			#define GMYawAngle_Default		0xE81

			//拨弹电机和摩擦轮速度
			#define POKSpeed 20;//拨弹电机速度
			#define FRISpeed 800//摩擦轮速度

			//全方向运动最大值
			#define MaxWheelSpeed 5610
			//遥控底盘速度
			#define ChassisYSRRate 8.50f  	//底盘纵向遥控和速度的比率
			#define ChassisXSRRate 7.5f	 	//底盘横向遥控和速度的比率
			#define GMPSRRate  		0.0015f	//
			#define GMYSRRate			0.003f	//
			//键盘底盘速度
			#define ChassisYSKRate 7.5f  	//底盘纵向遥控和速度的比率
			#define ChassisXSKRate 6.5f	 	//底盘横向遥控和速度的比率
			#define GMPSKRate  		0.025f	//
			#define GMYSKRate			0.025f	//

			//------PITCH位置初始值-----//
			#define GMPPositionPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			2000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------PITCH速度初始值-----//
			#define GMPSpeedPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			4900,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------YAW位置初始值-----//
			#define GMYPositionPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			2000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------YAW速度初始值-----//
			#define GMYSpeedPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			4900,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------底盘电机速度初始值-----//
			#define CMSpeedPID_default \
			{\
			3.0f,\
			0.16f,\
			10.0f,\
			0,\
			1000,\
			10000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}

			//------拨弹电机速度初始值-----//
			#define POKSpeedPID_default \
			{\
			25.0f,\
			0.5f,\
			0.0f,\
			0,\
			200,\
			1000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------跟随速度初始值-----//
			#define CMRotatePID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			100,\
			4950,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------功率PID初始值-----//
			#define PowerPID_default \
			{\
			200.0f,\
			5.0f,\
			10.0f,\
			0,\
			5000,\
			10000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
#endif


#ifdef Infantry_2
			//跟随pid
			#define crkp	80 
			#define	crki	0
			#define	crkd	0
			
			#define init_time_ms 8000//初始化斜坡
			#define friction_countmax 50//摩擦轮斜坡

			#define CmSPeedR_countmax 150//遥控启动速度斜坡
			#define CmSPeedK_countmax 150//键盘启动速度斜坡
			#define CMRotateR_countmax 80//键盘启动速度斜坡
			#define CMRotateK_countmax 80//键盘启动速度斜坡
			#define CMStop_countmax 150//停止启动速度斜坡


			#define MaxWheelPower 75
			
			
			//云台初始化角度
			#define GMPitchAngle_Default	0x742
			#define GMYawAngle_Default		0xAC2

			//拨弹电机和摩擦轮速度
			#define POKSpeed 20;//拨弹电机速度
			#define FRISpeed 800//摩擦轮速度

			//全方向运动最大值
			#define MaxWheelSpeed 5000
			//遥控底盘速度
			#define ChassisYSRRate 7.5f  	//底盘纵向遥控和速度的比率
			#define ChassisXSRRate 6.5f	 	//底盘横向遥控和速度的比率
			#define GMPSRRate  		0.0015f	//
			#define GMYSRRate			0.0020f	//
			//键盘底盘速度
			#define ChassisYSKRate 7.5f  	//底盘纵向遥控和速度的比率
			#define ChassisXSKRate 6.5f	 	//底盘横向遥控和速度的比率
			#define GMPSKRate  		0.015f	//
			#define GMYSKRate			0.020f	//

			//------PITCH位置初始值-----//
			#define GMPPositionPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			2000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------PITCH速度初始值-----//
			#define GMPSpeedPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			4900,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------YAW位置初始值-----//
			#define GMYPositionPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			2000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------YAW速度初始值-----//
			#define GMYSpeedPID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			1000,\
			4900,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------底盘电机速度初始值-----//
			#define CMSpeedPID_default \
			{\
			3.0f,\
			0.16f,\
			10.0f,\
			0,\
			1000,\
			10000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}

			//------拨弹电机速度初始值-----//
			#define POKSpeedPID_default \
			{\
			25.0f,\
			0.5f,\
			0.0f,\
			0,\
			200,\
			1000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
			//------跟随速度初始值-----//
			#define CMRotatePID_default \
			{\
			0.0f,\
			0.0f,\
			0.0f,\
			0,\
			100,\
			4950,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
						//------功率PID初始值-----//
			#define PowerPID_default \
			{\
			100.0f,\
			5.0f,\
			10.0f,\
			0,\
			5000,\
			10000,\
			0,\
			0,\
			0,\
			0,\
			{0,0,0},\
			{0,0,0},\
			{0,0,0},\
			&Pid_Test,\
			&Pid_Reset,\
			}
#endif



#endif