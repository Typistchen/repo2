#ifndef __DATA_TYPE_H
#define	__DATA_TYPE_H

#include "stm32f4xx.h"


typedef struct
{
	u16 time_1ms;                             //���ٶȶ�ȡ�˲�                   	
	u16 time_2ms;                             //���ٶ�PID���㣬�ڻ����ٶ�                     
	u16	time_5ms;                             //
	u16 time_10ms; 														//����
	u16	time_25ms;	                          //�������������룬������ת״̬���
	u16 time_100ms;                           //ϵͳ����ָʾ�� 	
} Task_loop_t;

#endif
