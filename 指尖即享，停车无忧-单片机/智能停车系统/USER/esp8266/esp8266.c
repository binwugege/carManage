/*
* @auto: �ɷɳ�˧��
* @funtion: �û��밢����������������������
* @param: none
* @name: feifei
*/


#include "./esp8266/esp8266.h"
#include "./timer/timer.h"
#include <string.h>
#include <stdio.h>


u8 USART2_RX_BUF[USART_REC_LEN]; 
vu16 USART2_RX_STA = 0;       

// �������ӵĻ�����
char MQTTUSERCFG[256],MQTTCLIENTID[256],MQTTCONN[256],CWJAP[256],SUBTOPIC[256],PUBTOPIC[256];

// �������ݵĻ�����
char DATA[32],PHONE[32];
char *dataBuf;

extern unsigned int carBookingStatus[4];

// TCP͸������
extern char TCPIP[];
extern char TCPPORT[];


// ԤԼ���ݴ���
char id[1],status[1],phone[11];


extern char subTopic[];
extern char pubTopic[];
extern int initEsp8266Success;

// �����ַ���
void ESP_SendString(USART_TypeDef *usart,uint8_t *str);
// ����ԤԼ
extern void handleCarBooking( void );

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART2);                     //(USART2->DR);��ȡ���յ�������
		if(USART2_RX_STA < 400){
			USART2_RX_BUF[USART2_RX_STA++] = Res;	
		}else{
			USART2_RX_STA = 1<< 15;
		}  		 
  }
	if( USART_GetITStatus(USART2, USART_IT_IDLE) != RESET ){
		
		// esp8266��ʼ���ɹ���ʼ����ԤԼ����
		if( initEsp8266Success ){
			dataBuf = strstr((const char *)USART2_RX_BUF,(const char *)"carData");
			if( dataBuf ){
				
				// �ռ�����
				strncpy(DATA,dataBuf,32);
				strncpy(id,DATA + 8,1);
				strncpy(status,DATA + 10,1);
				strncpy(phone,DATA + 18,11);
				
				// ������ȷ��Ӳ����ʼ����ԤԼ
				handleCarBooking();
			}else{
				// δ��¼ ������led��˸
			}
			
		}
		USART2_RX_BUF[USART2_RX_STA++] =  USART_ReceiveData(USART2);
		USART2_RX_STA = 0;
	}

}




void uart2_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10����ΪUSART2
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIO3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//��������ж�

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
	USART2_RX_STA = 0;		 // ����

}
void ESP_Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	USART_SendData(pUSARTx,ch);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void ESP_SendString(USART_TypeDef *usart,uint8_t *str){
	unsigned int k=0;
    do 
    {
        ESP_Usart_SendByte(usart, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}

void ESP_Init(char *WIFIname,char *WIFIpassword,char *userName,char *passWord,char *mqqtUrl,char *clientId){
	ESP_SendString(USART2,(uint8_t *)"AT+RST\r\n");
	delay_ms(1000);
	
	ESP_SendString(USART2,(uint8_t *)"AT+CWMODE=1\r\n");
	delay_ms(1000);
	
	sprintf(CWJAP,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFIname,WIFIpassword);	
	ESP_SendString(USART2,(uint8_t *)CWJAP);
	// ESP_SendString(USART2,(uint8_t *)"AT+CWJAP=\"feifei\",\"404340389\"\r\n");
	delay_ms(5000);
	
	ESP_SendString(USART2,(uint8_t *)"AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");
	delay_ms(1500);
	
  sprintf(MQTTUSERCFG,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n",userName,passWord);	
	// ESP_SendString(USART2,(uint8_t *)"AT+MQTTUSERCFG=0,1,\"NULL\",\"ESP8266&i4c2LeyEp9M\",\"cbb5962ccdd7006de0c339a746525ecaf03ab1b2fb32f6b3c6207c69b44df876\",0,0,\"\"\r\n");
	ESP_SendString(USART2,(uint8_t *)MQTTUSERCFG);
	delay_ms(1500);
	
	sprintf(MQTTCLIENTID,"AT+MQTTCLIENTID=0,\"%s\"\r\n",clientId);
	ESP_SendString(USART2,(uint8_t *)MQTTCLIENTID);
	// ESP_SendString(USART2,(uint8_t *)"AT+MQTTCLIENTID=0,\"i4c2LeyEp9M\\.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1672563580511|\"\r\n");
	delay_ms(1500);
	
	sprintf(MQTTCONN,"AT+MQTTCONN=0,\"%s\",1883,1\r\n",mqqtUrl);
	ESP_SendString(USART2,(uint8_t *)MQTTCONN);
	// ESP_SendString(USART2,(uint8_t *)"AT+MQTTCONN=0,\"iot-06z00a1zoz1yey7.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
	// delay_ms(1000);
	// ESP_SendString(USART2,(uint8_t *)"AT+MQTTSUB=0,\"/i4c2LeyEp9M/ESP8266/user/get\",1\r\n");
	// AT+MQTTPUB=0,"/i4c2LeyEp9M/ESP8266/user/update","{\"car\":\"1_0\"}",1,0
}

void ESP_subTopic(char *key,char *value){
	delay_ms(500);
	sprintf(SUBTOPIC,"AT+MQTTSUB=0,\"%s\",1\r\n",subTopic);  // ��������
	ESP_SendString(USART2,(uint8_t *)SUBTOPIC);
	
	delay_ms(500);
	sprintf(PUBTOPIC,"AT+MQTTPUB=0,\"%s\",\"{\\\"%s\\\":\\\"%s\\\"}\",1,0\r\n",pubTopic,key,value);  // ��������
	ESP_SendString(USART2,(uint8_t *)PUBTOPIC);
	// ESP_SendString(USART2,(uint8_t *)"AT+MQTTPUB=0,\"/i4c2LeyEp9M/ESP8266/user/update\",\"{\\\"car\\\":\\\"1_0\\\"}\",1,0\r\n");
}


/*
* auto: feifei
* function: ��������У��
* note: �û��������ݺ���ָ��ʱ���� ���ܷ����������� �������Բ������ݽ��
* params: �ȴ�ʱ��
*/
static u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[] = "time";

	USART2_RX_STA = 0;
	while( --waittime )
	{
		delay_ms( 1 );
		if( USART2_RX_STA & 0X8000 )//���յ�һ������
		{
			USART2_RX_STA = 0;
			data = strstr((const char * )USART2_RX_BUF,(const char * )str);
			if(data) return (u8 * )data;	
		}
	}
	return 0;
}


/**
* @author: feifei
* @funtion: JPEG�������ݳ�ʼ��
* @param: none
* @notework: none
* @return: none
*/
void SendPicDataInit( char *WIFIname,char *WIFIpassword ){
	char WIFICON[256],TCPCON[256];
	
	printf("AT+RST\r\n");
	delay_ms(1000);
	sprintf(WIFICON,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFIname,WIFIpassword);
	printf("%s",WIFICON);
	delay_ms(5000);
	printf("AT+CIPMODE=1\r\n");
	delay_ms(1000);
	sprintf(TCPCON,"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",TCPIP,TCPPORT);
	printf("%s",TCPCON);
	delay_ms(5000);
	printf("AT+CIPSEND\r\n");
	delay_ms(1000);
}


