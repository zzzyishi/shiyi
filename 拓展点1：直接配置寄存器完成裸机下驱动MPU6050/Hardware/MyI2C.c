//#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define RCC_APB2ENR (*(unsigned int *)0x40021018)
#define GPIOB_BSRR  (*(unsigned int *)0x40010C10)
#define GPIOB_BRR   (*(unsigned int *)0x40010C14)
#define GPIOB_IDR   (*(unsigned int *)0x40010C08)
#define GPIOB_CRH   (*(unsigned int *)0x40010C04)

void MyI2C_W_SCL(unsigned char BitValue)
{
	if(BitValue){  
        GPIOB_BSRR |= 1<<10;
    }
	else{  
        GPIOB_BRR |= 1<<10;  
    } 	
	Delay_us(10);
}

void MyI2C_W_SDA(unsigned char BitValue)
{
    if(BitValue) {  
        GPIOB_BSRR |= 1<<11;
    } 
	else{  
        GPIOB_BRR |= 1<<11;  
    }	
	Delay_us(10);
}

unsigned char MyI2C_R_SDA(void)  
{   
    return (GPIOB_IDR >> 11) & 0x01;  
}

void MyI2C_Init(void)
{
	RCC_APB2ENR = 0x0008;
	
    GPIOB_CRH &= 0xFFFFF0FF; //GPIO_Pin_10初始化
    GPIOB_CRH |= 0x00000700; //0111 开漏输出+50MHz
    GPIOB_CRH &= 0xFFFF0FFF; //GPIO_Pin_11初始化
    GPIOB_CRH |= 0x00007000;
      
    GPIOB_BSRR |= 1<<10 | 1<<11; 
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

unsigned char MyI2C_ReceiveByte(void)
{
	unsigned char i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(unsigned char AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

unsigned char MyI2C_ReceiveAck(void)
{
	unsigned char AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
