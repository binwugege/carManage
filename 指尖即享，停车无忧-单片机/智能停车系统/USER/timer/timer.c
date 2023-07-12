#include "./timer/timer.h"

extern vu16 USART2_RX_STA;
extern int carInitFlag;

volatile int openTimer = 1,RopenTimer = 1;

void TIM3_IRQHandler( void ){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
		openTimer--;
		if( !openTimer ){
			openTimer = RopenTimer;
			carInitFlag = 0;           // 开启车辆检测
			TIM_SetCompare1(TIM2,175); // 关闭升降杆
			TIM_Cmd(TIM3, DISABLE);    // 关闭TIM3
		}
		TIM_SetCounter(TIM3,0);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  // 清除TIM3更新中断标志                          
	}
}

void TIM3_Init(u16 arr,u16 psc){
	NVIC_InitTypeDef nvic_initTypeDef;
	TIM_TimeBaseInitTypeDef TIM_timebaseinitTypeDef;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_timebaseinitTypeDef.TIM_Period = arr;
	TIM_timebaseinitTypeDef.TIM_ClockDivision = psc;
	TIM_timebaseinitTypeDef.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_timebaseinitTypeDef.TIM_CounterMode  = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_timebaseinitTypeDef); 
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig( TIM3,TIM_IT_Update,ENABLE ); 
	TIM_Cmd(TIM3,DISABLE);
	
	nvic_initTypeDef.NVIC_IRQChannel = TIM3_IRQn;
	nvic_initTypeDef.NVIC_IRQChannelPreemptionPriority = 3;
	nvic_initTypeDef.NVIC_IRQChannelSubPriority = 3;
	nvic_initTypeDef.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_initTypeDef);
};

void TIM2_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	// TIM2时钟使能  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能PORTA时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); // GPIOA0复用为定时器2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // 复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	// 速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);               // 初始化PA0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化定时器2
	
	//初始化TIM2 Channel PWM模式 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式：TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较使能输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性：TIM输出比较极性低
	TIM_OCInitStructure.TIM_Pulse = 0;//比较初始值
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  // 使能TIM2在CCR1是的预装载寄存器
  TIM_ARRPreloadConfig(TIM2,ENABLE);                 // ARPE使能
	
	TIM_Cmd(TIM2, ENABLE); 			  
}  


