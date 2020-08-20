#ifndef __MOTOR_H
#define	__MOTOR_H

#include "stm32f4xx.h"

extern int motor_speed;
void TIM1_Motor_PWM_Init();
void motor_send(uint16_t *motor);
u16 add_checksum_and_telemetry(u16 packet, u8 telem);
void pwmWriteDigital(uint16_t *esc_cmd, int value);
#endif
