#include "usart1.h"


int sbus_adu[7];

#define U2_BUFFSIZERECE  100
#define U2_BUFFSIZESEND  100

uint8_t u1_receive_buff[U2_BUFFSIZERECE] = {0};
//uint8_t u1_send_buff[U4_BUFFSIZESEND]    = {0};

#define USARTx                           USART1
#define USARTx_CLK                       RCC_APB2Periph_USART1
#define USARTx_CLK_INIT                  RCC_APB2PeriphClockCmd
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_9             
#define USARTx_TX_GPIO_PORT              GPIOA                 
#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE                 GPIO_PinSource9
#define USARTx_TX_AF                     GPIO_AF_USART1

#define USARTx_RX_PIN                    GPIO_Pin_10       
#define USARTx_RX_GPIO_PORT              GPIOA                  
#define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USARTx_RX_SOURCE                 GPIO_PinSource10
#define USARTx_RX_AF                     GPIO_AF_USART1

#define USARTx_DMA                       DMA2
#define USARTx_DMAx_CLK                  RCC_AHB1Periph_DMA2
#define DMACLEAR                         USARTx_DMA->HIFCR = DMA_Stream6_IT_MASK


#define USART_RX_DMA 											DMA2_Stream1
#define USART_RX_DMA_Channel   						DMA_Channel_5
#define USART_RX_DMA_PeripheralBaseAddr  		(uint32_t) (&(USARTx->DR))

#define USART_TX_DMA 											DMA2_Stream6
#define USART_TX_DMA_Channel   						DMA_Channel_5
#define USART_TX_DMA_PeripheralBaseAddr  		(uint32_t) (&(USARTx->DR))



static void bsp_initUSART(u32 bound);
static void USART6_DMA_RxConfig(void);
static void USART6_RX_DMAReset(void);
static void USART6_DMA_Tx_init(uint32_t *BufferSRC, uint32_t BufferSize);

void USART6_Init(u32 bound)
{
	bsp_initUSART(bound);
}



static void bsp_initUSART(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK|USARTx_RX_GPIO_CLK, ENABLE);//使能GPIOA时钟

	USARTx_CLK_INIT(USARTx_CLK, ENABLE);

	RCC_AHB1PeriphClockCmd(USARTx_DMAx_CLK, ENABLE);  //使能DMA1时钟

	//串口3对应引脚复用映射
	GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
	GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

	 //USART端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
	GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN; 
	GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

  //USART3初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	
	USART6_DMA_RxConfig();   //配置DMA
	USART6_DMA_Tx_init(0,10);   //配置发送DMA
	USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);

	USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(USART_TX_DMA, DMA_IT_TC, ENABLE);  //开启相关中断	

   //USART NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;           //串口4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  	//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         	//子优先级0  第一时间响应
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);															//根据指定的参数初始化VIC寄存器、

	USART_ITConfig(USARTx,USART_IT_IDLE,ENABLE);
	USART_ITConfig(USARTx,USART_IT_TC,ENABLE);
	USART_Cmd(USARTx, ENABLE);
	USART_ClearFlag(USARTx, USART_FLAG_TC);
		
}

static void USART6_DMA_RxConfig(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	DMA_DeInit(USART_RX_DMA);
	DMA_Cmd(USART_RX_DMA, DISABLE); 
	while (DMA_GetCmdStatus(USART_RX_DMA) != DISABLE)
	{
	}
	
	DMA_InitStructure.DMA_BufferSize = U2_BUFFSIZERECE;              
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;   
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;       
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                 
	DMA_InitStructure.DMA_PeripheralBaseAddr =USART_RX_DMA_PeripheralBaseAddr ; 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;     
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                  
	

	DMA_InitStructure.DMA_Channel = USART_RX_DMA_Channel ;    
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;    
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)u1_receive_buff ;
	DMA_Init(USART_RX_DMA,&DMA_InitStructure);
	
	DMA_Cmd(USART_RX_DMA, ENABLE); //开启DMA传输
}


#define TRANSFER_IT_ENABLE_MASK (uint32_t)(DMA_SxCR_TCIE | DMA_SxCR_HTIE | \
                                           DMA_SxCR_TEIE | DMA_SxCR_DMEIE)

#define DMA_Stream0_IT_MASK     (uint32_t)(DMA_LISR_FEIF0 | DMA_LISR_DMEIF0 | \
                                           DMA_LISR_TEIF0 | DMA_LISR_HTIF0 | \
                                           DMA_LISR_TCIF0)

#define DMA_Stream1_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK << 6)
#define DMA_Stream2_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK << 16)
#define DMA_Stream3_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK << 22)
#define DMA_Stream4_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK | (uint32_t)0x20000000)
#define DMA_Stream5_IT_MASK     (uint32_t)(DMA_Stream1_IT_MASK | (uint32_t)0x20000000)
#define DMA_Stream6_IT_MASK     (uint32_t)(DMA_Stream2_IT_MASK | (uint32_t)0x20000000)
#define DMA_Stream7_IT_MASK     (uint32_t)(DMA_Stream3_IT_MASK | (uint32_t)0x20000000)


static void USART6_DMA_Tx_init(uint32_t *BufferSRC, uint32_t BufferSize)
{
	DMA_InitTypeDef  DMA_InitStructure;
	

	DMA_DeInit(USART_TX_DMA);	
	while (DMA_GetCmdStatus(USART_TX_DMA) != DISABLE)
	{
	}
	DMA_InitStructure.DMA_BufferSize = BufferSize;/* 配置DMA大小 */
 	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;                  /* 在这个程序里面使能或者禁止都可以的 */
 	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;          /* 设置阀值 */
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;           /* 设置内存为单字节突发模式 */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        /* 设置内存数据的位宽是字节 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                /* 使能地址自增 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                          /* 设置DMA是正常模式 */
	DMA_InitStructure.DMA_PeripheralBaseAddr =USART_TX_DMA_PeripheralBaseAddr; /* 设置外设地址 */
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    /* 设置外设为单字节突发模式 */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;; /* 设置外设数据的位宽是字节 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 禁止外设地址自增 */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;             /* 设置优先级 */
	
	/* 配置 TX DMA */
	DMA_InitStructure.DMA_Channel = USART_TX_DMA_Channel ;      /* 配置发送通道 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;     /* 设置从内存到外设 */
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)BufferSRC ; /* 设置内存地址 */
	DMA_Init(USART_TX_DMA,&DMA_InitStructure);
	
}

void USART6_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{
		
	while (DMA_GetCmdStatus(USART_TX_DMA) != DISABLE)
	{
	}
	
	USART_TX_DMA->NDTR = BufferSize;
	USART_TX_DMA->M0AR = (uint32_t)BufferSRC;

	DMA_Cmd(USART_TX_DMA, ENABLE); 
	
	
}

static void USART6_RX_DMAReset(void)
{ 
	DMA_Cmd(USART_RX_DMA, DISABLE);   
	while (DMA_GetCmdStatus(USART_RX_DMA) != DISABLE)
	{
	}
 	DMA_SetCurrDataCounter(USART_RX_DMA,U2_BUFFSIZERECE);//DMA通道的DMA缓存的大小
 	DMA_Cmd(USART_RX_DMA, ENABLE); 
}	

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

uint16_t  re_ch[16];
void USARTx_IRQHandler(void)
{	
	if (USART_GetITStatus(USARTx, USART_IT_IDLE) == SET)       
	{
		 USART_ReceiveData(USARTx);                       
		 uint8_t Uart1_Rec_Len = U2_BUFFSIZERECE - DMA_GetCurrDataCounter(USART_RX_DMA);			
	   USART6_RX_DMAReset();
		 //数据帧处理
		 //直接在中断中接收处理数据 		
		sbus_decode(u1_receive_buff,re_ch);		
		
	}
	
	else if(USART_GetITStatus(USARTx, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USARTx, USART_IT_TC);         //清除中断标志
		
		DMACLEAR;
		DMA_Cmd(USART_TX_DMA, DISABLE); 
	}

}

int sbus_re_calculate(int *Data1,int *Data2,int *Data3,int *Data4)//译码  遥控解码数据换算
{

		*Data1=50-((float)re_ch[0]/2048.f)*100; //抬升
		*Data2=((float)re_ch[1]/2048.f)*100-50; //压头
		*Data3=((float)re_ch[2]/2048.f)*100;	  //右转机身
		*Data4=50-((float)re_ch[3]/2048.f)*100;	//左转机身
	return 1;
}




