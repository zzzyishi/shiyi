#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "MPU6050.h"
#include "Kalman.h"

int16_t Roll, Pitch;

int main(void)
{
	MPU6050_Init();
	Servo_Init();
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);
	
	Kalman_Init();
	
	while (1)
	{
		Kalman_Update(&Roll, &Pitch);
		Servo_SetAngle1(90 - Pitch);
		Servo_SetAngle2(90 + Roll);
	}	
}
