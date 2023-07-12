/*
* @auto: 飞飞超帅的
* @funtion: 用户与阿里云物联网进行网络连接
* @param: none
* @name: feifei
*/


#include "./esp8266/esp8266.h"
#include "./timer/timer.h"
#include <string.h>
#include <stdio.h>


u8 USART2_RX_BUF[USART_REC_LEN]; 
vu16 USART2_RX_STA = 0;       

// 建立连接的缓冲区
char MQTTUSERCFG[256],MQTTCLIENTID[256],MQTTCONN[256],CWJAP[256],SUBTOPIC[256],PUBTOPIC[256];

// 处理数据的缓冲区
char DATA[32],PHONE[32];
char *dataBuf;

extern unsigned int carBookingStatus[4];

// TCP透传配置
extern char TCPIP[];
extern char TCPPORT[];


// 预约数据处理
char id[1],status[1],phone[11];


extern char subTopic[];
extern char pubTopic[];
extern int initEsp8266Success;

// 发送字符串
void ESP_SendString(USART_TypeDef *usart,uint8_t *str);
// 处理预约
extern void handleCarBooking( void );

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART2);                     //(USART2->DR);读取接收到的数据
		if(USART2_RX_STA < 400){
			USART2_RX_BUF[USART2_RX_STA++] = Res;	
		}else{
			USART2_RX_STA = 1<< 15;
		}  		 
  }
	if( USART_GetITStatus(USART2, USART_IT_IDLE) != RESET ){
		
		// esp8266初始化成功开始处理预约数据
		if( initEsp8266Success ){
			dataBuf = strstr((const char *)USART2_RX_BUF,(const char *)"carData");
			if( dataBuf ){
				
				// 收集数据
				strncpy(DATA,dataBuf,32);
				strncpy(id,DATA + 8,1);
				strncpy(status,DATA + 10,1);
				strncpy(phone,DATA + 18,11);
				
				// 数据正确，硬件开始处理预约
				handleCarBooking();
			}else{
				// 未登录 可以用led闪烁
			}
			
		}
		USART2_RX_BUF[USART2_RX_STA++] =  USART_ReceiveData(USART2);
		USART2_RX_STA = 0;
	}

}




void uart2_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART2
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIO3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
  USART_Cmd(USART2, ENABLE);  //使能串口2 	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启相关中断

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
	USART2_RX_STA = 0;		 // 清零

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
	sprintf(SUBTOPIC,"AT+MQTTSUB=0,\"%s\",1\r\n",subTopic);  // 订阅主题
	ESP_SendString(USART2,(uint8_t *)SUBTOPIC);
	
	delay_ms(500);
	sprintf(PUBTOPIC,"AT+MQTTPUB=0,\"%s\",\"{\\\"%s\\\":\\\"%s\\\"}\",1,0\r\n",pubTopic,key,value);  // 订阅主题
	ESP_SendString(USART2,(uint8_t *)PUBTOPIC);
	// ESP_SendString(USART2,(uint8_t *)"AT+MQTTPUB=0,\"/i4c2LeyEp9M/ESP8266/user/update\",\"{\\\"car\\\":\\\"1_0\\\"}\",1,0\r\n");
}


/*
* auto: feifei
* function: 串口数据校验
* note: 用户发送数据后在指定时间内 接受返回来的数据 进而可以查验数据结果
* params: 等待时间
*/
static u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[] = "time";

	USART2_RX_STA = 0;
	while( --waittime )
	{
		delay_ms( 1 );
		if( USART2_RX_STA & 0X8000 )//接收到一次数据
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
* @funtion: JPEG传输数据初始化
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


