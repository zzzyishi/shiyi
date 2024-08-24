#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(unsigned char Byte);
unsigned char MyI2C_ReceiveByte(void);
void MyI2C_SendAck(unsigned char AckBit);
unsigned char MyI2C_ReceiveAck(void);

#endif
