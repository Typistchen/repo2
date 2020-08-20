/*
******************************************************************************************************************
*Filename      :uart4_dma.h
*Programmer(s) :chu
*Description   :Design for uart4_dma
Ŀǰ����������λ��
							 UART4_RX---PC11
							 UART4_TX---PC10
******************************************************************************************************************
*/
#ifndef __UART4_DMA_H
#define __UART4_DMA_H	 
#include "sys.h"


//ʹ��USART4ʱ���������ÿ���ע��
#define U4_DATA_LEN  15
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))      
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
#define L	0
#define R	1
#define F	2
#define B	3
typedef struct
{
	u8 Distence_F1;
	u8 Distence_F2;
	u8 Distence_R1;
	u8 Distence_R2;
	u8 Distence_B1;
	u8 Distence_B2;
	u8 Distence_L1;
	u8 Distence_L2;
	
}UltraStruct;

extern UltraStruct Ultrasonic;

extern u8 rec_data_u4[U4_DATA_LEN];   	// ��������
extern u8 send_data_u4[U4_DATA_LEN];   	// ��������
extern u8 Handle[U4_DATA_LEN]; 
extern u8 connect_flag[4] ;				//��������flag   
extern char err[4] ;
/*
******************************************************************************************************************
*                                                 FUNCTION PROTOTYPES
*                                                   ����4�ú���
******************************************************************************************************************
*/
extern void UART4_Init(int Baud);
extern void USART4_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize);//UASRT DMA��������
extern void DMA1_S2_Reset(void);//DMA1_Stream2 ��������	
extern void Upper_f_Computer(float data1,float data2,float data3,float data4);


#endif
