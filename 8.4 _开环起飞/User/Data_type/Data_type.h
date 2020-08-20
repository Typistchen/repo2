#ifndef __DATA_TYPE_H
#define	__DATA_TYPE_H

#include "stm32f4xx.h"


typedef struct
{
	u16 time_1ms;                             //角速度读取滤波                   	
	u16 time_2ms;                             //角速度PID运算，内环角速度                     
	u16	time_5ms;                             //
	u16 time_10ms; 														//数传
	u16	time_25ms;	                          //触发超声波距离，车轮旋转状态检测
	u16 time_100ms;                           //系统运行指示灯 	
} Task_loop_t;

#endif
