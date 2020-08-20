/*
******************************************************************************************************************
*Filename      :usart3_dma.c
*Programmer(s) :chu
*Description   :Design for usart3_dma

							 USART3_RX---PD9
							 USART3_TX---PD8
******************************************************************************************************************
*/


#include "usart3_dma.h"

u8 rec_data_u3[U3_DATA_LEN];   // ��������
u8 send_data_u3[U3_DATA_LEN];


/*
********************************************************************************************************************
*                  void  USART3_Init(void)
*
*Description : ����3�ĳ�ʼ�� 
							 USART3_RX---PD9
							 USART3_TX---PD8
*Arguments   : Baud������������
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void USART3_Init(int Baud)
{
	NVIC_InitTypeDef NVIC_InitStructure ;//�����жϽṹ��
 	GPIO_InitTypeDef GPIO_InitStructure;//����IO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;//���崮�ڽṹ��
	DMA_InitTypeDef DMA_InitStructure;//����DMA�ṹ��

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//�򿪴��ڶ�Ӧ������ʱ��

	//**********************���� ���� DMA ���ã�DMA1_Stream6��**************************
	// 0 ����DMAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	// 1 DMA�����ж�����
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 2 DMAͨ������
	DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)send_data_u3;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U3_DATA_LEN;//����DMA�ڴ���ʱ�������ĳ���
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
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	// 4 ʹ���ж�
	DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);


	/* 5.ʹ�ܴ��ڵ�DMA���ͽӿ� */
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	// 6 �ر�ͨ��
	DMA_Cmd(DMA1_Stream3,DISABLE);//�ȹر�

	//**********************���� ���� DMA ����**************************
	// 0 ����DMAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//DMAͨ������
	
	// 2 DMAͨ������
	DMA_DeInit(DMA1_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rec_data_u3;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U3_DATA_LEN;//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//����DMA�����ȼ���

	// 3 ����DMA1��ͨ��
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	// 4 ʹ��ͨ��
	DMA_Cmd(DMA1_Stream1,ENABLE);

	/* 5.ʹ�ܴ��ڵ�DMA���� */
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);


    //��ʼ�����ڲ���
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_BaudRate = Baud;
	//��ʼ������
    USART_Init(USART3,&USART_InitStructure);


	//�����ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;               //ͨ������Ϊ�����ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       //�ж�ռ�ȵȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              //�ж���Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�
    NVIC_Init(&NVIC_InitStructure);

	//����DMA��ʽ����
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	//����DMA��ʽ����
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);

	//�ж�����
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);


    //��������
    USART_Cmd(USART3, ENABLE);

    //����IO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	 //�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	    //����:����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 //������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//IO���ٶ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  //�ܽ�ָ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	 //������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    //�ܽ�ָ��
	GPIO_Init(GPIOB, &GPIO_InitStructure);      //��ʼ��
}

/*
********************************************************************************************************************
*                  void  USART3_IRQHandler(void)
*
*Description : ����3���ͺ��� 
*Arguments   : BufferSRC:�������ݴ�ŵ�ַ��BufferSize:���������ֽ���
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void USART3_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize)//UASRT DMA��������
{
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
	DMA1_Stream3->NDTR = BufferSize;    //����BUFFER��С
	DMA1_Stream3->M0AR = (uint32_t)BufferSRC;   //���õ�ַ
	DMA1_Stream3->CR |= DMA_SxCR_EN;	//��DMA,��ʼ����
}



/*
********************************************************************************************************************
*                  void  USART3_IRQHandler(void)
*
*Description : ����3���պ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void USART3_IRQHandler(void)
{
	static uint8_t Uart3_Rec_Len = 0;
	int t;
	uint8_t i;
	u8 sum;
	uint16_t distance=0;
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //�����ж�
	{
		t= USART_ReceiveData(USART3);//��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
		Uart3_Rec_Len = U3_DATA_LEN - DMA1_Stream1->NDTR;	//����ӱ�֡���ݳ���

		//***********���⺯��************//
	
//    USART3_DMATransfer((u32*)rec_data_u3,sizeof(rec_data_u3));
		//*************************************//
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //����жϱ�־
		DMA1_S1_Reset();                                       //�ָ�DMAָ�룬�ȴ���һ�εĽ���
		
    }
	else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);

		DMA1_Stream3->CR &= (uint32_t)(~DMA_SxCR_EN);   //�ر�DMA,�������
	}
}
/*
********************************************************************************************************************
*                  void DMA1_Stream3_IRQHandler(void)
*
*Description : ����3�����жϺ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void DMA1_Stream3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3) != RESET)
    {
        /* �����־λ */
        DMA_ClearFlag(DMA1_Stream3,DMA_IT_TCIF3);
        /* �ر�DMA */
        DMA_Cmd(DMA1_Stream3,DISABLE);
        /* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
        USART_ITConfig(USART3,USART_IT_TC,ENABLE);
    }
}

/*
********************************************************************************************************************
*                  void DMA1_S1_Reset(void)
*
*Description : ����3�������� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void DMA1_S1_Reset(void)//DMA1_Stream6 ��������
{ 
	DMA_ClearFlag(DMA1_Stream1,DMA_IT_TCIF1|DMA_FLAG_HTIF1|DMA_FLAG_TEIF1|DMA_FLAG_DMEIF1|DMA_FLAG_FEIF1);  
	
	DMA_Cmd(DMA1_Stream1,DISABLE); //�ر�USART2 TX DMA1 ��ָʾ��ͨ��

 	//DMA_SetCurrDataCounter(DMA1_Channel5,Uart1_DMA_Len);//DMAͨ����DMA����Ĵ�С
	DMA1_Stream1->NDTR = U3_DATA_LEN;
 	//DMA_Cmd(DMA1_Channel5, ENABLE);                    
	DMA_Cmd(DMA1_Stream1,ENABLE); //ʹ��USART2 TX DMA1 ��ָʾ��ͨ��
	/* �����־λ */
   
}

void Upper3_f_Computer(float data1,float data2,float data3,float data4)
{
	u8 i,sum;
	static u8 Send_DATA[21];       
	Send_DATA[0] =0xAA;    
	Send_DATA[1] =0xAA;			
	Send_DATA[2] =0xF1;    
	Send_DATA[3] =16;      
	
	Send_DATA[4]  = BYTE3(data1);  
	Send_DATA[5]  = BYTE2(data1);
	Send_DATA[6]  = BYTE1(data1);
	Send_DATA[7]  = BYTE0(data1);
	
	Send_DATA[8]  = BYTE3(data2);
	Send_DATA[9]  = BYTE2(data2);
	Send_DATA[10] = BYTE1(data2);
	Send_DATA[11] = BYTE0(data2);
	
	Send_DATA[12] = BYTE3(data3);
	Send_DATA[13] = BYTE2(data3);
	Send_DATA[14] = BYTE1(data3);
	Send_DATA[15] = BYTE0(data3);
	
	Send_DATA[16] = BYTE3(data4);
	Send_DATA[17] = BYTE2(data4);
	Send_DATA[18] = BYTE1(data4);
	Send_DATA[19] = BYTE0(data4);
	
	sum = 0;
	for(i=0;i<20;i++)
		sum += Send_DATA[i];
	
	Send_DATA[20]=sum;
	USART3_DMATransfer((u32*)Send_DATA,21);
}


