#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"

float Pitch, Roll, Yaw;

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	
	while (1)
	{
		MPU6050_DMP_Get_Data(&Pitch, &Roll, &Yaw);
		
		OLED_ShowString(1, 1, "Pitch:");
		OLED_ShowString(2, 1, "Roll:");
		OLED_ShowString(3, 1, "Yaw:");
		
		OLED_ShowSignedNum(1, 7, Pitch, 5);
		OLED_ShowSignedNum(2, 7, Roll, 5);
		OLED_ShowSignedNum(3, 7, Yaw, 5);
	}
}
