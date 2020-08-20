#ifndef __ULTRA_H
#define	__ULTRA_H

#include "stm32f4xx.h"

#define 		TRIG_F 		{PEout(1)=1;PEout(2)=1;Delay_ULTRA(2);PEout(1)=0;PEout(2)=0;}
#define 		TRIG_B 		{PDout(10)=1;PDout(11)=1;Delay_ULTRA(2);PDout(10)=0;PDout(11)=0;}

typedef struct
{
	u8  CAPTURE_STA;                          //ͨ�����벶���־������λ�������־����6λ�������־		
	u16 CAPTURE_UPVAL;                        //ͨ����������
	u16 CAPTURE_DOWNVAL;                      //ͨ���½�����
	u32	CAPTURE_VAL;	     										//ͨ�����벶��ֵ
} Ultra_cap_t;

void Delay_ULTRA(unsigned char n);
void Ultrasonic_Init(void);
void Ultra_TIM5_Init(u16 arr, u16 psc);
void Ultra_TIM8_Init(u16 arr, u16 psc);

#endif
