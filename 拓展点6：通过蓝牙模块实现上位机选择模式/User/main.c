#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Serial.h"

int16_t Pitch, Yaw = 0;
uint8_t RxData;

int main(void)
{
	OLED_Init();
	Serial_Init();
	Servo_Init();
	Servo_SetAngle1(90);
	Servo_SetAngle2(90);	
	
	while (1)
	{
		OLED_ShowString(1, 1, "Yaw:");
		OLED_ShowString(2, 1, "Pitch:");
		OLED_ShowSignedNum(1, 7, Yaw, 2);
		OLED_ShowSignedNum(2, 7, -Pitch, 2);
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		RxData = USART_ReceiveData(USART1);
		if((RxData == 'L') && (Yaw > -90))           //向左转动10°
		{
			Yaw = Yaw - 10;
			Servo_SetAngle2(90 - Yaw);
		}
		else if((RxData == 'R') && (Yaw < 90))       //向右转动10°
		{
			Yaw = Yaw + 10;
			Servo_SetAngle2(90 - Yaw);
		}
		else if((RxData == 'U') && (Pitch > -90))    //向上转动10°
		{
			Pitch = Pitch - 10;
			Servo_SetAngle1(90 + Pitch);
		}
		else if((RxData == 'D') && (Pitch < 90))     //向下转动10°
		{
			Pitch = Pitch + 10;
			Servo_SetAngle1(90 + Pitch);
		}
		else                                        //错误提示
		{
			printf("ERROR");
		}
	}
}
