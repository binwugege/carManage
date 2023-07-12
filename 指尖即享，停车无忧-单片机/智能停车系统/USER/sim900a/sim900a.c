#include "./sim900a/sim900a.h"

#include "delay.h"
#include "stdio.h"

unsigned char *phoneBuf[128];

/**
* @author: feifei
* @funtion: 向SIM900A发送一个字节
* @param: none 
* @notework: none
* @return: none
*/
void SIM900_Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	USART_SendData(pUSARTx,ch);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


/**
* @author: feifei
* @funtion: 向SIM900A发送一个字符串
* @param: none
* @notework: none
* @return: none
*/
void SIM900_Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
unsigned int k=0;
    do 
    {
        SIM900_Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	
}

void SendBookingData( unsigned char * phone,unsigned char * BookingData ){
	delay_ms(1000);
	SIM900_Usart_SendString(SIM900_USART,(uint8_t *)"AT+CSCS=\"GSM\"\r\n");
  delay_ms(500);
	SIM900_Usart_SendString(SIM900_USART,(uint8_t *)"AT+CMGF=1\r\n");
  delay_ms(500);
	SIM900_Usart_SendString(SIM900_USART,(uint8_t *)"AT+CSCA?\r\n");
  delay_ms(500);
	SIM900_Usart_SendString(SIM900_USART,(uint8_t *)"AT+CSMP=17,167,0,8\r\n");
	delay_ms(500);
	
	sprintf((char *)phoneBuf,"AT+CMGS=\"%s\"\r\n",phone);
	SIM900_Usart_SendString(SIM900_USART,(unsigned char *)phoneBuf);
 	
	delay_ms(500);
	SIM900_Usart_SendString(SIM900_USART,BookingData);
  delay_ms(1000);
	SIM900_Usart_SendByte(SIM900_USART,0x1a);
  delay_ms(500);
}



void uart3_init(u32 bound){

  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); // 使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);// 使能USART3时钟
 

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); // GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); // GPIOB11复用为USART3
	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; // GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	
  USART_Init(USART3, &USART_InitStructure); 
	
  USART_Cmd(USART3, ENABLE);  

}
