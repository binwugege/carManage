#include "./timer/timer.h"

extern vu16 USART2_RX_STA;
extern int carInitFlag;

volatile int openTimer = 1,RopenTimer = 1;

void TIM3_IRQHandler( void ){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
		openTimer--;
		if( !openTimer ){
			openTimer = RopenTimer;
			carInitFlag = 0;           // �����������
			TIM_SetCompare1(TIM2,175); // �ر�������
			TIM_Cmd(TIM3, DISABLE);    // �ر�TIM3
		}
		TIM_SetCounter(TIM3,0);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  // ���TIM3�����жϱ�־                          
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	// TIM2ʱ��ʹ��  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // ʹ��PORTAʱ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); // GPIOA0����Ϊ��ʱ��2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        // ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	// �ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      // ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // ����
	GPIO_Init(GPIOA,&GPIO_InitStructure);               // ��ʼ��PA0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	//��ʼ��TIM2 Channel PWMģʽ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ�ʹ�����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //������ԣ�TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_Pulse = 0;//�Ƚϳ�ʼֵ
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  // ʹ��TIM2��CCR1�ǵ�Ԥװ�ؼĴ���
  TIM_ARRPreloadConfig(TIM2,ENABLE);                 // ARPEʹ��
	
	TIM_Cmd(TIM2, ENABLE); 			  
}  


