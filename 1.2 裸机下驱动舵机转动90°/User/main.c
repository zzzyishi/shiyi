#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h"

int main(void)
{
	PWM_Init();	
	
	PWM_SetCompare2(1500);
	
	while (1)
	{
		
	}
}
