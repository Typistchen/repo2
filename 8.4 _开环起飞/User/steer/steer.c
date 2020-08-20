#include "steer.h" 
#include "GPIO.h"
#include "Mymath.h"

#define Steer1_PIN                  GPIO_Pin_6                 
#define Steer1_GPIO_PORT            GPIOC                  
#define Steer1_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define Steer2_PIN                  GPIO_Pin_7                 
#define Steer2_GPIO_PORT            GPIOC                    
#define Steer2_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define Steer3_PIN                  GPIO_Pin_8                
#define Steer3_GPIO_PORT            GPIOC                    
#define Steer3_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define Steer4_PIN                  GPIO_Pin_9                 
#define Steer4_GPIO_PORT            GPIOC                   
#define Steer4_GPIO_CLK             RCC_AHB1Periph_GPIOC 

#define Steer1_TIM                  TIM3    

int start_angle[4] = {1600 , 1600 , 1600 , 1600};


/******************************************************************************************************************
*                       void TIM3_Steer1_2_PWM_Init(u32 arr,u16 psc)
*
*Description : TIM3舵机1 2PWM初始化 
*Arguments   : arr：自动重装值 psc：时钟预分
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/


void TIM3_Steer1_2_PWM_Init(u32 arr,u16 psc)                                    
{		 					 
	GPIO_InitTypeDef          GPIO_InitConfig;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseConfig;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_PinAFConfig(Steer1_GPIO_PORT,GPIO_PinSource6,GPIO_AF_TIM3);
  GPIO_PinAFConfig(Steer2_GPIO_PORT,GPIO_PinSource7,GPIO_AF_TIM3);	
	GPIO_PinAFConfig(Steer3_GPIO_PORT,GPIO_PinSource8,GPIO_AF_TIM3);
  GPIO_PinAFConfig(Steer4_GPIO_PORT,GPIO_PinSource9,GPIO_AF_TIM3);	
	
	GPIO_InitConfig.GPIO_Pin=Steer1_PIN | Steer2_PIN | Steer3_PIN| Steer4_PIN;
	GPIO_InitConfig.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitConfig.GPIO_OType=GPIO_OType_PP;
	GPIO_InitConfig.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitConfig.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(Steer1_GPIO_PORT,&GPIO_InitConfig);
	GPIO_Init(Steer2_GPIO_PORT,&GPIO_InitConfig);
	
	TIM_TimeBaseConfig.TIM_Period=arr;
	TIM_TimeBaseConfig.TIM_Prescaler=psc;
	TIM_TimeBaseConfig.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseConfig.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(Steer1_TIM,&TIM_TimeBaseConfig);                      
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	
  TIM_OCInitStructure.TIM_Pulse= 0;
	
	TIM_OC1Init(Steer1_TIM, &TIM_OCInitStructure); 
	TIM_OC2Init(Steer1_TIM, &TIM_OCInitStructure); 
	TIM_OC3Init(Steer1_TIM, &TIM_OCInitStructure); 
	TIM_OC4Init(Steer1_TIM, &TIM_OCInitStructure); 

	/************使能预装载寄存器****************/
	TIM_OC1PreloadConfig(Steer1_TIM,TIM_OCPreload_Enable);	
  TIM_OC2PreloadConfig(Steer1_TIM,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(Steer1_TIM,TIM_OCPreload_Enable);	
  TIM_OC4PreloadConfig(Steer1_TIM,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(Steer1_TIM,ENABLE);                                                    //允许在定时器工作时向ARR的缓冲器中写入新值，以便在更新事件发生时载入覆盖以前的值
	
	TIM_CtrlPWMOutputs(Steer1_TIM,ENABLE);                                                      /************************/
	TIM_Cmd(Steer1_TIM,ENABLE);
}
int speed0,speed1,speed2,speed3;
void Steer_Set_angle(u8 Steerx,int *angle)
{
	
	

	speed0 = angle[0] * 9 + 1600;
	speed1 = angle[1] * 9 + 1600;
	speed2 = angle[2] * 9 + 1600;
	speed3 = angle[3] * 9 + 1600;
	
  switch(Steerx)
		{
			case 0: speed0 = LIMIT( speed0,700,2500 ) ;  //0号舵机
				      TIM_SetCompare1(TIM3,speed0); 
			        break; 			
			case 1: speed1 = LIMIT( speed1,700,2500 );  //1号舵机
				      TIM_SetCompare2(TIM3,speed1); 
			        break;  			
			case 2: speed2 = LIMIT( speed2,700,2500 );  //2号舵机
				      TIM_SetCompare3(TIM3,speed2); 
			        break;                          
			case 3: speed3 = LIMIT( speed3,700,2500 );  //3号舵机
				      TIM_SetCompare4(TIM3,speed3); 
			        break;                      
			default:break;
		}
}

void Steer_Mode()
{
  Steer_Set_angle(0,start_angle);
	Steer_Set_angle(0,start_angle);
	Steer_Set_angle(0,start_angle);
	Steer_Set_angle(0,start_angle);
}

void Steer_delay_ms(int n)

{
{
	unsigned int i,j;
	for(j = 0;j < n;j++)
	{
		for(i=0;i<1000;i++);
	}
}
	
}
	

