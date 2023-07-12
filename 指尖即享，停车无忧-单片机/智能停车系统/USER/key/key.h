#ifndef _KEY_H
#define _KEY_H

#include "stm32f4xx.h"

#define INIT_EXIT_GPIO			    GPIOA
#define INIT_EXIT_GPIO_PIN		  GPIO_Pin_1
#define INIT_EXIT_RCCPERIPH		  RCC_AHB1Periph_GPIOA
#define INIT_EXTI_LINE			    EXTI_Line1
#define INIT_EXIT_IT_IRQn		    EXTI1_IRQn
#define INIT_EXIT_IT_HANDLER		EXTI1_IRQHandler
#define INIT_EXTI_PORT_SOURCE	  EXTI_PortSourceGPIOA
#define INIT_EXTI_PIN_SOURSE		EXTI_PinSource1


void GPIO_IT_Config(void);
#endif /* KEY */
