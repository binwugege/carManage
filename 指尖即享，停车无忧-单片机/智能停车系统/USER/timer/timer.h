#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f4xx.h"

void TIM3_Init(u16 arr,u16 psc);

void TIM2_PWM_Init(u32 arr,u32 psc);


#endif /* TIMER */

