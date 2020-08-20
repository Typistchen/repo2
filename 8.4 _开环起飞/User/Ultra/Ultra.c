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

void Delay_ULTRA(unsigned char n)      //������
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
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	                         //ʹ��TIM5,3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);                          //ʹ��GPIOA,Bʱ��

	TIM_DeInit(TIM5);	                                                             //���³�ʼ��TIM5ΪĬ��״̬	

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);                          //PA0 ����ΪTIM5 CH1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); 	                       //PA1 ����ΪTIM5 CH2
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                   //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                           //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                 //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;                                 //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //��ʼ��PA0,1,6,7  

	//��ʼ��TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr;                                        //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	                                   //Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                        //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                    //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);                                //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM5��ʱ�������λ

	//��ʼ��TIM5���벶����� ͨ��1
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;                              //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	//��ʼ��TIM5���벶����� ͨ��2
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2;                              //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                //�ж����ȼ�����2
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;                                //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                      //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                             //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                                                //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 ,ENABLE);  					 //��������жϣ�����CC1IE,CC2IE�����ж�

	TIM_Cmd(TIM5, ENABLE); 		                                                     //ʹ�ܶ�ʱ��2
}


void  Ultra_TIM8_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM8_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);	                         //ʹ��TIM8ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);                          //ʹ��GPIOCʱ��

	TIM_DeInit(TIM8);	                                                             //���³�ʼ��TIM8ΪĬ��״̬	

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);                          //PC8 ����ΪTIM8 CH3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8); 	                       //PC9 ����ΪTIM8 CH4
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                   //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	                           //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                 //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;                                 //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);                                         //��ʼ��PC8,9  

	//��ʼ��TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr;                                        //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	                                   //Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                        //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                    //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);                                //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIM5��ʱ�������λ

	//��ʼ��TIM8���벶����� ͨ��3
	TIM8_ICInitStructure.TIM_Channel = TIM_Channel_3;                              //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //�����ز���
	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //ӳ�䵽TI1��
	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //���������Ƶ,����Ƶ 
	TIM8_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM8, &TIM8_ICInitStructure);

	//��ʼ��TIM8���벶����� ͨ��4
	TIM8_ICInitStructure.TIM_Channel = TIM_Channel_4;                              //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	                 //�����ز���
	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;               //ӳ�䵽TI1��
	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //���������Ƶ,����Ƶ 
	TIM8_ICInitStructure.TIM_ICFilter = 0x01;	                                     //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM8, &TIM8_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                //�ж����ȼ�����2
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;                                //TIM8�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                      //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                             //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                                                //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM8,TIM_IT_Update | TIM_IT_CC3 | TIM_IT_CC4 ,ENABLE);  					 //��������жϣ�����CC1IE,CC2IE,CC3IE�����ж�

	TIM_Cmd(TIM8, ENABLE); 		                                                     //ʹ�ܶ�ʱ��2
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
//ͨ��1�������
	if ((ultra_T5C1.CAPTURE_STA & 0X80) == 0) 		                                 //��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)                             //���
		{	     
			if(ultra_T5C1.CAPTURE_STA&0X40)                                            //�Ѿ����񵽸ߵ�ƽ��
			{
				if((ultra_T5C1.CAPTURE_STA&0X02)==0X02)                                  //�ߵ�ƽ̫����
				{
					ultra_T5C1.CAPTURE_STA = 0;		                                         //�����־λ����
					TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);                    //����Ϊ�����ز���		
					ultra_T5C1.CAPTURE_VAL = 0XFFFFFFFF;
				}
				else ultra_T5C1.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) 		                         //����1���������¼�
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); 		                               //����жϱ�־λ
			if (ultra_T5C1.CAPTURE_STA & 0X40)		                                     //����һ���½���
			{
				ultra_T5C1.CAPTURE_DOWNVAL = TIM_GetCapture1(TIM5);                      //��¼�´�ʱ�Ķ�ʱ������ֵ
				ultra_T5C1.CAPTURE_VAL = ultra_T5C1.CAPTURE_DOWNVAL - ultra_T5C1.CAPTURE_UPVAL + (ultra_T5C1.CAPTURE_STA&0X3F)*0XFFFF;
				UltraF1 = ultra_T5C1.CAPTURE_VAL*340.0f/20000.0f;				
				ultra_T5C1.CAPTURE_STA = 0;		                                           //�����־λ����
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);                      //����Ϊ�����ز���		  
			}
		else                                                                         //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				ultra_T5C1.CAPTURE_UPVAL = TIM_GetCapture1(TIM5);		                     //��ȡ����������
				ultra_T5C1.CAPTURE_STA |= 0X40;		                                       //����Ѳ���������
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);                     //����Ϊ�½��ز���
			}
		}
	}
//ͨ��2�������
	if ((ultra_T5C2.CAPTURE_STA & 0X80) == 0) 		                                 //��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)//���
		{	     
			if(ultra_T5C2.CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((ultra_T5C2.CAPTURE_STA&0X02)==0X02)//�ߵ�ƽ̫����
				{
					ultra_T5C2.CAPTURE_STA = 0;		                                         //�����־λ����
					TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling);                   //����Ϊ�½��ز���
					ultra_T5C2.CAPTURE_VAL = 0XFFFFFFFF;
				}else ultra_T5C2.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET) 		                         //����1���������¼�
		{
			TIM_ClearITPendingBit(TIM5, TIM_IT_CC2); 		                               //����жϱ�־λ
			if (ultra_T5C2.CAPTURE_STA & 0X40)		                                     //����һ���½���
			{
				ultra_T5C2.CAPTURE_DOWNVAL = TIM_GetCapture2(TIM5);                      //��¼�´�ʱ�Ķ�ʱ������ֵ
				ultra_T5C2.CAPTURE_VAL = ultra_T5C2.CAPTURE_DOWNVAL - ultra_T5C2.CAPTURE_UPVAL + (ultra_T5C2.CAPTURE_STA&0X3F)*0XFFFF;
				UltraB1 = ultra_T5C2.CAPTURE_VAL*340.0f/20000.0f;	
				ultra_T5C2.CAPTURE_STA = 0;		                                           //�����־λ����
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Rising);                      //����Ϊ�����ز���		  
			}
			else                                                                       //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				ultra_T5C2.CAPTURE_UPVAL = TIM_GetCapture2(TIM5);		                     //��ȡ����������
				ultra_T5C2.CAPTURE_STA |= 0X40;		                                       //����Ѳ���������
				TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling);                     //����Ϊ�½��ز���
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 ); //����жϱ�־λ	
}

void TIM8_CC_IRQHandler(void)
{
//ͨ��3�������
	if ((ultra_T8C3.CAPTURE_STA & 0X80) == 0) 		                                 //��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET)                             //���
		{	     
			if(ultra_T8C3.CAPTURE_STA&0X40)                                            //�Ѿ����񵽸ߵ�ƽ��
			{
				if((ultra_T8C3.CAPTURE_STA&0X02)==0X02)                                  //�ߵ�ƽ̫����
				{
					ultra_T8C3.CAPTURE_STA = 0;		                                         //�����־λ����
					TIM_OC3PolarityConfig(TIM8, TIM_ICPolarity_Rising);                    //����Ϊ�����ز���		
					ultra_T8C3.CAPTURE_VAL = 0XFFFFFFFF;
				}
				else ultra_T8C3.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET) 		                         //����1���������¼�
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC3); 		                               //����жϱ�־λ
			if (ultra_T8C3.CAPTURE_STA & 0X40)		                                     //����һ���½���
			{
				ultra_T8C3.CAPTURE_DOWNVAL = TIM_GetCapture3(TIM8);                      //��¼�´�ʱ�Ķ�ʱ������ֵ
				ultra_T8C3.CAPTURE_VAL = ultra_T8C3.CAPTURE_DOWNVAL - ultra_T8C3.CAPTURE_UPVAL + (ultra_T8C3.CAPTURE_STA&0X3F)*0XFFFF;
				UltraF2 = ultra_T8C3.CAPTURE_VAL*340.0f/40000.0f;				
				ultra_T8C3.CAPTURE_STA = 0;		                                           //�����־λ����
				TIM_OC3PolarityConfig(TIM8, TIM_ICPolarity_Rising);                      //����Ϊ�����ز���		  
			}
		else                                                                         //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				ultra_T8C3.CAPTURE_UPVAL = TIM_GetCapture3(TIM8);		                     //��ȡ����������
				ultra_T8C3.CAPTURE_STA |= 0X40;		                                       //����Ѳ���������
				TIM_OC3PolarityConfig(TIM8, TIM_ICPolarity_Falling);                     //����Ϊ�½��ز���
			}
		}
	}
//ͨ��4�������
	if ((ultra_T8C4.CAPTURE_STA & 0X80) == 0) 		                                 //��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET)//���
		{	     
			if(ultra_T8C4.CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((ultra_T8C4.CAPTURE_STA&0X02)==0X02)//�ߵ�ƽ̫����
				{
					ultra_T8C4.CAPTURE_STA = 0;		                                         //�����־λ����
					TIM_OC4PolarityConfig(TIM8, TIM_ICPolarity_Falling);                   //����Ϊ�½��ز���
					ultra_T8C4.CAPTURE_VAL = 0XFFFFFFFF;
				}else ultra_T8C4.CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET) 		                         //����1���������¼�
		{
			TIM_ClearITPendingBit(TIM8, TIM_IT_CC4); 		                               //����жϱ�־λ
			if (ultra_T8C4.CAPTURE_STA & 0X40)		                                     //����һ���½���
			{
				ultra_T8C4.CAPTURE_DOWNVAL = TIM_GetCapture4(TIM8);                      //��¼�´�ʱ�Ķ�ʱ������ֵ
				ultra_T8C4.CAPTURE_VAL = ultra_T8C4.CAPTURE_DOWNVAL - ultra_T8C4.CAPTURE_UPVAL + (ultra_T8C4.CAPTURE_STA&0X3F)*0XFFFF;
				UltraB2 = ultra_T8C4.CAPTURE_VAL*340.0f/40000.0f;	
				ultra_T8C4.CAPTURE_STA = 0;		                                           //�����־λ����
				TIM_OC4PolarityConfig(TIM8, TIM_ICPolarity_Rising);                      //����Ϊ�����ز���		  
			}
			else                                                                       //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				ultra_T8C4.CAPTURE_UPVAL = TIM_GetCapture4(TIM8);		                     //��ȡ����������
				ultra_T8C4.CAPTURE_STA |= 0X40;		                                       //����Ѳ���������
				TIM_OC4PolarityConfig(TIM8, TIM_ICPolarity_Falling);                     //����Ϊ�½��ز���
			}
		}
	}
	TIM_ClearITPendingBit(TIM8, TIM_IT_Update | TIM_IT_CC3 | TIM_IT_CC4 ); //����жϱ�־λ	
}

///*********************************************END OF FILE**********************/
