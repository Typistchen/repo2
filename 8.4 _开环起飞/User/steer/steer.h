#ifndef __STEER_H
#define	__STEER_H

#include "stm32f4xx.h"

#define Steer0   0
#define Steer1   1
#define Steer2   2
#define Steer3   3

 void TIM3_Steer1_2_PWM_Init(u32 arr,u16 psc);
 void Steer_Set_angle(u8 Steerx,int *angle);
void Steer_delay_ms(int n);
void Steer_Mode(void);
#endif
