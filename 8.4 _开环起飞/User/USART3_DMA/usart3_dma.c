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

u8 rec_data_u3[U3_DATA_LEN];   // 接收数据
u8 send_data_u3[U3_DATA_LEN];


/*
********************************************************************************************************************
*                  void  USART3_Init(void)
*
*Description : 串口3的初始化 
							 USART3_RX---PD9
							 USART3_TX---PD8
*Arguments   : Baud：波特率配置
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void USART3_Init(int Baud)
{
	NVIC_InitTypeDef NVIC_InitStructure ;//定义中断结构体
 	GPIO_InitTypeDef GPIO_InitStructure;//定义IO初始化结构体
	USART_InitTypeDef USART_InitStructure;//定义串口结构体
	DMA_InitTypeDef DMA_InitStructure;//定义DMA结构体

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//打开串口对应的外设时钟

	//**********************串口 发送 DMA 设置（DMA1_Stream6）**************************
	// 0 启动DMA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	// 1 DMA发送中断设置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// 2 DMA通道配置
	DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)send_data_u3;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//dma传输方向
	DMA_InitStructure.DMA_BufferSize = U3_DATA_LEN;//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//设置DMA的内存递增模式

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//设置DMA的传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//设置DMA的优先级别
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    //指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  	//指定了FIFO阈值水平
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //指定的Burst转移配置内存传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //指定的Burst转移配置外围转移 */

	// 3 配置DMA1的通道
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	// 4 使能中断
	DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);


	/* 5.使能串口的DMA发送接口 */
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	// 6 关闭通道
	DMA_Cmd(DMA1_Stream3,DISABLE);//先关闭

	//**********************串口 接受 DMA 设置**************************
	// 0 启动DMA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//DMA通道配置
	
	// 2 DMA通道配置
	DMA_DeInit(DMA1_Stream1);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rec_data_u3;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//dma传输方向
	DMA_InitStructure.DMA_BufferSize = U3_DATA_LEN;//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//设置DMA的内存递增模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//设置DMA的传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//设置DMA的优先级别

	// 3 配置DMA1的通道
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	// 4 使能通道
	DMA_Cmd(DMA1_Stream1,ENABLE);

	/* 5.使能串口的DMA接收 */
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);


    //初始化串口参数
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_BaudRate = Baud;
	//初始化串口
    USART_Init(USART3,&USART_InitStructure);


	//配置中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;               //通道设置为串口中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       //中断占先等级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;              //中断响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断
    NVIC_Init(&NVIC_InitStructure);

	//采用DMA方式发送
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	//采用DMA方式接收
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);

	//中断配置
	USART_ITConfig(USART3,USART_IT_TC,DISABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);


    //启动串口
    USART_Cmd(USART3, ENABLE);

    //设置IO口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	 //管脚模式:输出口
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	    //类型:推挽模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 //上拉下拉设置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//IO口速度
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  //管脚指定
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//管脚模式:输入口
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	 //上拉下拉设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    //管脚指定
	GPIO_Init(GPIOB, &GPIO_InitStructure);      //初始化
}

/*
********************************************************************************************************************
*                  void  USART3_IRQHandler(void)
*
*Description : 串口3发送函数 
*Arguments   : BufferSRC:发送数据存放地址；BufferSize:发送数据字节数
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void USART3_DMATransfer(uint32_t *BufferSRC, uint32_t BufferSize)//UASRT DMA发送设置
{
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
	DMA1_Stream3->NDTR = BufferSize;    //配置BUFFER大小
	DMA1_Stream3->M0AR = (uint32_t)BufferSRC;   //配置地址
	DMA1_Stream3->CR |= DMA_SxCR_EN;	//打开DMA,开始发送
}



/*
********************************************************************************************************************
*                  void  USART3_IRQHandler(void)
*
*Description : 串口3接收函数 
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
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //接收中断
	{
		t= USART_ReceiveData(USART3);//读取数据 注意：这句必须要，否则不能够清除中断标志位。
		Uart3_Rec_Len = U3_DATA_LEN - DMA1_Stream1->NDTR;	//算出接本帧数据长度

		//***********激光函数************//
	
//    USART3_DMATransfer((u32*)rec_data_u3,sizeof(rec_data_u3));
		//*************************************//
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);         //清除中断标志
		DMA1_S1_Reset();                                       //恢复DMA指针，等待下一次的接收
		
    }
	else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);

		DMA1_Stream3->CR &= (uint32_t)(~DMA_SxCR_EN);   //关闭DMA,发送完成
	}
}
/*
********************************************************************************************************************
*                  void DMA1_Stream3_IRQHandler(void)
*
*Description : 串口3发送中断函数 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void DMA1_Stream3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3) != RESET)
    {
        /* 清除标志位 */
        DMA_ClearFlag(DMA1_Stream3,DMA_IT_TCIF3);
        /* 关闭DMA */
        DMA_Cmd(DMA1_Stream3,DISABLE);
        /* 打开发送完成中断,确保最后一个字节发送成功 */
        USART_ITConfig(USART3,USART_IT_TC,ENABLE);
    }
}

/*
********************************************************************************************************************
*                  void DMA1_S1_Reset(void)
*
*Description : 串口3接收重置 
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/

void DMA1_S1_Reset(void)//DMA1_Stream6 接收重置
{ 
	DMA_ClearFlag(DMA1_Stream1,DMA_IT_TCIF1|DMA_FLAG_HTIF1|DMA_FLAG_TEIF1|DMA_FLAG_DMEIF1|DMA_FLAG_FEIF1);  
	
	DMA_Cmd(DMA1_Stream1,DISABLE); //关闭USART2 TX DMA1 所指示的通道

 	//DMA_SetCurrDataCounter(DMA1_Channel5,Uart1_DMA_Len);//DMA通道的DMA缓存的大小
	DMA1_Stream1->NDTR = U3_DATA_LEN;
 	//DMA_Cmd(DMA1_Channel5, ENABLE);                    
	DMA_Cmd(DMA1_Stream1,ENABLE); //使能USART2 TX DMA1 所指示的通道
	/* 清除标志位 */
   
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


