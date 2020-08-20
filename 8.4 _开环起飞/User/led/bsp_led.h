#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

//LED 

#define LED1_PIN                  GPIO_Pin_15                 
#define LED1_GPIO_PORT            GPIOB                     
#define LED1_GPIO_CLK             RCC_AHB1Periph_GPIOB


/************************************************************/


#define ON  1
#define OFF 0

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN)

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//输出反转状态

/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_ON			  digitalHi(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF			digitalLo(LED1_GPIO_PORT,LED1_PIN)

void LED_Init(void);


#endif /* __LED_H */
