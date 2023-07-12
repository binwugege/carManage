#ifndef _ESP8266_H
#define _ESP8266_H

#include "stm32f4xx.h"
#include "delay.h"


#define USART_REC_LEN  			200  	//定义最大接收字节数 200

extern vu16 USART2_RX_STA;
void uart2_init(u32 bound);

void ESP_Init(char *WIFIname,char *WIFIpassword,char *userName,char *passWord,char *mqqtUrl,char *clientId);
void ESP_subTopic(char *key,char *value);
void ESP_Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch );
void SendPicDataInit( char *WIFIname,char *WIFIpassword );

#endif /* ESP8266 */

