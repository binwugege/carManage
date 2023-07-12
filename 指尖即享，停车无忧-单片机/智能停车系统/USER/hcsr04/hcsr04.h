#ifndef _HCSR04_H
#define _HCSR04_H

#include "stm32f4xx.h"

#define	Trig_Send_Open GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define	Trig_Send_Close GPIO_ResetBits(GPIOB, GPIO_Pin_1) 
#define Echo_Receive GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)


void HC_SR04Init(void);
float Get_Length(void);
void TIM4_Open(void);
void TIM4_Close(void);
int Get_Echo_TIM(void);

#endif /* HCSR04 */
