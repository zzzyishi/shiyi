#include "stm32f10x.h"                  // Device header
#include "Servo.h"

#define MIN_ANGLE 0
#define MAX_ANGLE 180

float KP = 1.0;         //比例系数
float KI = 0.01;        //积分系数
float KD = 0.1;         //微分系数
float Position = 0;     //当前位置
float integral = 0;     //积分值
float pre_error = 0;    //上一次的误差值

float Limit(float value, float min, float max)  
{  
    if (value < min)  
    {  
        return min;  
    }  
    else if (value > max)  
    {  
        return max;  
    }  
    else  
    {  
        return value;  
    }  
}

void PID_Control1(float Target)
{
	float error = Target - Position;
	integral += error;
	
	float output = KP*error + KI*integral + KD*(error - pre_error);  //PID公式计算
	
	pre_error = error;
	
	Position += output;
	Position = Limit(Position, MIN_ANGLE, MAX_ANGLE);          //限幅
	Servo_SetAngle1(Position);	
}

void PID_Control2(float Target)
{
	float error = Target - Position;
	integral += error;
	
	float output = KP*error + KI*integral + KD*(error - pre_error);
	
	pre_error = error;
	
	Position += output;
	Position = Limit(Position, MIN_ANGLE, MAX_ANGLE);
	Servo_SetAngle2(Position);	
}
