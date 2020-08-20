/*
******************************************************************************************************************
*Filename      :uart5_dma.h
*Programmer(s) :chu
*Description   :Design for uart5_dma

							 UART5_RX---PD2
							 UART5_TX---PC12
******************************************************************************************************************
*/
#ifndef __UART5_DMA_H
#define __UART5_DMA_H	 
#include "sys.h"


//使用USART5时开启，不用可以注释
#define U5_DATA_LEN  15
extern u8 rec_data_u5[U5_DATA_LEN];   // 接收数据
extern u8 send_data_u5[U5_DATA_LEN];   // 接收数据
/*
******************************************************************************************************************
*                                                 FUNCTION PROTOTYPES
*                                                   串口5用函数
******************************************************************************************************************
*/
extern void UART5_Init(int Baud);
extern void UART5_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize);//UASRT DMA发送设置
extern void DMA1_S0_Reset(void);//DMA1_Stream1 接收重置	


#endif
