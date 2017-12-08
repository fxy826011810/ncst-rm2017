#include "stm32f10x.h"
#include "pid.h"

PID_TypeDef CM1SpeedPID = CMSpeedPID_default;
PID_TypeDef CM2SpeedPID = CMSpeedPID_default;
PID_TypeDef CM3SpeedPID = CMSpeedPID_default;
PID_TypeDef CM4SpeedPID = CMSpeedPID_default;
void abs_limit(float *a, float ABS_MAX)
{
	if (*a > ABS_MAX)
		*a = ABS_MAX;
	if (*a < -ABS_MAX)
		*a = -ABS_MAX;
}

void  Pid_Reset(PID_TypeDef *pid)
{
	pid->Pout = 0;
	pid->Iout = 0;
	pid->Dout = 0;
	pid->output = 0;

}

void Bsp_Pid_Init(void)
	{
		CM4SpeedPID.reset(&CM4SpeedPID);
		CM3SpeedPID.reset(&CM3SpeedPID);
		CM2SpeedPID.reset(&CM2SpeedPID);
		CM1SpeedPID.reset(&CM1SpeedPID);
	}

void Pid_Calc(PID_TypeDef *pid)
{
		pid->error[NOW] = pid->setdata - pid->realdata;
		pid->Pout = pid->Kp*pid->error[NOW];
		pid->Iout += pid->Ki*pid->error[NOW];
		pid->Dout = pid->Kd*(pid->error[NOW] - pid->error[LAST]);
		abs_limit(&(pid->Iout),pid->setimax);
		pid->output = pid->Pout + pid->Iout + pid->Dout;
		abs_limit(&(pid->output),pid->setomax);
		pid->error[LAST] = pid->error[NOW];
}
