/*
******************************************************************************************************************
*Filename      :usart1_dma.c
*Programmer(s) :CJQ
*Description   :Design for usart1_dma

							 USART1_RX---PB7
							 USART1_TX---PB6
******************************************************************************************************************
*/
#include "usart1_dma.h"




#include "control.h"


u8 rec_data_u1[U1_DATA_LEN];   // ��������
u8 send_data_u1[U1_DATA_LEN];  // ��������
u8 catch_flag = 0;


/*
********************************************************************************************************************
*                  void  USART1_Init(void)
*
*Description : ����1�ĳ�ʼ�� 
							 USART1_RX---PB7
							 USART1_TX---PB6
*Arguments   : Baud������������
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void USART1_Init(int Baud)
{
	NVIC_InitTypeDef NVIC_InitStructure ; //�����жϽṹ��
 	GPIO_InitTypeDef GPIO_InitStructure;  //����IO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;//���崮�ڽṹ��
	DMA_InitTypeDef DMA_InitStructure;    //����DMA�ṹ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//�򿪴��ڶ�Ӧ������ʱ��

	//**********************���� ���� DMA ���ã�DMA1_Stream7��**************************
	// 0 ����DMAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	// 1 DMA�����ж�����
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 2 DMAͨ������
	DMA_DeInit(DMA2_Stream7);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)send_data_u1;    //�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;            //dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U1_DATA_LEN;                    //����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //����DMA���ڴ����ģʽ

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//����DMA�����ȼ���
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  	//ָ����FIFO��ֵˮƽ
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //ָ����Burstת�������ڴ洫��
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //ָ����Burstת��������Χת�� */

	// 3 ����DMA1��ͨ��
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	// 4 ʹ���ж�
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);


	/* 5.ʹ�ܴ��ڵ�DMA���ͽӿ� */
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	// 6 �ر�ͨ��
	DMA_Cmd(DMA2_Stream7,DISABLE);//�ȹر�

	//**********************���� ���� DMA ����**************************
	// 0 ����DMAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMAͨ������
	
	// 2 DMAͨ������
	DMA_DeInit(DMA2_Stream5);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);//�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rec_data_u1;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//dma���䷽��
	DMA_InitStructure.DMA_BufferSize = U1_DATA_LEN;//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//����DMA���ڴ����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����DMA�Ĵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//����DMA�����ȼ���

	// 3 ����DMA2��ͨ��
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);
	// 4 ʹ��ͨ��
	DMA_Cmd(DMA2_Stream5,ENABLE);

	/* 5.ʹ�ܴ��ڵ�DMA���� */
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);


    //��ʼ�����ڲ���
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_BaudRate = Baud;
	//��ʼ������
    USART_Init(USART1,&USART_InitStructure);


	//�����ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;               //ͨ������Ϊ�����ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       //�ж�ռ�ȵȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              //�ж���Ӧ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�
    NVIC_Init(&NVIC_InitStructure);

	//����DMA��ʽ����
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	//����DMA��ʽ����
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

	//�ж�����
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);


    //��������
    USART_Cmd(USART1, ENABLE);

    //����IO��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	 //�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	    //����:����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 //������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//IO���ٶ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  //�ܽ�ָ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//�ܽ�ģʽ:�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	 //������������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //�ܽ�ָ��
	GPIO_Init(GPIOA, &GPIO_InitStructure);      //��ʼ��
}
/*
********************************************************************************************************************
*                  void  USART1_IRQHandler(void)
*
*Description : ����1���ͺ��� 
*Arguments   : BufferSRC:�������ݴ�ŵ�ַ��BufferSize:���������ֽ���
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void USART1_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize)//UASRT DMA��������
{
	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
	DMA2_Stream7->NDTR = BufferSize;    //����BUFFER��С
	DMA2_Stream7->M0AR = (uint32_t)BufferSRC;   //���õ�ַ
	DMA2_Stream7->CR |= DMA_SxCR_EN;	//��DMA,��ʼ����
}

/*
********************************************************************************************************************
*                  void  USART1_IRQHandler(void)
*
*Description : ����1���պ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void USART1_IRQHandler(void)
{
	static uint8_t UART1_Rec_Len = 0;
	int t;
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //�����ж�
	{
		t= USART_ReceiveData(USART1);                        //��ȡ���� ע�⣺������Ҫ�������ܹ�����жϱ�־λ��
		UART1_Rec_Len = U1_DATA_LEN - DMA2_Stream5->NDTR;	   //����ӱ�֡���ݳ���         //����жϱ�־
		//***********֡���ݴ�����************//
		
		//*************************************//
//		USART_ClearITPendingBit(USART1, USART_IT_IDLE);         //����жϱ�־
		DMA2_S5_Reset();        
		//�ָ�DMAָ�룬�ȴ���һ�εĽ���
        //����жϱ�־

    }
	else if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);

		DMA2_Stream7->CR &= (uint32_t)(~DMA_SxCR_EN);   //�ر�DMA,�������
	}
}

/*
********************************************************************************************************************
*                  void DMA2_Stream7_IRQHandler(void)
*
*Description : ����1�����жϺ��� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void DMA2_Stream7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) != RESET)
    {
        /* �����־λ */
        DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);
        /* �ر�DMA */
        DMA_Cmd(DMA2_Stream7,DISABLE);
        /* �򿪷�������ж�,ȷ�����һ���ֽڷ��ͳɹ� */
        USART_ITConfig(USART1,USART_IT_TC,ENABLE);
    }
}

/*
********************************************************************************************************************
*                  void DMA2_S5_Reset(void)
*
*Description : ����1�������� 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void DMA2_S5_Reset(void)//DMA2_Stream6 ��������
{ 
	DMA_ClearFlag(DMA2_Stream5,DMA_IT_TCIF5|DMA_FLAG_HTIF5|DMA_FLAG_TEIF5|DMA_FLAG_DMEIF5|DMA_FLAG_FEIF5);  
	
	DMA_Cmd(DMA2_Stream5,DISABLE); //�ر�USART1 TX DMA1 ��ָʾ��ͨ��

 	//DMA_SetCurrDataCounter(DMA1_Channel5,Uart1_DMA_Len);//DMAͨ����DMA����Ĵ�С
	DMA2_Stream5->NDTR = U1_DATA_LEN;
 	//DMA_Cmd(DMA1_Channel5, ENABLE);                    
	DMA_Cmd(DMA2_Stream5,ENABLE); //ʹ��USART1 TX DMA1 ��ָʾ��ͨ��
	/* �����־λ */
   
}


void Upper1_f_Computer(float data1,float data2,float data3,float data4)
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
	USART1_DMATransfer((u32*)Send_DATA,21);
}



