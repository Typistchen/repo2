#ifndef _MYMATH_H
#define _MYMATH_H
#include "stm32f4xx.h"
#include "math.h"

typedef struct
{
	s16 X;
	s16 Y;
	s16 Z;
}S16_XYZ;

typedef struct
{
	float X;
	float Y;
	float Z;
}SI_F_XYZ;

typedef struct
{
	float roll;
	float pitch;
	float yaw;
}SI_F_ANGEL;

#define Pi	3.1415927f
#define Radian_to_Angle	   57.2957795f
#define Angle_to_Radian	   Pi/180.0f
#define RawData_to_Angle	0.0305175781f	//以下参数对应1000度每秒
#define RawData_to_Radian	0.0010653f

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )

void Get_Radian(S16_XYZ *Gyro_in,SI_F_XYZ *Gyro_out);
float invSqrt(float x);
void Calculate_FilteringCoefficient(float Time, float Frequency);
void ACC_IIR_Filter(S16_XYZ *Acc_in,S16_XYZ *Acc_out);
void Gyro_Slid_Filter(S16_XYZ *Gyro_in,S16_XYZ *Gyro_out);
void Gyro_Offset(S16_XYZ *Gyro_in,S16_XYZ *Gyro_out);
void Acc_Offset(S16_XYZ *Acc_in,S16_XYZ *Acc_out);

s16 enhanceMeanFilter(s16 newDatas);

#endif

