/*
******************************************************************************************************************
*Filename      :usart3_dma.h
*Programmer(s) :CJQ
*Description   :Design for usart3_dma
Ŀǰ����PID������
							 USART3_RX---PD9
							 USART3_TX---PD8
******************************************************************************************************************
*/
#ifndef __USART3_DMA_H
#define __USART3_DMA_H	 
#include "sys.h"


//ʹ��USART3ʱ���������ÿ���ע��
#define U3_DATA_LEN  15


//extern u8 connect_flag;//���ӱ�־
extern u8 rec_data_u3[U3_DATA_LEN];   // ��������
extern u8 send_data_u3[U3_DATA_LEN];   // ��������

/*
******************************************************************************************************************
*                                                 FUNCTION PROTOTYPES
*                                                   ����3�ú���
******************************************************************************************************************
*/
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

void Upper3_f_Computer(float data1,float data2,float data3,float data4);

extern void USART3_Init(int Baud);
extern void USART3_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize);//UASRT DMA��������
extern void DMA1_S1_Reset(void);//DMA1_Stream1 ��������	


#endif
