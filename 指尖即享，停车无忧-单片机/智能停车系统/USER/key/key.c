#include "./key/key.h"
#include "./esp8266/esp8266.h"
#include "./oled/oled.h"

extern char WIFIname[];
extern char WIFIpassword[];

extern char userName[];
extern char password[];
extern char mqttUrl[];
extern char clientId[];

void GPIO_IT_Config(void){
	EXTI_InitTypeDef EXTI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//开启相关时钟
	RCC_AHB1PeriphClockCmd(INIT_EXIT_RCCPERIPH, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //enable SYSCFG
	
	//GPIO设置-下拉输入
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin 	  = INIT_EXIT_GPIO_PIN;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_25MHz;//大于等于50最好开启IO补偿
	GPIO_Init(INIT_EXIT_GPIO, &GPIO_InitStruct);
	
	//EXIT设置-中断模式上升沿触发
	EXTI_InitStruct.EXTI_Line 	 = INIT_EXTI_LINE;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode 	 = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	
	//NVIC设置-中断配置
	NVIC_InitStruct.NVIC_IRQChannel = INIT_EXIT_IT_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//最高 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//中断源配置-PA1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
}


void MPU_EXIT_IT_HANDLER(){
	if(EXTI_GetITStatus(INIT_EXTI_LINE) != RESET)
	{
		OLED_ShowChLength(32,40,14,3);
		ESP_Init(WIFIname,WIFIpassword,userName,password,mqttUrl,clientId);
		EXTI_ClearITPendingBit(INIT_EXTI_LINE);
	}
}


