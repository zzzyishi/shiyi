#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_WriteReg(unsigned char RegAddress, unsigned char Data);
unsigned char MPU6050_ReadReg(unsigned char RegAddress);

void MPU6050_Init(void);
unsigned char MPU6050_GetID(void);
void MPU6050_GetData(signed short int *AccX, signed short int *AccY, signed short int *AccZ, 
						signed short int *GyroX, signed short int *GyroY, signed short int *GyroZ);

#endif
