#include "Ultra.h"  
#include "GPIO.h"
	
#define Trig1_PIN                  GPIO_Pin_2                 
#define Trig1_GPIO_PORT            GPIOE                      
#define Trig1_GPIO_CLK             RCC_AHB1Periph_GPIOE	

#define Trig2_PIN                  GPIO_Pin_11                 
#define Trig2_GPIO_PORT            GPIOD                      
#define Trig2_GPIO_CLK             RCC_AHB1Periph_GPIOD	
	
#define Trig3_PIN                  GPIO_Pin_1                 
#define Trig3_GPIO_PORT            GPIOE                      
#define Trig3_GPIO_CLK             RCC_AHB1Periph_GPIOE	

#define Trig4_PIN                  GPIO_Pin_10                 
#define Trig4_GPIO_PORT            GPIOD                      
#define Trig4_GPIO_CLK             RCC_AHB1Periph_GPIOD		

void Delay_ULTRA(unsigned char n)      //待测试
{
	unsigned int i,j;
	for(j = 0;j < n;j++)
	{
		for(i=0;i<40;i++);
	}
}

void Ultrasonic_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(Trig1_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = Trig1_PIN|Trig3_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(Trig1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(Trig1_GPIO_PORT,Trig1_PIN|Trig3_PIN); 
	
	RCC_AHB1PeriphClockCmd(Trig2_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = Trig2_PIN|Trig4_PIN;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(Trig2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(Trig2_GPIO_PORT,Trig2_PIN|Trig4_PIN); 		
	
	Ultra_TIM5_Init(0XFFFF,84-1);
	Ultra_TIM8_Init(0XFFFF,84-1);	
}
void  Ultra_TIM5_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM5_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	                         //使能TIM5,3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);                          //使能GPIOA,B时钟

	TIM_DeInit(TIM5);	                                                             //重新初始化TIM5为默认状态	

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);                          //PA0 复用为TIM5 CH1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); 	                       //PA1 复用为TIM5 CH2
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                           //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                 //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;                                 //下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //初始化PA0,1,6,7  

	//初始化TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr;                                        //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	                                   //预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                        //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                    //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);                                //根据TIM_TimeBaseInitStruct中指定的参数初始化TIM5的时间基数单位

	//初始化TIM5输入捕获参数 通道1
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;                              //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //配置输入分频,不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	//初始化TIM5输入捕获参数 通道2
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2;                              //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //配置输入分频,不分频 
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//中断分组初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                //中断优先级分组2
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;                                //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                      //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                             //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                                //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 ,ENABLE);  					 //允许更新中断，允许CC1IE,CC2IE捕获中断

	TIM_Cmd(TIM5, ENABLE); 		                                                     //使能定时器2
}


void  Ultra_TIM8_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM8_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	                         //使能TIM8时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);                          //使能GPIOC时钟

	TIM_DeInit(TIM8);	                                                             //重新初始化TIM8为默认状态	

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);                          //PC8 复用为TIM8 CH3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8); 	                       //PC9 复用为TIM8 CH4
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                   //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                           //速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                 //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;                                 //下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);                                         //初始化PC8,9  

	//初始化TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr;                                        //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	                                   //预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                        //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                    //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);                                //根据TIM_TimeBaseInitStruct中指定的参数初始化TIM5的时间基数单位

	//初始化TIM8输入捕获参数 通道3
	TIM8_ICInitStructure.TIM_Channel = TIM_Channel_3;                              //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //上升沿捕获
	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //映射到TI1上
	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //配置输入分频,不分频 
	TIM8_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM8, &TIM8_ICInitStructure);

	//初始化TIM8输入捕获参数 通道4
	TIM8_ICInitStructure.TIM_Channel = TIM_Channel_4;                              //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //上升沿捕获
	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //映射到TI1上
	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //配置输入分频,不分频 
	TIM8_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM8, &TIM8_ICInitStructure);
	
	//中断分组初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                //中断优先级分组2
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;                                //TIM8中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                      //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                             //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                                //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM8,TIM_IT_Update | TIM_IT_CC3 | TIM_IT_CC4 ,ENABLE);  					 //允许更新中断，允许CC1IE,CC2IE,CC3IE捕获中断

	TIM_Cmd(TIM8, ENABLE); 		                                                     //使能定时器2
}


Ultra_cap_t ultra_T5C1={0,0,0,0};
Ultra_cap_t ultra_T5C2={0,0,0,0};
Ultra_cap_t ultra_T8C3={0,0,0,0};
Ultra_cap_t ultra_T8C4={0,0,0,0};
 float UltraF1=0;
 float UltraF2=0;
 float UltraB1=0;
 float UltraB2=0;


void TIM5_IRQHandler(void)
{
//通道1捕获程序
	if ((ultra_T5C1.CAPTURE_STA & 0X80) == 0) 		                                 //还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)                             //溢出
		{	     
			if(ultra_T5C1.CAPTURE_STA&0X40)                                            //已经捕获到高电平了
			{
				if((ultra_T5C1.CAPTURE_STA&0X02)==0X02)                                  //高电平太长了
				{
					ultra_T5C1.CAPTURE_STA = 0;		                                         //捕获标志位清零
					TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);                    //设置为上升沿捕获		
					ultra_T5C1.CAPTURE_VAL = 0XFFFFFFFF;
				}
				else ultra_T5C1.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) 		                         //捕获1发生捕获事件
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); 		                               //清除中断标志位
			if (ultra_T5C1.CAPTURE_STA & 0X40)		                                     //捕获到一个下降沿
			{
				ultra_T5C1.CAPTURE_DOWNVAL = TIM_GetCapture1(TIM5);                      //记录下此时的定时器计数值
				ultra_T5C1.CAPTURE_VAL = ultra_T5C1.CAPTURE_DOWNVAL - ultra_T5C1.CAPTURE_UPVAL + (ultra_T5C1.CAPTURE_STA&0X3F)*0XFFFF;
				UltraF1 = ultra_T5C1.CAPTURE_VAL*340.0f/20000.0f;				
				ultra_T5C1.CAPTURE_STA = 0;		                                           //捕获标志位清零
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);                      //设置为上升沿捕获		  
			}
		else                                                                         //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				ultra_T5C1.CAPTURE_UPVAL = TIM_GetCapture1(TIM5);		                     //获取上升沿数据
				ultra_T5C1.CAPTURE_STA |= 0X40;		                                       //标记已捕获到上升沿
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);                     //设置为下降沿捕获
			}
		}
	}
//通道2捕获程序
	if ((ultra_T5C2.CAPTURE_STA & 0X80) == 0) 		                                 //还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)//溢出
		{	     
			if(ultra_T5C2.CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((ultra_T5C2.CAPTURE_STA&0X02)==0X02)//高电平太长了
				{
					ultra_T5C2.CAPTURE_STA = 0;		                                         //捕获标志位清零
					TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling);                   //设置为下降沿捕获
					ultra_T5C2.CAPTURE_VAL = 0XFFFFFFFF;
				}else ultra_T5C2.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET) 		                         //捕获1发生捕获事件
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC2); 		                               //清除中断标志位
			if (ultra_T5C2.CAPTURE_STA & 0X40)		                                     //捕获到一个下降沿
			{
				ultra_T5C2.CAPTURE_DOWNVAL = TIM_GetCapture2(TIM5);                      //记录下此时的定时器计数值
				ultra_T5C2.CAPTURE_VAL = ultra_T5C2.CAPTURE_DOWNVAL - ultra_T5C2.CAPTURE_UPVAL + (ultra_T5C2.CAPTURE_STA&0X3F)*0XFFFF;
				UltraB1 = ultra_T5C2.CAPTURE_VAL*340.0f/20000.0f;	
				ultra_T5C2.CAPTURE_STA = 0;		                                           //捕获标志位清零
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Rising);                      //设置为上升沿捕获		  
			}
			else                                                                       //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				ultra_T5C2.CAPTURE_UPVAL = TIM_GetCapture2(TIM5);		                     //获取上升沿数据
				ultra_T5C2.CAPTURE_STA |= 0X40;		                                       //标记已捕获到上升沿
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling);                     //设置为下降沿捕获
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 ); //清除中断标志位	
}

void TIM8_CC_IRQHandler(void)
{
//通道3捕获程序
	if ((ultra_T8C3.CAPTURE_STA & 0X80) == 0) 		                                 //还未成功捕获	
	{
		if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET)                             //溢出
		{	     
			if(ultra_T8C3.CAPTURE_STA&0X40)                                            //已经捕获到高电平了
			{
				if((ultra_T8C3.CAPTURE_STA&0X02)==0X02)                                  //高电平太长了
				{
					ultra_T8C3.CAPTURE_STA = 0;		                                         //捕获标志位清零
					TIM_OC3PolarityConfig(TIM8, TIM_ICPolarity_Rising);                    //设置为上升沿捕获		
					ultra_T8C3.CAPTURE_VAL = 0XFFFFFFFF;
				}
				else ultra_T8C3.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET) 		                         //捕获1发生捕获事件
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC3); 		                               //清除中断标志位
			if (ultra_T8C3.CAPTURE_STA & 0X40)		                                     //捕获到一个下降沿
			{
				ultra_T8C3.CAPTURE_DOWNVAL = TIM_GetCapture3(TIM8);                      //记录下此时的定时器计数值
				ultra_T8C3.CAPTURE_VAL = ultra_T8C3.CAPTURE_DOWNVAL - ultra_T8C3.CAPTURE_UPVAL + (ultra_T8C3.CAPTURE_STA&0X3F)*0XFFFF;
				UltraF2 = ultra_T8C3.CAPTURE_VAL*340.0f/40000.0f;				
				ultra_T8C3.CAPTURE_STA = 0;		                                           //捕获标志位清零
				TIM_OC3PolarityConfig(TIM8, TIM_ICPolarity_Rising);                      //设置为上升沿捕获		  
			}
		else                                                                         //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				ultra_T8C3.CAPTURE_UPVAL = TIM_GetCapture3(TIM8);		                     //获取上升沿数据
				ultra_T8C3.CAPTURE_STA |= 0X40;		                                       //标记已捕获到上升沿
				TIM_OC3PolarityConfig(TIM8, TIM_ICPolarity_Falling);                     //设置为下降沿捕获
			}
		}
	}
//通道4捕获程序
	if ((ultra_T8C4.CAPTURE_STA & 0X80) == 0) 		                                 //还未成功捕获	
	{
		if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET)//溢出
		{	     
			if(ultra_T8C4.CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((ultra_T8C4.CAPTURE_STA&0X02)==0X02)//高电平太长了
				{
					ultra_T8C4.CAPTURE_STA = 0;		                                         //捕获标志位清零
					TIM_OC4PolarityConfig(TIM8, TIM_ICPolarity_Falling);                   //设置为下降沿捕获
					ultra_T8C4.CAPTURE_VAL = 0XFFFFFFFF;
				}else ultra_T8C4.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET) 		                         //捕获1发生捕获事件
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC4); 		                               //清除中断标志位
			if (ultra_T8C4.CAPTURE_STA & 0X40)		                                     //捕获到一个下降沿
			{
				ultra_T8C4.CAPTURE_DOWNVAL = TIM_GetCapture4(TIM8);                      //记录下此时的定时器计数值
				ultra_T8C4.CAPTURE_VAL = ultra_T8C4.CAPTURE_DOWNVAL - ultra_T8C4.CAPTURE_UPVAL + (ultra_T8C4.CAPTURE_STA&0X3F)*0XFFFF;
				UltraB2 = ultra_T8C4.CAPTURE_VAL*340.0f/40000.0f;	
				ultra_T8C4.CAPTURE_STA = 0;		                                           //捕获标志位清零
				TIM_OC4PolarityConfig(TIM8, TIM_ICPolarity_Rising);                      //设置为上升沿捕获		  
			}
			else                                                                       //发生捕获时间但不是下降沿，第一次捕获到上升沿，记录此时的定时器计数值
			{
				ultra_T8C4.CAPTURE_UPVAL = TIM_GetCapture4(TIM8);		                     //获取上升沿数据
				ultra_T8C4.CAPTURE_STA |= 0X40;		                                       //标记已捕获到上升沿
				TIM_OC4PolarityConfig(TIM8, TIM_ICPolarity_Falling);                     //设置为下降沿捕获
			}
		}
	}
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update | TIM_IT_CC3 | TIM_IT_CC4 ); //清除中断标志位	
}

///*********************************************END OF FILE**********************/
