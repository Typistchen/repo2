#include "bsp_spi.h"

 /**
  * @brief  SPI初始化
  * @param  无
  * @retval 无
  */
void SPI_OLED_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 使能 OLED_SPI 及GPIO 时钟 */
  /*!< SPI_OLED_SPI_CS_GPIO, SPI_OLED_SPI_MOSI_GPIO, 
       SPI_OLED_SPI_MISO_GPIO,SPI_OLED_SPI_SCK_GPIO 时钟使能 */
  RCC_AHB1PeriphClockCmd (OLED_SPI_SCK_GPIO_CLK |OLED_SPI_MOSI_GPIO_CLK|OLED_CS_GPIO_CLK, ENABLE);

  /*!< SPI_OLED_SPI 时钟使能 */
  OLED_SPI_CLK_INIT(OLED_SPI_CLK, ENABLE);
 
  //设置引脚复用
  GPIO_PinAFConfig(OLED_SPI_SCK_GPIO_PORT,OLED_SPI_SCK_PINSOURCE,OLED_SPI_SCK_AF); 
	GPIO_PinAFConfig(OLED_SPI_MOSI_GPIO_PORT,OLED_SPI_MOSI_PINSOURCE,OLED_SPI_MOSI_AF); 
  
  /*!< 配置 SPI_OLED_SPI 引脚: SCK */
  GPIO_InitStructure.GPIO_Pin = OLED_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(OLED_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_OLED_SPI 引脚: MISO */

  
	/*!< 配置 SPI_OLED_SPI 引脚: MOSI */
  GPIO_InitStructure.GPIO_Pin = OLED_SPI_MOSI_PIN;
  GPIO_Init(OLED_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/*!< 配置 SPI_OLED_SPI 引脚: CS */
  GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(OLED_CS_GPIO_PORT, &GPIO_InitStructure);

  /* 停止信号 OLED: CS引脚高电平*/
  SPI_OLED_CS_HIGH();

  /* OLED_SPI 模式配置 */
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 1;
  SPI_Init(OLED_SPI, &SPI_InitStructure);

  /* 使能 OLED_SPI  */
  SPI_Cmd(OLED_SPI, ENABLE);

}


//SPI3 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{		 			 
	uint8_t timeOut = 0; 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
	{
		timeOut ++;
		if(timeOut == 255)
		{
			return 0;
		}	
	}//等待发送区空
	
	SPI_I2S_SendData(SPI3, TxData); //通过外设SPIx发送一个byte  数据
	timeOut = 0;		
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
	{
		timeOut ++;
		if(timeOut == 10)
		{
			return 0;
		}	
	} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据	
 		    
}
  
   
/*********************************************END OF FILE**********************/
