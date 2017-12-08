#include "stm32f10x.h"
#include "pid.h"


PID_TypeDef GMPPositionPID = GMPPositionPID_default;
PID_TypeDef GMPSpeedPID = GMPSpeedPID_default;
PID_TypeDef GMYPositionPID = GMYPositionPID_default;
PID_TypeDef GMYSpeedPID = GMYSpeedPID_default;

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
	GMPPositionPID.reset(&GMPPositionPID);
	GMYPositionPID.reset(&GMYPositionPID);
	GMPSpeedPID.reset(&GMPSpeedPID);
	GMYSpeedPID.reset(&GMYSpeedPID);

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
