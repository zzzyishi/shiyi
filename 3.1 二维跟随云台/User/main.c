#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "MPU6050.h"
#include "inv_mpu.h"

float Pitch, Roll, Yaw;

int main(void)
{
	MPU6050_Init();
	MPU6050_DMP_Init();
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	
	Servo_Init();
	
	while (1)
	{
		MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
		Servo_SetAngle1(90 - Pitch);
		Servo_SetAngle2(90 + Yaw);
	}
}
