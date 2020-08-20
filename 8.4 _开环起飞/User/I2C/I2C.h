#ifndef __I2C_H
#define __I2C_H

#include "stm32f4xx.h"
#include "GPIO.h"	   
////驱动接口，GPIO模拟IIC
//SCL-->PB8
//SDA-->PB9
#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	         //PB7输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;}          //PB7输出模式

#define I2C_GPIO_PORT				GPIOB			                                       /* GPIO端口 */
#define I2C_GPIO_CLK			 	RCC_AHB1Periph_GPIOB		                         /* GPIO端口时钟 */
#define I2C_SCL_PIN					GPIO_Pin_6			                                 /* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN					GPIO_Pin_7			                                 /* 连接到SDA数据线的GPIO */

//IO操作函数	 
#define IIC_SCL    PBout(6)                                                  //SCL
#define IIC_SDA    PBout(7)                                                  //SDA	 
#define READ_SDA   PBin(7)                                                   //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                                                         //初始化IIC的IO口				 
void IIC_Start(void);			   	                                               //发送IIC开始信号
void IIC_Stop(void);	  			                                               //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			                                             //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);                                         //IIC读取一个字节
u8 IIC_Wait_Ack(void); 				                                               //IIC等待ACK信号
void IIC_Ack(void);					                                                 //IIC发送ACK信号
void IIC_NAck(void);				                                                 //IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	 
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 IICwriteBit(u8 dev,u8 reg,u8 bitNum,u8 data);
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data);

#endif

//*************************************************************************************






