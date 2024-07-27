#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h"

int main(void)
{
	PWM_Init();	
	
	while (1)
	{
		PWM_Swing(90, 90);
	}
}
