#include "stm32f4xx.h"
#include "pid.h"

PID_TypeDef CM1SpeedPID = CM1SpeedPID_default;
PID_TypeDef CM2SpeedPID = CM2SpeedPID_default;
PID_TypeDef CM3SpeedPID = CM3SpeedPID_default;
PID_TypeDef CM4SpeedPID = CM4SpeedPID_default;



void abs_limit(float *a, float ABS_MAX)
{
	if (*a > ABS_MAX)
		*a = ABS_MAX;
	if (*a < -ABS_MAX)
		*a = -ABS_MAX;
}

void  Pid_Reset(PID_TypeDef* pid)
{
	pid->Pout = 0;
	pid->Iout = 0;
	pid->Dout = 0;
	pid->output[NOW] = 0;
	pid->output[LAST] = 0;
	pid->output[LLAST] = 0;
}

void Bsp_Pid_Init(void)
	{

	CM1SpeedPID.reset(&CM1SpeedPID);
	CM2SpeedPID.reset(&CM2SpeedPID);
	CM3SpeedPID.reset(&CM3SpeedPID);
	CM4SpeedPID.reset(&CM4SpeedPID);
	}

void Pid_Test(PID_TypeDef* pid)
{
	
		pid->error[NOW] = pid->setdata - pid->realdata[NOW];
		pid->Pout = pid->Kp*pid->error[NOW];
		pid->Iout += pid->Ki*pid->error[NOW];
		pid->Dout = pid->Kd*(pid->error[NOW] - pid->error[LAST]);

		abs_limit(&(pid->Iout), pid->setimax);

		pid->output[NOW] = pid->Pout + pid->Iout + pid->Dout;
		abs_limit(&(pid->output[NOW]), pid->setmax);
                
    pid->output[LLAST] = pid->output[LAST];
		pid->output[LAST] = pid->output[NOW];
		pid->error[LLAST] = pid->error[LAST];
		pid->error[LAST] = pid->error[NOW];
		pid->realdata[LLAST] = pid->realdata[LAST];
		pid->realdata[LAST] = pid->realdata[NOW];
}


//sPID->Kp = 5.0f;
//sPID->Ki = 0.01f;
//sPID->Kd = 0.0f;//λ��ʽPID ����̨6623







//	if (POSITION_PID)
//	{
//
//
//	Cloud_Pid[a].Kp = 5.0f;
//	Cloud_Pid[a].Ki = 0.0f;
//	Cloud_Pid[a].Kd = 0.0f;


//		RM3510_Pid[a].Kp = 5.0f;
//		RM3510_Pid[a].Ki = 0.16f;
//		RM3510_Pid[a].Kd = 20.0f;//3510
//	}
//	else if (DELTA_PID)
//	{
//
//		RM3510_Pid[a].Kp = 2.5f;
//		RM3510_Pid[a].Ki = 0.08f;
//		RM3510_Pid[a].Kd = 0.0f;
//	}
