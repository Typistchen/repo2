/*
******************************************************************************************************************
*Filename      :usart1_dma.h
*Programmer(s) :CJQ
*Description   :Design for usart1_dma

							 USART1_RX---PB7
							 USART1_TX---PB6
******************************************************************************************************************
*/
#ifndef __USART1_DMA_H
#define __USART1_DMA_H	 
#include "sys.h"

//ʹ��USART1ʱ���������ÿ���ע��
#define U1_DATA_LEN  15
extern u8 rec_data_u1[U1_DATA_LEN];   // ��������
extern u8 send_data_u1[U1_DATA_LEN];   // ��������
/*
******************************************************************************************************************
*                                                 FUNCTION PROTOTYPES
*                                                   ����1�ú���
******************************************************************************************************************
*/
extern void USART1_Init(int Baud);
extern void USART1_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize);//UASRT DMA��������
extern void DMA2_S5_Reset(void);//DMA1_Stream6 ��������	
extern void Upper1_f_Computer(float data1,float data2,float data3,float data4);
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
#endif
