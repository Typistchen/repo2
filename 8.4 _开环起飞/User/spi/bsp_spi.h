#ifndef __SPI_OLED_H
#define __SPI_OLED_H

#include "stm32f4xx.h"
#include "GPIO.h"

/*SPI接口定义-开头****************************/
#define OLED_SPI                           SPI3
#define OLED_SPI_CLK                       RCC_APB1Periph_SPI3
#define OLED_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd//SPI3:APB1;SPI1/SPI2:SPI2

#define OLED_SPI_SCK_PIN                   GPIO_Pin_3                  
#define OLED_SPI_SCK_GPIO_PORT             GPIOB                       
#define OLED_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define OLED_SPI_SCK_PINSOURCE             GPIO_PinSource3
#define OLED_SPI_SCK_AF                    GPIO_AF_SPI3

#define OLED_SPI_MOSI_PIN                  GPIO_Pin_5                
#define OLED_SPI_MOSI_GPIO_PORT            GPIOB                     
#define OLED_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define OLED_SPI_MOSI_PINSOURCE            GPIO_PinSource5
#define OLED_SPI_MOSI_AF                   GPIO_AF_SPI3

#define OLED_CS_PIN                        GPIO_Pin_5               
#define OLED_CS_GPIO_PORT                  GPIOD                     
#define OLED_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOD


#define SPI_OLED_CS_LOW()      {OLED_CS_GPIO_PORT->BSRRH=OLED_CS_PIN;}
#define SPI_OLED_CS_HIGH()     {OLED_CS_GPIO_PORT->BSRRL=OLED_CS_PIN;}


/*SPI接口定义-结尾****************************/

void SPI_OLED_Init(void);

u8 SPI3_ReadWriteByte(u8 TxData);


#endif /* __SPI_OLED_H */

