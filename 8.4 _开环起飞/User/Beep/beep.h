#ifndef __BEEP_H
#define	__BEEP_H

#include "stm32f4xx.h"

//BEEP PC3
#define BEEP_PIN                  GPIO_Pin_1               
#define BEEP_GPIO_PORT            GPIOB                     
#define BEEP_GPIO_CLK             RCC_AHB1Periph_GPIOB

/************************************************************/
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//输出反转状态

/* 定义控制IO的宏 */
#define BEEP_TOGGLE		digitalToggle(BEEP_GPIO_PORT,BEEP_PIN)
#define BEEP_ON			  digitalHi(BEEP_GPIO_PORT,BEEP_PIN)
#define BEEP_OFF			digitalLo(BEEP_GPIO_PORT,BEEP_PIN)


void BEEP_Init(void);


#endif /* __LED_H */
