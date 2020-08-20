#include "motor.h"  
#include "GPIO.h"
#include "Mymath.h"
#include "bsp_SysTick.h"
#define Motor_PIN                  	GPIO_Pin_11                 
#define Motor_GPIO_PORT            	GPIOA                      
#define Motor_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define Motor_TIM                  	TIM1  
#define	Low_vol										 	15
#define	How_vol											30
/******************************************************************************************************************
*                         void TIM1_Motor_PWM_Init(u16 arr,u16 psc)	
*Description : TIM1���PWM_dma��ʼ�� 
*Arguments   : arr���Զ���װֵ psc��ʱ��Ԥ��
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
uint16_t data[18];
uint16_t Send_data[16];
void TIM1_Motor_PWM_Init()										 									//PWM��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);					//ʹ�� PORTB ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);					//ʹ�� TIM3 ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);					//ʹ�� DMA ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 										//PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 									//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 								//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);			//���� PA11Ϊ��ʱ��1����
	
	TIM_TimeBaseStructure.TIM_Period = (40 - 1); 									//800kHz   ʱ��168M
	TIM_TimeBaseStructure.TIM_Prescaler = 7-1;										//��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 						//ʹ��Ԥװ��
	TIM_DMACmd(TIM1, TIM_DMA_CC4, ENABLE);												//ʹ�� TIM1 CC2 DMA

	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1,ENABLE);														//�����ڶ�ʱ������ʱ��ARR�Ļ�������д����ֵ���Ա��ڸ����¼�����ʱ���븲����ǰ��ֵ
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	TIM_Cmd(TIM1, ENABLE);
	
	DMA_DeInit(DMA2_Stream4);
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;	
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&TIM1->CCR4);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;				//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = 18;	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  	
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream4, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream4, DMA_IT_TC, ENABLE);
}
/******************************************************************************************************************
*                         motor_send(uint16_t *motor)	
*Description : DMA���ͺ���
*Arguments   : motor���͵�����
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
void motor_send(uint16_t *motor)
{
	uint16_t memaddr;
	uint16_t buffersize;
	int i = 0 ;
	buffersize = 18;        																		// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes
	memaddr = 0;   																							// reset buffer memory index
	for(i = 0; i < 16 ; i++)
	data[i] = motor[i];
	data[16] = 0;
	data[17] = 0;
	DMA_SetCurrDataCounter(DMA2_Stream4, buffersize);         	// load number of bytes to be transferred
	DMA_Cmd(DMA2_Stream4, ENABLE);                         			// enable DMA channel 6
	TIM_Cmd(TIM1, ENABLE);                                      // enable Timer 1
	while(!DMA_GetFlagStatus(DMA2_Stream4,DMA_FLAG_TCIF4)) ;    // wait until transfer complete
	TIM_Cmd(TIM1, DISABLE);         														// disable Timer1
	DMA_Cmd(DMA2_Stream4, DISABLE);                         		// disable DMA channel 6
	DMA_ClearFlag(DMA2_Stream4,DMA_FLAG_TCIF4);                 // clear DMA2 Channel 6 transfer complete flag
}
/******************************************************************************************************************
*                        u16 add_checksum_and_telemetry(u16 packet, u8 telem)
*Description : У��λ����
*Arguments   : packet����ֵ 			telem�ش�λ
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
u16 add_checksum_and_telemetry(u16 packet, u8 telem) {
    u16 packet_telemetry = (packet << 1) | (telem & 1);
    u8 i;
    u16 csum = 0;
    u16 csum_data = packet_telemetry;

    for (i = 0; i < 3; i++) {
        csum ^=  csum_data;   																// xor data by nibbles
        csum_data >>= 4;
    }
    csum &= 0xf;
    return (packet_telemetry << 4) | csum;    								//append checksum
}
/******************************************************************************************************************
*                       pwmWriteDigital(uint16_t *esc_cmd, int value)
*Description : ����16λ����
*Arguments   : esc_cmd���ɵ�λ			value����ֵ
*Returns     : none
*Notes       : none
*******************************************************************************************************************
*/
void pwmWriteDigital(uint16_t *esc_cmd, int value)
{
    value = ( (value > 2047) ? 2047 : value );
    value = add_checksum_and_telemetry(value, 0);
    esc_cmd[0]  = (value & 0x8000) ? How_vol :  Low_vol ;
    esc_cmd[1]  = (value & 0x4000) ? How_vol :  Low_vol ;
    esc_cmd[2]  = (value & 0x2000) ? How_vol :  Low_vol ;
    esc_cmd[3]  = (value & 0x1000) ? How_vol :  Low_vol ;
    esc_cmd[4]  = (value & 0x0800) ? How_vol :  Low_vol ;
    esc_cmd[5]  = (value & 0x0400) ? How_vol :  Low_vol ;
    esc_cmd[6]  = (value & 0x0200) ? How_vol :  Low_vol ;
    esc_cmd[7]  = (value & 0x0100) ? How_vol :  Low_vol ;
    esc_cmd[8]  = (value & 0x0080) ? How_vol :  Low_vol ;
    esc_cmd[9]  = (value & 0x0040) ? How_vol :  Low_vol ;
    esc_cmd[10] = (value & 0x0020) ? How_vol :  Low_vol ;
    esc_cmd[11] = (value & 0x0010) ? How_vol :  Low_vol ;   
    esc_cmd[12] = (value & 0x8) ? How_vol : Low_vol;
    esc_cmd[13] = (value & 0x4) ? How_vol : Low_vol;
    esc_cmd[14] = (value & 0x2) ? How_vol : Low_vol;
    esc_cmd[Low_vol] = (value & 0x1) ? How_vol : Low_vol;
}
