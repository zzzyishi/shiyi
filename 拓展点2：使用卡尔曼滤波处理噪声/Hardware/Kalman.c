#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MPU6050.h"
#include <math.h>

float ax_offset = 0, ay_offset = 0;//初始偏移量
float gx_offset = 0, gy_offset = 0;

float roll_v = 0, pitch_v = 0;//角速度
float dt = 0.001;

float rad2deg = 57.29578;

float gyro_roll = 0, gyro_pitch = 0;//先验值
float acc_roll = 0, acc_pitch = 0;//观测值
float k_roll = 0, k_pitch = 0;//最优估计值

float P[2][2] = {{1,0},{0,1}};//预测协方差矩阵P

float K[2][2] = {{0,0},{0,0}};//卡尔曼增益

//计算偏移量
void Kalman_Init(void) 
{	
	for(float i = 1; i <= 100; i++)
	{
		int16_t AX, AY, AZ, GX, GY, GZ;
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		ax_offset = ax_offset + AX;
		ay_offset = ay_offset + AY;
		gx_offset = gx_offset + GX;
		gy_offset = gy_offset + GY;
	}
	ax_offset = ax_offset / 100;
	ay_offset = ay_offset / 100;
	gx_offset = gx_offset / 100;
	gy_offset = gy_offset / 100;
	Delay_us(100);
}

void Kalman_Update(int16_t *roll, int16_t *pitch) 
{	
    int16_t AX, AY, AZ, GX, GY, GZ;
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

    //计算先验状态
    roll_v = (GX - gx_offset) + ((sin(k_pitch / rad2deg)*sin(k_roll / rad2deg))/cos(k_pitch / rad2deg))*(GY - gy_offset) + ((sin(k_pitch / rad2deg)*cos(k_roll / rad2deg))/cos(k_pitch / rad2deg))*GZ;
    pitch_v = cos(k_roll / rad2deg)*(GY - gy_offset) - sin(k_roll / rad2deg)*GZ;
    gyro_roll = k_roll + roll_v*dt;
    gyro_pitch = k_pitch + pitch_v*dt;

    //计算先验误差协方差矩阵P
    P[0][0] = P[0][0] + 0.003;//过程噪声协方差矩阵Q取值0.003I
    P[0][1] = P[0][1] + 0;
    P[1][0] = P[1][0] + 0;
    P[1][1] = P[1][1] + 0.003;

    //更新卡尔曼增益K
    K[0][0] = P[0][0]/(P[0][0]+0.3);//观测噪声协方差矩阵R取值0.3I
    K[0][1] = 0;
    K[1][0] = 0;
    K[1][1] = P[1][1]/(P[1][1]+0.3);
    
    //计算最优估计状态
    acc_roll = atan((AY - ay_offset) / (AZ))*rad2deg;
    acc_pitch = -1*atan((AX - ax_offset) / sqrt((AY - ay_offset)*(AY - ay_offset) + (AZ)*(AZ)))*rad2deg;
    k_roll = gyro_roll + K[0][0]*(acc_roll - gyro_roll);
    k_pitch = gyro_pitch + K[1][1]*(acc_pitch - gyro_pitch);
	*roll = k_roll;
	*pitch = k_pitch;
    
    //更新协方差矩阵P
    P[0][0] = (1 - K[0][0])*P[0][0];
    P[0][1] = 0;
    P[1][0] = 0;
    P[1][1] = (1 - K[1][1])*P[1][1];
}
