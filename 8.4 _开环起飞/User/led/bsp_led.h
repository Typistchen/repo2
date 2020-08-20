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

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_PIN)

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//����Ϊ�ߵ�ƽ
#define digitalLo(p,i)			 {p->BSRRH=i;}		//����͵�ƽ
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//�����ת״̬

/* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_ON			  digitalHi(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF			digitalLo(LED1_GPIO_PORT,LED1_PIN)

void LED_Init(void);


#endif /* __LED_H */
