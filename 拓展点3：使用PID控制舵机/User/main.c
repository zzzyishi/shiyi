#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "PID.h"
#include "Timer.h"

float Pitch, Roll, Yaw;

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
		PID_Control1(90 - Pitch);
		PID_Control2(90 + Yaw);
	}	
}

int main(void)
{
	MPU6050_Init();
	MPU6050_DMP_Init();
	Servo_Init();
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	
	Timer_Init();
	
	while (1)
	{
		
	}
}
