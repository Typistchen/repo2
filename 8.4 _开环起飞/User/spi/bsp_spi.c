#include "bsp_spi.h"

 /**
  * @brief  SPI��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_OLED_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ�� OLED_SPI ��GPIO ʱ�� */
  /*!< SPI_OLED_SPI_CS_GPIO, SPI_OLED_SPI_MOSI_GPIO, 
       SPI_OLED_SPI_MISO_GPIO,SPI_OLED_SPI_SCK_GPIO ʱ��ʹ�� */
  RCC_AHB1PeriphClockCmd (OLED_SPI_SCK_GPIO_CLK |OLED_SPI_MOSI_GPIO_CLK|OLED_CS_GPIO_CLK, ENABLE);

  /*!< SPI_OLED_SPI ʱ��ʹ�� */
  OLED_SPI_CLK_INIT(OLED_SPI_CLK, ENABLE);
 
  //�������Ÿ���
  GPIO_PinAFConfig(OLED_SPI_SCK_GPIO_PORT,OLED_SPI_SCK_PINSOURCE,OLED_SPI_SCK_AF); 
	GPIO_PinAFConfig(OLED_SPI_MOSI_GPIO_PORT,OLED_SPI_MOSI_PINSOURCE,OLED_SPI_MOSI_AF); 
  
  /*!< ���� SPI_OLED_SPI ����: SCK */
  GPIO_InitStructure.GPIO_Pin = OLED_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(OLED_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_OLED_SPI ����: MISO */

  
	/*!< ���� SPI_OLED_SPI ����: MOSI */
  GPIO_InitStructure.GPIO_Pin = OLED_SPI_MOSI_PIN;
  GPIO_Init(OLED_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/*!< ���� SPI_OLED_SPI ����: CS */
  GPIO_InitStructure.GPIO_Pin = OLED_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(OLED_CS_GPIO_PORT, &GPIO_InitStructure);

  /* ֹͣ�ź� OLED: CS���Ÿߵ�ƽ*/
  SPI_OLED_CS_HIGH();

  /* OLED_SPI ģʽ���� */
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

  /* ʹ�� OLED_SPI  */
  SPI_Cmd(OLED_SPI, ENABLE);

}


//SPI3 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
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
	}//�ȴ���������
	
	SPI_I2S_SendData(SPI3, TxData); //ͨ������SPIx����һ��byte  ����
	timeOut = 0;		
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
	{
		timeOut ++;
		if(timeOut == 10)
		{
			return 0;
		}	
	} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����	
 		    
}
  
   
/*********************************************END OF FILE**********************/
