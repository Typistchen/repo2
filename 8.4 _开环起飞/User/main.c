#include "stm32f4xx.h"
#include "bsp_SysTick.h"
#include "GPIO.h"
#include "bsp_led.h" 
#include "bsp_basic_tim.h"
#include "beep.h" 
#include "motor.h"
#include "mpu6050.h"
#include "bsp_spi.h" 
#include "Data_type.h" 
#include "Ultra.h"
#include "IMU.h"
#include "MYMATH.h"
#include "steer.h" 
#include "usart2_dma.h" 
#include "usart3_dma.h"
#include "usart1_dma.h"

extern  uint16_t Send_data[16];
short GyroRaw_Z = 0;
extern int sbus_adu[7];
extern uint16_t Raw_code[16];
int main(void)
{
	SysTick_Init();
	BEEP_Init();
	LED_Init();
	TIM1_Motor_PWM_Init(2000-1,168-1);
	TIM3_Steer1_2_PWM_Init(20000-1,84-1);
	MPU_Init();
	USART2_Init(100000);
	USART3_Init(115200);
	USART1_Init(115200);
	TIM7_Init(1000,84-1);
	
	while(1)
	{
//		sbus_re_calculate(sbus_adu);
//		Steer_Set_angle(0,sbus_adu);
//		Steer_Set_angle(1,sbus_adu);
//		Steer_Set_angle(2,sbus_adu);
//		Steer_Set_angle(3,sbus_adu);
		
//		 Steer_Mode();
		sbus_re_calculate(sbus_adu);
		pwmWriteDigital(Send_data,sbus_adu[3]);
		motor_send(Send_data);
	}
}

