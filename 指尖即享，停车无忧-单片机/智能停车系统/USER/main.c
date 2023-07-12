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
	
#define jpeg_buf_size 10 * 1024        // 定义JPEG数据缓存jpeg_buf的大小(*4字节)
__align(4) u32 jpeg_buf[jpeg_buf_size];// JPEG数据缓存buf

volatile u32 jpeg_data_len = 0;        // buf中的JPEG有效数据长度 
volatile u8 jpeg_data_ok = 0;          // JPEG数据采集完成标志 
						   // 0,数据没有采集完;
						   // 1,数据采集完了,但是还没处理;
						   // 2,数据已经处理完成了,可以开始下一帧接收

//JPEG尺寸支持列表
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


// 舵机转动角度
#define open_parking  185
#define close_parking 175

// 车辆进站标识
unsigned char flag1_T = 0;
unsigned char flag1_F = 0;

unsigned char flag2_T = 0;
unsigned char flag2_F = 0;

unsigned char flag3_T = 0;
unsigned char flag3_F = 0;

unsigned char flag4_T = 0;
unsigned char flag4_F = 0;

float length;                  // 检车距离判断是否有车经过
int carInitFlag = 0;           //车辆是否触发了超声波
unsigned char overCarFlag = 1; // 是否有车位剩余 0:没有剩余，1:剩余


// 获取的预约的数据存储下来
extern unsigned char id[1],status[1],phone[11];


unsigned int carBookingStatus[4] = {0,0,0,0}; // 车位的预约状态
int initEsp8266Success = 0;                   // esp8266启动成功后才开启数据处理

// TCP透传配置
char TCPIP[] = "192.168.255.159";
char TCPPORT[] = "55559";

// WIFI 配置
char WIFIname[] = "feifei";
char WIFIpassword[] = "404340389";

// 阿里云配置
char userName[]= "ESP8266&i4c2LeyEp9M";
char password[]= "c09eaa1bdf0038c57f319a4fe420ebcf8eed9798e8d49b7d9e565ae27e9d7aec";
char mqttUrl[]= "iot-06z00a1zoz1yey7.mqtt.iothub.aliyuncs.com";
char clientId[]= "i4c2LeyEp9M.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1673078799334|";

// 订阅配置
char subTopic[] = "/i4c2LeyEp9M/ESP8266/user/get";
char pubTopic[] = "/i4c2LeyEp9M/ESP8266/user/update";


void carGetIn(void);
void runStatus(void);

/**
* @author: feifei
* @funtion: 初始化DCMI并且启动ESP数据透传
* @param: none
* @notework: none
* @return: none
*/
void dcmiDataInit( void ){
	OV2640_JPEG_Mode();// JPEG模式
	My_DCMI_Init();//初始化DCMI
	DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_HalfWord,DMA_MemoryInc_Enable);
	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);
	DCMI_Start();//启动传输
	SendPicDataInit(WIFIname,WIFIpassword);
}

void jpeg_data_process( void )
{
	if( ov2640_mode )//只有在JPEG格式下,才需要做处理
	{
		if( jpeg_data_ok == 0 )//如果还未采集完数据
		{	
			DMA_Cmd(DMA2_Stream1, DISABLE);//停止当前传输 
			while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);//等待DMA2_Stream1可配置  
			jpeg_data_len = jpeg_buf_size - DMA_GetCurrDataCounter(DMA2_Stream1);//得到此次数据传输的长度
			jpeg_data_ok = 1;//标记JPEG数据采集完成,等待其他函数处理
		}
        
		if( jpeg_data_ok == 2 )//如果jpeg数据已经被处理了
		{
			DMA2_Stream1 -> NDTR = jpeg_buf_size;	
			DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//传输长度为jpeg_buf_size*4字节
			DMA_Cmd(DMA2_Stream1,ENABLE);//重新传输
			jpeg_data_ok = 0;  //标记数据未采集
			jpeg_data_len = 0;				//数据重新开始
		}
	}
}

void SendPicData( void ){
	while(!jpeg_data_ok);
	if(jpeg_data_ok == 1)//若已经采集完一帧图像
		{ 	
			p = (u8*) jpeg_buf;
			for(i = 0;i < jpeg_data_len * 4;i++)//dma传输1次等于4字节,要乘4
			{
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
				USART1->DR = p[i];
				if((p[i]==0XFF) && (p[i+1]==0XD9)){
					while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
				  USART1->DR = p[i+1];
					break;
				}
			}
			jpeg_data_ok = 2; // 标记jpeg数据处理完毕,可以让DMA采集下一帧
		}
}


/**
* @author: feifei
* @funtion: 控制车位的预约状态
* @param: id->设备ID status->设备状态
* @notework: none
* @return: none
*/
void handleCarBooking( void ){
	switch( id[0] ){
		case('0'):
			if(status[0] == '1'){
				// 判断车位是否运行  =1未运行
				if( flag1_F ){
					// 开启 设备一 舵机
					OLED_ShowString(0,0,(unsigned char *)"carID:1 booking",16,1);
				}else{
					// 提示车辆运行中
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car0 booking success");
			}else{
				if( flag1_F ){
					// 操作 关闭设备一 舵机
					OLED_ShowString(0,0,(unsigned char *)"carID:1 freeing",16,1);
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car0 cancel success");
			}
			break;
		case('1'):
			if(status[0] == '1'){
				// 判断车位是否运行  =1未运行
				if( flag2_F ){
					// 开启 设备二 舵机
					OLED_ShowString(0,16,(unsigned char *)"carID:2 booking",16,1);
				}else{
					// 提示车辆运行中
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car1 booking success");
			}else{				
				if( flag2_F ){
					// 操作 关闭设备二 舵机
					OLED_ShowString(0,16,(unsigned char *)"carID:2 freeing",16,1);
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car1 cancel success");
			}
			break;
		case('2'):
			if(status[0] == '1'){
				// 判断车位是否运行  =1未运行
				if( flag3_F ){
					// 开启 设备三 舵机
					OLED_ShowString(0,32,(unsigned char *)"carID:3 booking",16,1);
				}else{
					// 提示车辆运行中
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car2 booking success");
			}else{
				if( flag3_F ){
					// 操作 关闭设备三 舵机
					OLED_ShowString(0,32,(unsigned char *)"carID:3 freeing",16,1);
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car2 cancel success");				
			}
			break;
		case('3'):
			if(status[0] == '1'){
				// 判断车位是否运行  =1未运行
				if( flag4_F ){
					// 开启 设备四 舵机
					OLED_ShowString(0,48,(unsigned char *)"carID:4 booking",16,1);
				}else{
					// 提示车辆运行中
				}
				SendBookingData((unsigned char *)phone,(unsigned char *)"car3 cancel success");
			}else{		
				if( flag4_F ){
					// 操作 关闭设备四 舵机
					OLED_ShowString(0,48,(unsigned char *)"carID:4 freeing",16,1);
				}		
        SendBookingData((unsigned char *)phone,(unsigned char *)"car3 cancel success");				
			}
			break;
	}
}

int main( void )
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );// 设置系统中断优先级分组2
	
	delay_init(168);      // 初始化延时函数
	uart_init(115200);		// esp8266图像上传
	uart2_init(115200);   // esp8266串口初始化
  uart3_init(9600);     // sim900a串口初始化
	
	// TIM2_PWM_Init(199,8399); // 舵机控制的PWM
	TIM3_Init(9999,8399);       // 升降杆时间控制
	CAR_GPIO_Config();          // 车位引脚状态采集
	HC_SR04Init();              // 超声波初始化
	GPIO_IT_Config();           // 外部中断初始化 
	OLED_Init();				        // 初始化OLED
	OLED_ShowChLength(32,40,11,3);
	// ESP_Init(WIFIname,WIFIpassword,userName,password,mqttUrl,clientId); //esp8266初始化
	
	initEsp8266Success = 1; // 等待esp8266初始化完毕
	// initCarManage(); 
	while( OV2640_Init() ); //初始化OV2640 成功返回0 失败返回1
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
* @funtion: 检查是否还有车位剩余
* @param: none
* @notework: none
* @return: none
*/
void checkCarSpace(void){
	// --------检测是否还有车位剩余
	if( ID1 && ID2 && ID3 && ID4 ){ 
		overCarFlag = 1; 
	}
  else{
		overCarFlag = 0;
	}	
}

/**
* @author: feifei
* @funtion: 有车进站
* @param: none
* @notework: none
* @return: none
*/
void carGetIn(void){
	length = Get_Length();
	checkCarSpace();
	
	// --------有车进来且车位还有剩余
	if(length <	5	&& overCarFlag && !carInitFlag){
		carInitFlag = 1;           // 停止检测 由超声波来关闭
		TIM_SetCompare1(TIM2,185); // 打开升降杆
		SendPicData();            // 上传图片数据
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
	 // 舵机工作
	}else if(length <	15 && !overCarFlag){
		TIM_SetCompare1(TIM2,175);
		// 舵机不工作 且提示车位已满
	}
	
	// 如果用户触发了进站的红外对管则关闭定时器
	if( 0 ){
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,DISABLE);
		TIM_SetCompare1(TIM2,close_parking);
	}
}

/**
* @author: feifei
* @funtion: 上传车位的情况
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
* @funtion: 所有的舵机打开
* @param: none
* @notework: none
* @return: none
*/
void initCarManage( void ){
	// 管理进出的舵机关闭
	TIM_SetCompare1(TIM2,175);
	// 车位的舵机全部开启
}
