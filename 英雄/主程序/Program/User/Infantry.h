#ifndef __INFANTRY_H
#define __INFANTRY_H

			#define init_time_ms 2000//��ʼ��б�£�ms*2��
			
			#define CmSPeedR_countmax 100//ң�������ٶ�б��
			#define CmSPeedK_countmax 500//���������ٶ�б��
			#define CMRotateR_countmax 100//���������ٶ�б��
			#define CMRotateK_countmax 100//���������ٶ�б��
			#define CMStop_countmax 100//ֹͣ�����ٶ�б��


			//��̨��ʼ���Ƕ�
			#define GMPitchAngle_Default	0xC18

			//���������Ħ�����ٶ�
			#define friction_countmax 50//Ħ����б��	��ms*3��
			#define FRISpeed 300//Ħ�����ٶ�
			#define FRIBSpeed 280//Ħ�����ٶ�
			#define pok_stopcountmax 200//���������ʼʱ�䣨ms*3��
			#define pok_startcountmax 200//�����������ʱ�䣨ms*3��
			
			#define sg_stopcountmax 120//�񵯶����ʼʱ�䣨ms*3��
			#define sg_startcountmax 120//�񵯶������ʱ�䣨ms*3��
			//ȫ�����˶����ֵ
			#define MaxWheelSpeed 200
			#define MaxWheelPower 75//�������ֵ
			//ң�ص����ٶ�
			#define ChassisYRate 0.2f  	//��������ң�غ��ٶȵı���
			#define ChassisXRate 0.03f	 	//���̺���ң�غ��ٶȵı���
			
			#define ChassisYHRate 0.3f  	//��������ң�غ��ٶȵı���
			#define ChassisXHRate 0.10f	 	//���̺���ң�غ��ٶȵı���
			//ң��ת��
			#define ChassisRRate 0.035f	 	//���̺���ң�غ��ٶȵı���
			#define ChassisRHRate 0.07f	 	//���̺���ң�غ��ٶȵı���
			#define GMPRate  		0.00025f	//ң��pitch
			//���ת��
			#define ChassisKRRate 0.7f	 	//���̺���ң�غ��ٶȵı���
			#define ChassisKRHRate 1.0f	 	//���̺���ң�غ��ٶȵı���
			#define GMPKRate  		0.0025f	//���pitch

			//------PITCHλ�ó�ʼֵ-----//
			#define GMPPositionPID_default { 0.0f,	 0.0f, 0.0f, 	0, 	1000, 	800 , 	0, 0, 0, 0,	{0,0,0}, {0,0,0},	{0,0,0}, &Pid_Test, &Pid_Reset,}//pitchλ��
			//------PITCH�ٶȳ�ʼֵ-----//
			#define GMPSpeedPID_default    { 0.0f,  0.0f, 0.0f, 	0, 	1000, 	4900, 	0, 0, 0, 0, {0,0,0}, {0,0,0}, {0,0,0}, &Pid_Test, &Pid_Reset,}//pitch�ٶ�
			//------���̵���ٶȳ�ʼֵ-----//
			#define CMSpeedPID_default     { 220.0f, 0.0f, 0.4f, 	0, 	4900, 	8000,		0, 0, 0, 0, {0,0,0}, {0,0,0}, {0,0,0}, &Pid_Test, &Pid_Reset,}//1���ٶ�

#endif