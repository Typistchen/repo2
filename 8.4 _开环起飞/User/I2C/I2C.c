#include "I2C.h"

/**************************实现函数********************************************
*函数原型: void IIC_Init(void)
*功　　能: 初始化I2C对应的接口引脚。
//SCL-->PB8
//SDA-->PB9
*******************************************************************************/
void IIC_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(I2C_GPIO_CLK, ENABLE);	                                                   /* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);

	IIC_SCL=1;
	IIC_SDA=1;	
}

void Delay_IIC(unsigned char n)
{
	unsigned int i,j;
	for(j = 0;j < n;j++)
	{
		for(i=0;i<5;i++);
	}
}

/**************************实现函数********************************************
*函数原型:	 void IIC_Start(void)
*功　　能: 产生IIC起始信号
*******************************************************************************/
void IIC_Start(void)
{
	SDA_OUT();                                                                                       //sda线输出 
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay_IIC(10);
 	IIC_SDA=0;                                                                                       //START:when CLK is high,DATA change form high to low 
	Delay_IIC(10);
	IIC_SCL=0;                                                                                       //钳住I2C总线，准备发送或接收数据 
}

/**************************实现函数********************************************
*函数原型:	 void IIC_Stop(void)
*功　　能:	 产生IIC停止信号
*******************************************************************************/	  
void IIC_Stop(void)
{
	SDA_OUT();                                                                                       //sda线输出
	IIC_SCL=0;
	IIC_SDA=0;                                                                                       //STOP:when CLK is high DATA change form low to high
	Delay_IIC(10);
	IIC_SCL=1; 
	IIC_SDA=1;                                                                                       //发送I2C总线结束信号
	Delay_IIC(10);						   	
}

/**************************实现函数********************************************
*函数原型:	 u8 IIC_Wait_Ack(void)
*功　　能:	 等待应答信号到来 
*返回值    
          1:接收应答失败
          0:接收应答成功
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();                                                                                        //SDA设置为输入  
	IIC_SDA=1;	Delay_IIC(10);	   
	IIC_SCL=1;	Delay_IIC(10); 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;
		}
	Delay_IIC(10);
	}
	IIC_SCL=0;                                                                                       //时钟输出0 	   
	return 0;  
} 

/**************************实现函数********************************************
*函数原型:	 void IIC_Ack(void)
*功　　能:	 产生ACK应答
*******************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	Delay_IIC(10);
	IIC_SCL=1;
	Delay_IIC(10);
	IIC_SCL=0;
}
	
/**************************实现函数********************************************
*函数原型:	 void IIC_NAck(void)
*功　　能:	 产生NACK应答
*******************************************************************************/	    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	Delay_IIC(10);
	IIC_SCL=1;
	Delay_IIC(10);
	IIC_SCL=0;
}					 				     

/**************************实现函数********************************************
*函数原型:		void IIC_Send_Byte(u8 txd)
*功　　能:	  IIC发送一个字节
*******************************************************************************/		  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;                                                                                     //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
				Delay_IIC(10); 
				IIC_SCL=1;
				Delay_IIC(10);
				IIC_SCL=0;	
				Delay_IIC(10);
    }	 
} 	 
   
/**************************实现函数********************************************
*函数原型:		u8 IIC_Read_Byte(unsigned char ack)
*功　　能:	  读1个字节，
            ack=1，发送ACK，
            ack=0，发送nACK 
*******************************************************************************/  
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();                                                                                        //SDA设置为输入
	for(i=0;i<8;i++)
	{
		IIC_SCL=0; 
		Delay_IIC(10);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;   
		Delay_IIC(10);
	}					 
	if (ack)
			IIC_Ack();                                                                                   //发送ACK 
	else
			IIC_NAck();                                                                                  //发送nACK  
	return receive;
}

/**************************实现函数***********************************************
*函数原型:	unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*功　　能:	读取指定设备 指定寄存器的一个值
*输入参数：
					I2C_Addr : 目标设备地址
					addr	   : 寄存器地址
*返回值： 设备addr地址上的一个数据
**********************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC_Start();	
	IIC_Send_Byte(I2C_Addr);	                                                                       //发送写命令
	res++;
	IIC_Wait_Ack();
	IIC_Send_Byte(addr); res++;                                                                      //发送地址
	IIC_Wait_Ack();	  
	//IIC_Stop();//产生一个停止条件	                                                                                 
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); res++;                                                                //进入接收模式			   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
  IIC_Stop();                                                                                      //产生一个停止条件

	return res;
}


/**************************实现函数********************************************
*函数原型:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	  读取指定设备,指定寄存器的length个值
*输入参数: 
						dev    : 目标设备地址
						reg	   : 寄存器地址
						length : 要读的字节数
						*data  : 读出的数据将要存放的指针
*返回值：   读取到多少个数据
*******************************************************************************/ 
u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
  u8 count = 0;
	u8 temp;
	IIC_Start();
	IIC_Send_Byte(dev);	                                                                             //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);                                                                              //发送地址
  IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(dev+1);                                                                            //进入接收模式	
	IIC_Wait_Ack();
	
  for(count=0;count<length;count++)
	{
		if(count!=(length-1))
		  temp = IIC_Read_Byte(1);                                                                     //带ACK的读数据
		else  
			temp = IIC_Read_Byte(0);	                                                                   //最后一个字节NACK
		data[count] = temp;
	}
    IIC_Stop();                                                                                    //产生一个停止条件
    return count;
}

/**************************实现函数********************************************
*函数原型:	u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能:	将多个字节写入指定设备 指定寄存器
*输入参数:
					dev    : 目标设备地址
					reg	   : 寄存器地址
					length : 要写的字节数
					*data  : 将要写的数据的首地址
返回值:   返回是否成功
*******************************************************************************/ 

u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
{
 	u8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	                                                                             //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);                                                                              //发送地址
  IIC_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	}
	IIC_Stop();                                                                                      //产生一个停止条件

  return 1;                                                                                        //status == 0;
}

/**************************实现函数********************************************
*函数原型:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*功　　能:	  读取指定设备 指定寄存器的一个值
*输入参数	
						dev   : 目标设备地址
						reg	  : 寄存器地址
						*data : 读出的数据将要存放的地址
*返回值:    1
*******************************************************************************/ 
u8 IICreadByte(u8 dev, u8 reg, u8 *data)
{
	*data=I2C_ReadOneByte(dev, reg);
  return 1;
}

/**************************实现函数********************************************
*函数原型:	 unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*功　　能:	 写入指定设备 指定寄存器一个字节
*输入参数	
					dev  : 目标设备地址
					reg  : 寄存器地址
					data : 将要写入的字节
*返回值:   1
*******************************************************************************/ 
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
  return IICwriteBytes(dev, reg, 1, &data);
}

/**************************实现函数********************************************
*函数原型:	 u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*功　　能:	 读 修改 写 指定设备 指定寄存器一个字节 中的多个位
*输入参数:	
						dev     : 目标设备地址
						reg	    : 寄存器地址
						bitStart: 目标字节的起始位
						length  : 位长度
						data    : 存放改变目标字节位的值
*返回值: 
						成功 :1 
						失败 :0
*******************************************************************************/ 
u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

	u8 b;
	if (IICreadByte(dev, reg, &b) != 0) 
	{
		u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		return IICwriteByte(dev, reg, b);
	}else  return 0;
}

/**************************实现函数********************************************
*函数原型:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*功　　能:	  读 修改 写 指定设备 指定寄存器一个字节 中的1个位
*输入参数:
						dev   : 目标设备地址
						reg	  : 寄存器地址
						bitNum: 要修改目标字节的bitNum位
						data  : 
										0:目标位将被清0 
										1:目标位将被置位
*返回值:  
						成功  : 1 
						失败  : 0
*******************************************************************************/ 
u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
    u8 b;
    IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    
    return IICwriteByte(dev, reg, b);
}
