#include "./hcsr04/hcsr04.h"

#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"

volatile uint16_t mscount = 0;
int flag = 0;
int TIMcount = 3000;

void HC_SR04Init(void)
{
		GPIO_InitTypeDef GPIOinitStructure;
		TIM_TimeBaseInitTypeDef TIMInitStructure;
		NVIC_InitTypeDef NVICinitStructure;
	
		//1.使能GPIO时钟、计时器时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		//2.配置GPIO
	  GPIOinitStructure.GPIO_Pin   = GPIO_Pin_1;
		GPIOinitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	  GPIOinitStructure.GPIO_OType = GPIO_OType_PP;
		GPIOinitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIOinitStructure);
	  GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	
	  GPIOinitStructure.GPIO_Pin   = GPIO_Pin_2;
		GPIOinitStructure.GPIO_Mode  = GPIO_Mode_IN;
		GPIOinitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init(GPIOB, &GPIOinitStructure);
		 
		
		//3.配置TIM
		TIMInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIMInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
		TIMInitStructure.TIM_Period        = TIMcount-1;
		TIMInitStructure.TIM_Prescaler     = 84-1;
		
		TIM_TimeBaseInit(TIM4, &TIMInitStructure);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM4, DISABLE);
		
		//配置中断优先级
		NVICinitStructure.NVIC_IRQChannel                   = TIM4_IRQn;
		NVICinitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVICinitStructure.NVIC_IRQChannelSubPriority        = 1;
		NVICinitStructure.NVIC_IRQChannelCmd                = ENABLE;
		NVIC_Init(&NVICinitStructure);
		
		printf("初始化成功\r\n");
		
}

void TIM4_Open(void)
{
		TIM_SetCounter(TIM4, 0);
	  mscount = 0;
		TIM_Cmd(TIM4, ENABLE);
}

void TIM4_Close(void)
{
		TIM_Cmd(TIM4, DISABLE);
}

void TIM4_IRQHandler(void)
{
		if( TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET )
		{
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			  flag = 1;
		}
}

int Get_Echo_TIM(void)
{
		uint32_t t = 0;
		t += TIM_GetCounter(TIM4);
		TIM4->CNT = 0;
		delay_us(50);
		
		return t;
}

float Get_Length(void)
{
		int i = 0;
		uint32_t t = 0;
		float sum = 0;
		float length = 0;
	
		while(i < 5){
			
			PBout(1) = 1;
			delay_us(20);	
			PBout(1) = 0;
			
			while( PBin(2) == 0 && flag == 0);
			TIM4_Open();
			while( PBin(2) == 1 && flag == 0);
			TIM4_Close();				
			if(flag)flag = 0;			
			t = Get_Echo_TIM();	
			length = ((float) t / 58);
			sum += length;
			i++;
		}
    sum /= i;
		return sum;
}

