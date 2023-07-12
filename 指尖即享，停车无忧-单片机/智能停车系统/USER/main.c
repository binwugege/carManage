#include "sys.h"
#include "delay.h"
#include "usart.h"

#include "./key/key.h"
#include "./car/car.h"
#include "./timer/timer.h"

#include "./dcmi/dcmi.h"
#include "./sccb/sccb.h"
#include "./ov2640/ov2640.h"

#include "./oled/oled.h"
#include "./hcsr04/hcsr04.h"
#include "./sim900a/sim900a.h"
#include "./esp8266/esp8266.h"

u8 ov2640_mode = 1;   
u8* p;
u8 size = 3;  
u32 i; 
	
#define jpeg_buf_size 10 * 1024        // ����JPEG���ݻ���jpeg_buf�Ĵ�С(*4�ֽ�)
__align(4) u32 jpeg_buf[jpeg_buf_size];// JPEG���ݻ���buf

volatile u32 jpeg_data_len = 0;        // buf�е�JPEG��Ч���ݳ��� 
volatile u8 jpeg_data_ok = 0;          // JPEG���ݲɼ���ɱ�־ 
						   // 0,����û�вɼ���;
						   // 1,���ݲɼ�����,���ǻ�û����;
						   // 2,�����Ѿ����������,���Կ�ʼ��һ֡����

//JPEG�ߴ�֧���б�
const u16 jpeg_img_size_tbl[][2]=
{
	176,144,	 // QCIF
	160,120,	 // QQVGA
	352,288,	 // CIF
	320,240,	 // QVGA
	640,480,	 // VGA
	800,600,	 // SVGA
	1024,768,	 // XGA
	1280,1024, // SXGA
	1600,1200, // UXGA
};

#define ID1  PBin(12)
#define ID2  PBin(13)
#define ID3  PBin(14)
#define ID4  PBin(15)


// ���ת���Ƕ�
#define open_parking  185
#define close_parking 175

// ������վ��ʶ
unsigned char flag1_T = 0;
unsigned char flag1_F = 0;

unsigned char flag2_T = 0;
unsigned char flag2_F = 0;

unsigned char flag3_T = 0;
unsigned char flag3_F = 0;

unsigned char flag4_T = 0;
unsigned char flag4_F = 0;

float length;                  // �쳵�����ж��Ƿ��г�����
int carInitFlag = 0;           //�����Ƿ񴥷��˳�����
unsigned char overCarFlag = 1; // �Ƿ��г�λʣ�� 0:û��ʣ�࣬1:ʣ��


// ��ȡ��ԤԼ�����ݴ洢����
extern unsigned char id[1],status[1],phone[11];


unsigned int carBookingStatus[4] = {0,0,0,0}; // ��λ��ԤԼ״̬
int initEsp8266Success = 0;                   // esp8266�����ɹ���ſ������ݴ���

// TCP͸������
char TCPIP[] = "192.168.255.159";
char TCPPORT[] = "55559";

// WIFI ����
char WIFIname[] = "feifei";
char WIFIpassword[] = "404340389";

// ����������
char userName[]= "ESP8266&i4c2LeyEp9M";
char password[]= "c09eaa1bdf0038c57f319a4fe420ebcf8eed9798e8d49b7d9e565ae27e9d7aec";
char mqttUrl[]= "iot-06z00a1zoz1yey7.mqtt.iothub.aliyuncs.com";
char clientId[]= "i4c2LeyEp9M.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1673078799334|";

// ��������
char subTopic[] = "/i4c2LeyEp9M/ESP8266/user/get";
char pubTopic[] = "/i4c2LeyEp9M/ESP8266/user/update";


void carGetIn(void);
void runStatus(void);

/**
* @author: feifei
* @funtion: ��ʼ��DCMI��������ESP����͸��
* @param: none
* @notework: none
* @return: none
*/
void dcmiDataInit( void ){
	OV2640_JPEG_Mode();// JPEGģʽ
	My_DCMI_Init();//��ʼ��DCMI
	DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Enable);
	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);
	DCMI_Start();//��������
	SendPicDataInit(WIFIname,WIFIpassword);
}

void jpeg_data_process( void )
{
	if( ov2640_mode )//ֻ����JPEG��ʽ��,����Ҫ������
	{
		if( jpeg_data_ok == 0 )//�����δ�ɼ�������
		{	
			DMA_Cmd(DMA2_Stream1, DISABLE);//ֹͣ��ǰ���� 
			while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);//�ȴ�DMA2_Stream1������  
			jpeg_data_len = jpeg_buf_size - DMA_GetCurrDataCounter(DMA2_Stream1);//�õ��˴����ݴ���ĳ���
			jpeg_data_ok = 1;//���JPEG���ݲɼ����,�ȴ�������������
		}
        
		if( jpeg_data_ok == 2 )//���jpeg�����Ѿ���������
		{
			DMA2_Stream1 -> NDTR = jpeg_buf_size;	
			DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//���䳤��Ϊjpeg_buf_size*4�ֽ�
			DMA_Cmd(DMA2_Stream1,ENABLE);//���´���
			jpeg_data_ok = 0;  //�������δ�ɼ�
			jpeg_data_len = 0;				//�������¿�ʼ
		}
	}
}

void SendPicData( void ){
	while(!jpeg_data_ok);
	if(jpeg_data_ok == 1)//���Ѿ��ɼ���һ֡ͼ��
		{ 	
			p = (u8*) jpeg_buf;
			for(i = 0;i < jpeg_data_len * 4;i++)//dma����1�ε���4�ֽ�,Ҫ��4
			{
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
				USART1->DR = p[i];
				if((p[i]==0XFF) && (p[i+1]==0XD9)){
					while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
				  USART1->DR = p[i+1];
					break;
				}
			}
			jpeg_data_ok = 2; // ���jpeg���ݴ������,������DMA�ɼ���һ֡
		}
}


/**
* @author: feifei
* @funtion: ���Ƴ�λ��ԤԼ״̬
* @param: id->�豸ID status->�豸״̬
* @notework: none
* @return: none
*/
void handleCarBooking( void ){
	switch( id[0] ){
		case('0'):
			if(status[0] == '1'){
				// �жϳ�λ�Ƿ�����  =1δ����
				if( flag1_F ){
					// ���� �豸һ ���
					OLED_ShowString(0,0,(unsigned char *)"carID:1 booking",16,1);
				}else{
					// ��ʾ����������
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car0 booking success");
			}else{
				if( flag1_F ){
					// ���� �ر��豸һ ���
					OLED_ShowString(0,0,(unsigned char *)"carID:1 freeing",16,1);
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car0 cancel success");
			}
			break;
		case('1'):
			if(status[0] == '1'){
				// �жϳ�λ�Ƿ�����  =1δ����
				if( flag2_F ){
					// ���� �豸�� ���
					OLED_ShowString(0,16,(unsigned char *)"carID:2 booking",16,1);
				}else{
					// ��ʾ����������
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car1 booking success");
			}else{				
				if( flag2_F ){
					// ���� �ر��豸�� ���
					OLED_ShowString(0,16,(unsigned char *)"carID:2 freeing",16,1);
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car1 cancel success");
			}
			break;
		case('2'):
			if(status[0] == '1'){
				// �жϳ�λ�Ƿ�����  =1δ����
				if( flag3_F ){
					// ���� �豸�� ���
					OLED_ShowString(0,32,(unsigned char *)"carID:3 booking",16,1);
				}else{
					// ��ʾ����������
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car2 booking success");
			}else{
				if( flag3_F ){
					// ���� �ر��豸�� ���
					OLED_ShowString(0,32,(unsigned char *)"carID:3 freeing",16,1);
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car2 cancel success");				
			}
			break;
		case('3'):
			if(status[0] == '1'){
				// �жϳ�λ�Ƿ�����  =1δ����
				if( flag4_F ){
					// ���� �豸�� ���
					OLED_ShowString(0,48,(unsigned char *)"carID:4 booking",16,1);
				}else{
					// ��ʾ����������
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car3 cancel success");
			}else{		
				if( flag4_F ){
					// ���� �ر��豸�� ���
					OLED_ShowString(0,48,(unsigned char *)"carID:4 freeing",16,1);
				}		
        SendBookingData((unsigned char *)phone,(unsigned char *)"car3 cancel success");				
			}
			break;
	}
}

int main( void )
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );// ����ϵͳ�ж����ȼ�����2
	
	delay_init(168);      // ��ʼ����ʱ����
	uart_init(115200);		// esp8266ͼ���ϴ�
	uart2_init(115200);   // esp8266���ڳ�ʼ��
  uart3_init(9600);     // sim900a���ڳ�ʼ��
	
	// TIM2_PWM_Init(199,8399); // ������Ƶ�PWM
	TIM3_Init(9999,8399);       // ������ʱ�����
	CAR_GPIO_Config();          // ��λ����״̬�ɼ�
	HC_SR04Init();              // ��������ʼ��
	GPIO_IT_Config();           // �ⲿ�жϳ�ʼ�� 
	OLED_Init();				        // ��ʼ��OLED
	OLED_ShowChLength(32,40,11,3);
	// ESP_Init(WIFIname,WIFIpassword,userName,password,mqttUrl,clientId); //esp8266��ʼ��
	
	initEsp8266Success = 1; // �ȴ�esp8266��ʼ�����
	// initCarManage(); 
	while( OV2640_Init() ); //��ʼ��OV2640 �ɹ�����0 ʧ�ܷ���1
	delay_ms(1000);
	dcmiDataInit();
	
	while(1){
		OLED_Refresh();
		carGetIn();
		runStatus();
		delay_ms(500);
	}
	
}



/**
* @author: feifei
* @funtion: ����Ƿ��г�λʣ��
* @param: none
* @notework: none
* @return: none
*/
void checkCarSpace(void){
	// --------����Ƿ��г�λʣ��
	if( ID1 && ID2 && ID3 && ID4 ){ 
		overCarFlag = 1; 
	}
  else{
		overCarFlag = 0;
	}	
}

/**
* @author: feifei
* @funtion: �г���վ
* @param: none
* @notework: none
* @return: none
*/
void carGetIn(void){
	length = Get_Length();
	checkCarSpace();
	
	// --------�г������ҳ�λ����ʣ��
	if(length <	5	&& overCarFlag && !carInitFlag){
		carInitFlag = 1;           // ֹͣ��� �ɳ��������ر�
		TIM_SetCompare1(TIM2,185); // ��������
		SendPicData();            // �ϴ�ͼƬ����
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
	 // �������
	}else if(length <	15 && !overCarFlag){
		TIM_SetCompare1(TIM2,175);
		// ��������� ����ʾ��λ����
	}
	
	// ����û������˽�վ�ĺ���Թ���رն�ʱ��
	if( 0 ){
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCompare1(TIM2,close_parking);
	}
}

/**
* @author: feifei
* @funtion: �ϴ���λ�����
* @param: none
* @notework: none
* @return: none
*/
void runStatus(void){
	// ---------- 1 -------------
	if(ID1 == 0 && flag1_T == 0){
		OLED_ShowString(0,0,(unsigned char *)"carID:1 running",16,1);
		ESP_subTopic("car","0_1");
		flag1_T = 1;
		flag1_F = 0;
	}
	if(ID1 == 1 && flag1_F == 0){
		OLED_ShowString(0,0,(unsigned char *)"carID:1 freeing",16,1);
		ESP_subTopic("car","0_0");
		flag1_T = 0;
		flag1_F = 1;
	}
	// ---------- 2 -------------
	if(ID2 == 0 && flag2_T == 0){
		OLED_ShowString(0,16,(unsigned char *)"carID:2 running",16,1);
		ESP_subTopic("car","1_1");
		flag2_T = 1;
		flag2_F = 0;
	}
	if(ID2 == 1 && flag2_F == 0){
		OLED_ShowString(0,16,(unsigned char *)"carID:2 freeing",16,1);
		ESP_subTopic("car","1_0");
		flag2_T = 0;
		flag2_F = 1;
	}
	
	// ---------- 3 -------------
	if(ID3 == 0 && flag3_T == 0){
		OLED_ShowString(0,32,(unsigned char *)"carID:3 running",16,1);
		ESP_subTopic("car","2_1");
		flag3_T = 1;
		flag3_F = 0;
	}
	if(ID3 == 1 && flag3_F == 0){
		OLED_ShowString(0,32,(unsigned char *)"carID:3 freeing",16,1);
		ESP_subTopic("car","2_0");
		flag3_T = 0;
		flag3_F = 1;
	}
	
	// ---------- 4 -------------
	if(ID4 == 0 && flag4_T == 0){
		OLED_ShowString(0,48,(unsigned char *)"carID:4 running",16,1);
		ESP_subTopic("car","3_1");
		flag4_T = 1;
		flag4_F = 0;
	}
	if(ID4 == 1 && flag4_F == 0){
		OLED_ShowString(0,48,(unsigned char *)"carID:4 freeing",16,1);
		ESP_subTopic("car","3_0");
		flag4_T = 0;
		flag4_F = 1;
	}	
	
}

/**
* @author: feifei
* @funtion: ���еĶ����
* @param: none
* @notework: none
* @return: none
*/
void initCarManage( void ){
	// ��������Ķ���ر�
	TIM_SetCompare1(TIM2,175);
	// ��λ�Ķ��ȫ������
}
