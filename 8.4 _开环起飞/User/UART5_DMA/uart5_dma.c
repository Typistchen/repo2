/*
******************************************************************************************************************
*Filename      :uart5_dma.c
*Programmer(s) :chu
*Description   :Design for uart5_dma

							 UART5_RX---PD2
							 UART5_TX---PC12
******************************************************************************************************************
*/
#include "UART5_dma.h"
u8 rec_data_u5[U5_DATA_LEN];   // ��������

u8 send_data_u5[U5_DATA_LEN];
/*
********************************************************************************************************************
*                  void  UART5_Init(void)
*
*Description : ����5�ĳ�ʼ�� 
							 UART5_RX---PD2
							 UART5_TX---PC12
*Arguments   : Baud������������
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/


void UART5_Init(int Baud)
{
	NVIC_InitTypeDef NVIC_InitStructure ;//�����жϽṹ��
 	GPIO_InitTypeDef GPIO_InitStructure;//����IO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;//���崮�ڽṹ��
	DMA_InitTypeDef DMA_InitStructure;//����DMA�ṹ��

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//�򿪴��ڶ�Ӧ������ʱ��

	//**********************���� ���� DMA ���ã�DMA1_Stream6��**************************
	// 0 ����DMAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	// 1 DMA�����ж�����
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 2 DMAͨ������
	DMA_DeInit(DMA1_Stream7);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)send_data_u5;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U5_DATA_LEN;//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����DMA���ڴ����ģʽ

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//����DMA�����ȼ���
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  	//ָ����FIFO��ֵˮƽ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //ָ����Burstת�������ڴ洫��
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //ָ����Burstת��������Χת�� */

	// 3 ����DMA1��ͨ��
	DMA_Init(DMA1_Stream7, &DMA_InitStructure);
	// 4 ʹ���ж�
	DMA_ITConfig(DMA1_Stream7,DMA_IT_TC,ENABLE);


	/* 5.ʹ�ܴ��ڵ�DMA���ͽӿ� */
    USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);
	// 6 �ر�ͨ��
	DMA_Cmd(DMA1_Stream7,DISABLE);//�ȹر�

	//**********************���� ���� DMA ����**************************
	// 0 ����DMAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//DMAͨ������
	
	// 2 DMAͨ������
	DMA_DeInit(DMA1_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rec_data_u5;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U5_DATA_LEN;//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//����DMA�����ȼ���

	// 3 ����DMA1��ͨ��
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);
	// 4 ʹ��ͨ��
	DMA_Cmd(DMA1_Stream0,ENABLE);

	/* 5.ʹ�ܴ��ڵ�DMA���� */
	USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);


    //��ʼ�����ڲ���
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_BaudRate = Baud;
	//��ʼ������
    USART_Init(UART5,&USART_InitStructure);


	//�����ж�
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;               //ͨ������Ϊ�����ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       //�ж�ռ�ȵȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              //�ж���Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�
    NVIC_Init(&NVIC_InitStructure);

	//����DMA��ʽ����
	USART_DMACmd(UART5,USART_DMAReq_Tx,ENABLE);
	//����DMA��ʽ����
	USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);

	//�ж�����
	USART_ITConfig(UART5,USART_IT_TC,DISABLE);
	USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);
	USART_ITConfig(UART5,USART_IT_TXE,DISABLE);
	USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);


    //��������
    USART_Cmd(UART5, ENABLE);

    //����IO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	 //�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	    //����:����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 //������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//IO���ٶ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //�ܽ�ָ��
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	 //������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;    //�ܽ�ָ��
	GPIO_Init(GPIOD, &GPIO_InitStructure);      //��ʼ��
}
/*
********************************************************************************************************************
*                  void UART5_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize)
*
*Description : ����5���ͺ��� 
*Arguments   : BufferSRC:�������ݴ�ŵ�ַ
			   BufferSize:���������ֽ���
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void UART5_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize)//UASRT DMA��������
{
	DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
	DMA1_Stream7->NDTR = BufferSize;    //����BUFFER��С
	DMA1_Stream7->M0AR = (uint32_t)BufferSRC;   //���õ�ַ
	DMA1_Stream7->CR |= DMA_SxCR_EN;	//��DMA,��ʼ����
}



/*
********************************************************************************************************************
*                 void UART5_IRQHandler(void)
*
*Description : ����5�����жϺ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void UART5_IRQHandler(void)
{
	static uint8_t Uart5_Rec_Len = 0;
	int t;
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)  //�����ж�
	{
		t= USART_ReceiveData(UART5);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
		Uart5_Rec_Len = U5_DATA_LEN - DMA1_Stream7->NDTR;	//����ӱ�֡���ݳ���
		//***********֡���ݴ�����************//

		//*************************************//
		DMA1_S0_Reset();                                       //�ָ�DMAָ�룬�ȴ���һ�εĽ���

    }
	else if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_TC);

		DMA1_Stream7->CR &= (uint32_t)(~DMA_SxCR_EN);   //�ر�DMA,�������
	}
}
/*
********************************************************************************************************************
*                 void DMA1_Stream7_IRQHandler(void))
*
*Description : ����5�����жϺ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void DMA1_Stream7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream7,DMA_IT_TCIF7) != RESET)
    {
        /* �����־λ */
        DMA_ClearFlag(DMA1_Stream7,DMA_IT_TCIF7);
        /* �ر�DMA */
        DMA_Cmd(DMA1_Stream7,DISABLE);
        /* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
        USART_ITConfig(UART5,USART_IT_TC,ENABLE);
    }
}

/*
********************************************************************************************************************
*                  void DMA1_S0_Reset(void)
*
*Description : ����5�������� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void DMA1_S0_Reset(void)//DMA1_Stream0 ��������
{ 
	DMA_ClearFlag(DMA1_Stream0,DMA_IT_TCIF0|DMA_FLAG_HTIF0|DMA_FLAG_TEIF0|DMA_FLAG_DMEIF0|DMA_FLAG_FEIF0);  
	
	DMA_Cmd(DMA1_Stream0,DISABLE); //�ر�USART2 TX DMA1 ��ָʾ��ͨ��

 	//DMA_SetCurrDataCounter(DMA1_Channel5,Uart1_DMA_Len);//DMAͨ����DMA����Ĵ�С
	DMA1_Stream0->NDTR = U5_DATA_LEN;
 	//DMA_Cmd(DMA1_Channel5, ENABLE);                    
	DMA_Cmd(DMA1_Stream0,ENABLE); //ʹ��USART2 TX DMA1 ��ָʾ��ͨ��
	/* �����־λ */
   
}



