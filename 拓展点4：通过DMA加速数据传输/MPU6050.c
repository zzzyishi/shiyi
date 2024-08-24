#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS		0xD0

int8_t MPU6050_Data[14] = {0};
int flag = 0;

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel5, DISABLE);                
	DMA_SetCurrDataCounter(DMA1_Channel5, 14);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC5);
}

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if(Timeout == 0)
		{
			break;
		}
	}
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);

	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	I2C_GenerateSTOP(I2C2, ENABLE);
}

void MPU6050_ReadReg(uint8_t RegAddress)
{
//	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	if(flag != 0)MyDMA_Transfer();

	for(int8_t count=0; count<14; count++){	 
		 if(count != 14 - 1)
		 {			 
			 MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//EV_7 				
		 }
		 else{
					I2C_AcknowledgeConfig(I2C2, DISABLE);
					I2C_GenerateSTOP(I2C2, ENABLE);					
					MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//EV_7		
			}							
	}
	flag ++;

//	I2C_AcknowledgeConfig(I2C2, DISABLE);
//	I2C_GenerateSTOP(I2C2, ENABLE);
//	
//	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
//	Data = I2C_ReceiveData(I2C2);
//	
//	I2C_AcknowledgeConfig(I2C2, ENABLE);

//	return Data;
}

void MPU6050_Init(void)
{	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(I2C2->DR);
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)MPU6050_Data;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;   
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 14;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
													
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel5, ENABLE);
	I2C_DMACmd(I2C2, ENABLE);	
	I2C_Cmd(I2C2, ENABLE);

//  DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
//		
//	NVIC_InitTypeDef NVIC_InitStructure;

//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

	I2C_GenerateSTART(I2C2, ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

//void DMA1_Channel5_IRQHandler(void){
//    if(DMA_GetITStatus(DMA1_IT_TC5)){
//					
//		}
//		DMA_ClearITPendingBit(DMA1_IT_TC5);
//}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataH = MPU6050_Data[1];
	DataL = MPU6050_Data[0];
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_Data[3];
	DataL = MPU6050_Data[2];
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_Data[5];
	DataL = MPU6050_Data[4];
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_Data[9];
	DataL = MPU6050_Data[8];
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_Data[11];
	DataL = MPU6050_Data[10];
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_Data[13];
	DataL = MPU6050_Data[12];
	*GyroZ = (DataH << 8) | DataL;
	
//	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
//	DataH = MPU6050_Data[0];
//	MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
//	DataL = MPU6050_Data[0];
//	*AccX = (DataH << 8) | DataL;
//	
//	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
//	DataH = MPU6050_Data[0];
//	MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
//	DataL = MPU6050_Data[0];
//	*AccY = (DataH << 8) | DataL;
//	
//	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
//	DataH = MPU6050_Data[0];
//	MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
//	DataL = MPU6050_Data[0];
//	*AccZ = (DataH << 8) | DataL;
//	
//	MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
//	DataH = MPU6050_Data[0];
//	MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
//	DataL = MPU6050_Data[0];
//	*GyroX = (DataH << 8) | DataL;
//	
//	MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
//	DataH = MPU6050_Data[0];
//	MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
//	DataL = MPU6050_Data[0];
//	*GyroY = (DataH << 8) | DataL;
//	
//	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
//	DataH = MPU6050_Data[0];
//	MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
//	DataL = MPU6050_Data[0];
//	*GyroZ = (DataH << 8) | DataL;
}
