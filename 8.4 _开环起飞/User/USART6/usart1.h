
#ifndef USART1_H__
#define USART1_H__

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"

#include "math.h"
#ifdef __cplusplus
 extern "C" {
#endif

extern void USART6_Init(u32 bound);
extern void USART6_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
	 
extern uint8_t u6_receive_buff[];
	 
extern void sbus_decode(uint8_t *raw_code,uint16_t *de_code); 
extern uint16_t  re_ch[16];
extern int sbus_re_calculate(int *Data1,int *Data2,int *Data3,int *Data4);//译码  数据移位合并
extern int sbus_adu[7]; 
//extern s16 light_Raw[7];	 


	 
#ifdef __cplusplus
}
#endif


#endif








