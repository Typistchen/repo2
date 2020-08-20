#ifndef _IMU_H_
#define	_IMU_H_
#include "stm32f4xx.h"
#include "MYMATH.h"
#include "mpu6050.h"
extern	S16_XYZ Acc_Raw;
extern	S16_XYZ Gyro_Raw;
extern	S16_XYZ Acc_Filter;
extern	S16_XYZ Gyro_Filter;
extern	SI_F_XYZ  SI_gyro;
//S16_XYZ  Gyro_Adu;
//S16_XYZ  Acc_Adu;
extern	SI_F_ANGEL  Angel;
extern float Sin_Pitch,Sin_Roll,Sin_Yaw;
extern float Cos_Pitch,Cos_Roll,Cos_Yaw;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void Get_Eulerian_Angle( SI_F_ANGEL *angle);
void Get_Angle(void);
void Get_Angle_Sin(void);
#endif

