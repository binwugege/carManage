#include "./car/car.h"

void CAR_GPIO_Config(void){
	GPIO_InitTypeDef car_gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //Ê¹ÄÜGPIOGÊ±ÖÓ
	
	car_gpio.GPIO_Mode = GPIO_Mode_IN;
	car_gpio.GPIO_OType = GPIO_OType_PP;
	car_gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	car_gpio.GPIO_Speed = GPIO_Speed_50MHz;
	car_gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	
	GPIO_Init(GPIOB,&car_gpio);
}

