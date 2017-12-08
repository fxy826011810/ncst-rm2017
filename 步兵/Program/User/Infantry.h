#ifndef __INFANTRY_H
#define __INFANTRY_H

#define Infantry_2
//#define Infantry_3


#ifdef Infantry_3
			//����pid
			#define crkp	80 
			#define	crki	0
			#define	crkd	0
			
			#define init_time_ms 8000//��ʼ��б��
			#define friction_countmax 50//Ħ����б��

			#define CmSPeedR_countmax 150//ң�������ٶ�б��
			#define CmSPeedK_countmax 150//���������ٶ�б��
			#define CMRotateR_countmax 80//���������ٶ�б��
			#define CMRotateK_countmax 80//���������ٶ�б��
			#define CMStop_countmax 150//ֹͣ�����ٶ�б��


			#define MaxWheelPower 75
			//��̨��ʼ���Ƕ�
			#define GMPitchAngle_Default	0xC9F
			#define GMYawAngle_Default		0xE81

			//���������Ħ�����ٶ�
			#define POKSpeed 20;//��������ٶ�
			#define FRISpeed 800//Ħ�����ٶ�

			//ȫ�����˶����ֵ
			#define MaxWheelSpeed 5610
			//ң�ص����ٶ�
			#define ChassisYSRRate 8.50f  	//��������ң�غ��ٶȵı���
			#define ChassisXSRRate 7.5f	 	//���̺���ң�غ��ٶȵı���
			#define GMPSRRate  		0.0015f	//
			#define GMYSRRate			0.003f	//
			//���̵����ٶ�
			#define ChassisYSKRate 7.5f  	//��������ң�غ��ٶȵı���
			#define ChassisXSKRate 6.5f	 	//���̺���ң�غ��ٶȵı���
			#define GMPSKRate  		0.025f	//
			#define GMYSKRate			0.025f	//

			//------PITCHλ�ó�ʼֵ-----//
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
			//------PITCH�ٶȳ�ʼֵ-----//
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
			//------YAWλ�ó�ʼֵ-----//
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
			//------YAW�ٶȳ�ʼֵ-----//
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
			//------���̵���ٶȳ�ʼֵ-----//
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

			//------��������ٶȳ�ʼֵ-----//
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
			//------�����ٶȳ�ʼֵ-----//
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
			//------����PID��ʼֵ-----//
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
			//����pid
			#define crkp	80 
			#define	crki	0
			#define	crkd	0
			
			#define init_time_ms 8000//��ʼ��б��
			#define friction_countmax 50//Ħ����б��

			#define CmSPeedR_countmax 150//ң�������ٶ�б��
			#define CmSPeedK_countmax 150//���������ٶ�б��
			#define CMRotateR_countmax 80//���������ٶ�б��
			#define CMRotateK_countmax 80//���������ٶ�б��
			#define CMStop_countmax 150//ֹͣ�����ٶ�б��


			#define MaxWheelPower 75
			
			
			//��̨��ʼ���Ƕ�
			#define GMPitchAngle_Default	0x742
			#define GMYawAngle_Default		0xAC2

			//���������Ħ�����ٶ�
			#define POKSpeed 20;//��������ٶ�
			#define FRISpeed 800//Ħ�����ٶ�

			//ȫ�����˶����ֵ
			#define MaxWheelSpeed 5000
			//ң�ص����ٶ�
			#define ChassisYSRRate 7.5f  	//��������ң�غ��ٶȵı���
			#define ChassisXSRRate 6.5f	 	//���̺���ң�غ��ٶȵı���
			#define GMPSRRate  		0.0015f	//
			#define GMYSRRate			0.0020f	//
			//���̵����ٶ�
			#define ChassisYSKRate 7.5f  	//��������ң�غ��ٶȵı���
			#define ChassisXSKRate 6.5f	 	//���̺���ң�غ��ٶȵı���
			#define GMPSKRate  		0.015f	//
			#define GMYSKRate			0.020f	//

			//------PITCHλ�ó�ʼֵ-----//
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
			//------PITCH�ٶȳ�ʼֵ-----//
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
			//------YAWλ�ó�ʼֵ-----//
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
			//------YAW�ٶȳ�ʼֵ-----//
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
			//------���̵���ٶȳ�ʼֵ-----//
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

			//------��������ٶȳ�ʼֵ-----//
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
			//------�����ٶȳ�ʼֵ-----//
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
						//------����PID��ʼֵ-----//
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