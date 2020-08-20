/*
******************************************************************************************************************
*Filename      :usart2_dma.c
*Programmer(s) :CJQ
*Description   :Design for usart2_dma

							 USART2_RX---PD6
							 USART2_TX---PD5
******************************************************************************************************************
*/
#include "usart2_dma.h"
u8 rec_data_u2[U2_DATA_LEN];   // ��������
u8 send_data_u2[U2_DATA_LEN];  // ��������
uint16_t Raw_code[16]; 
int sbus_adu[7];

/*
********************************************************************************************************************
*                  void  USART2_Init(void)
*
*Description : ����2�ĳ�ʼ�� 
							 USART2_RX---PD6
							 USART2_TX---PD5
*Arguments   : Baud������������
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void USART2_Init(int Baud)
{
	NVIC_InitTypeDef NVIC_InitStructure;																			//�����жϽṹ��
 	GPIO_InitTypeDef GPIO_InitStructure;																			//����IO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;																		//���崮�ڽṹ��
	DMA_InitTypeDef DMA_InitStructure;																				//����DMA�ṹ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);										//�򿪴��ڶ�Ӧ������ʱ��

	//**********************���� ���� DMA ���ã�DMA1_Stream6��**************************
	// 0 ����DMAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	// 1 DMA�����ж�����
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// 2 DMAͨ������
	DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);				//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)send_data_u2;						//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;										//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U2_DATA_LEN;														//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;				//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;															//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;												//����DMA�����ȼ���
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    								//ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  				//ָ����FIFO��ֵˮƽ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 							//ָ����Burstת�������ڴ洫��
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 			//ָ����Burstת��������Χת�� 
	// 3 ����DMA1��ͨ��
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);
	// 4 ʹ���ж�
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
	/* 5.ʹ�ܴ��ڵ�DMA���ͽӿ� */
   USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	// 6 �ر�ͨ��
	DMA_Cmd(DMA1_Stream6,DISABLE);//�ȹر�
	//**********************���� ���� DMA ����**************************
	// 0 ����DMAʱ��
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);											//DMAͨ������
	// 2 DMAͨ������
	DMA_DeInit(DMA1_Stream5);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);				//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rec_data_u2;						//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;										//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U2_DATA_LEN;														//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;														//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;										//����DMA�����ȼ���
	// 3 ����DMA1��ͨ��
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	// 4 ʹ��ͨ��
	DMA_Cmd(DMA1_Stream5,ENABLE);
	/* 5.ʹ�ܴ��ڵ�DMA���� */
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
  //��ʼ�����ڲ���
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_BaudRate = Baud;
	//��ʼ������
  USART_Init(USART2,&USART_InitStructure);
	//�����ж�
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;               				//ͨ������Ϊ�����ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       				//�ж�ռ�ȵȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              				//�ж���Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 				//���ж�
  NVIC_Init(&NVIC_InitStructure);
	//����DMA��ʽ����
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	//����DMA��ʽ����
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	//�ж�����
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
  //��������
  USART_Cmd(USART2, ENABLE);
	//����IO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	 												//�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	    									//����:����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 												//������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;										//IO���ٶ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  														//�ܽ�ָ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);																//��ʼ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;													//�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	 										//������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    													//�ܽ�ָ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);      													//��ʼ��
}

/*
********************************************************************************************************************
*                  USART2_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize)
*
*Description : ����2���ͺ��� 
*Arguments   : BufferSRC:�������ݴ�ŵ�ַ��BufferSize:���������ֽ���
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void USART2_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize)				//UASRT DMA��������
{
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
	DMA1_Stream6->NDTR = BufferSize;    																	//����BUFFER��С
	DMA1_Stream6->M0AR = (uint32_t)BufferSRC;   													//���õ�ַ
	DMA1_Stream6->CR |= DMA_SxCR_EN;																			//��DMA,��ʼ����
}
/*
********************************************************************************************************************
*                  void  USART2_IRQHandler(void)
*
*Description : ����2���պ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
uint16_t  re_ch[16];
void USART2_IRQHandler(void)
{
	static uint8_t Uart2_Rec_Len = 0;
	int t;
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  									//�����ж�
	{
		t= USART_ReceiveData(USART2);																					//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
		Uart2_Rec_Len = U2_DATA_LEN - DMA1_Stream5->NDTR;											//����ӱ�֡���ݳ���  
		//***********���ݴ���֡***********//
		sbus_decode(rec_data_u2,re_ch);	
		//******************************//	
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);         							//����жϱ�־
		DMA1_S5_Reset();                                       								//�ָ�DMAָ�룬�ȴ���һ�εĽ���
    }
	else if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		DMA1_Stream6->CR &= (uint32_t)(~DMA_SxCR_EN);   											//�ر�DMA,�������
	}
}
/*
********************************************************************************************************************
*                  void sbus_decode(uint8_t *raw_code,uint16_t *de_code)
*
*Description : sbus���뺯��
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void sbus_decode(uint8_t *raw_code,uint16_t *de_code)
{
	de_code[0]  = ((raw_code[1]|raw_code[2]<<8)           		   		 & 0x07FF);
	de_code[1]  = ((raw_code[2]>>3 |raw_code[3]<<5)                  & 0x07FF);
	de_code[2]  = ((raw_code[3]>>6 |raw_code[4]<<2 |raw_code[5]<<10) & 0x07FF);
	de_code[3]  = ((raw_code[5]>>1 |raw_code[6]<<7)                  & 0x07FF);
	de_code[4]  = ((raw_code[6]>>4 |raw_code[7]<<4)                  & 0x07FF);
	de_code[5]  = ((raw_code[7]>>7 |raw_code[8]<<1 |raw_code[9]<<9)  & 0x07FF);
	de_code[6]  = ((raw_code[9]>>2 |raw_code[10]<<6)                 & 0x07FF);
	de_code[7]  = ((raw_code[10]>>5|raw_code[11]<<3)                 & 0x07FF);
	de_code[8]  = ((raw_code[12]   |raw_code[13]<<8)                 & 0x07FF);
	de_code[9]  = ((raw_code[13]>>3|raw_code[14]<<5)                 & 0x07FF);
	de_code[10] = ((raw_code[14]>>6|raw_code[15]<<2|raw_code[16]<<10)& 0x07FF);
	de_code[11] = ((raw_code[16]>>1|raw_code[17]<<7)                 & 0x07FF);
	de_code[12] = ((raw_code[17]>>4|raw_code[18]<<4)                 & 0x07FF);
	de_code[13] = ((raw_code[18]>>7|raw_code[19]<<1|raw_code[20]<<9) & 0x07FF);
	de_code[14] = ((raw_code[20]>>2|raw_code[21]<<6)                 & 0x07FF);
	de_code[15] = ((raw_code[21]>>5|raw_code[22]<<3)                 & 0x07FF);
}
/*
********************************************************************************************************************
*                  void DMA1_Stream6_IRQHandler(void)
*Description : ����2�����жϺ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void DMA1_Stream6_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) != RESET)
    {
        DMA_ClearFlag(DMA1_Stream6,DMA_IT_TCIF6);												//�����־λ 
        DMA_Cmd(DMA1_Stream6,DISABLE);																	//�ر�DMA 																									
        USART_ITConfig(USART2,USART_IT_TC,ENABLE);											//�򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ�
    }
}

/*
********************************************************************************************************************
*                  void DMA1_S5_Reset(void)
*
*Description : ����2�������� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void DMA1_S5_Reset(void)//DMA1_Stream6 ��������
{ 
	DMA_ClearFlag(DMA1_Stream5,DMA_IT_TCIF5|DMA_FLAG_HTIF5|DMA_FLAG_TEIF5|DMA_FLAG_DMEIF5|DMA_FLAG_FEIF5);  
	DMA_Cmd(DMA1_Stream5,DISABLE); 																				//�ر�USART2 TX DMA1 ��ָʾ��ͨ��
	DMA1_Stream5->NDTR = U2_DATA_LEN;                   
	DMA_Cmd(DMA1_Stream5,ENABLE);  																				//ʹ��USART2 TX DMA1 ��ָʾ��ͨ��
}
/*
********************************************************************************************************************
*                  sbus_re_calculate(int *Data)
*
*Description : ����  ң�ؽ������ݻ���
*Arguments   : ң����
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
int sbus_re_calculate(int *Data)
{
	Data[1] = (float)re_ch[0] * -0.156 + 139.93;												//ǰ��
	Data[2] = (float)re_ch[1] * -0.144 + 143.05;												//����
	Data[3] = (float)re_ch[2] * -0.144 + 143.79;												//����
	Data[4] = (float)re_ch[3] * 0.144  - 143.79;											  //ƫ��
	Data[5] = (float)re_ch[4]; 																					//��־λ 306 =0 1694 = 1
	return 1;
}

