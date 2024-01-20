// 网络协议层
#include "onenet.h"

// 网络设备
#include "esp8266.h"

// 硬件驱动
#include "usart.h"
#include "beep.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "time.h"
#include "dht11.h"
#include "tftlcd.h"
#include "lsens.h"
#include "hwjs.h"
#include "ws2812.h"
#include <string.h>
#include "touch_key.h"
#include "touch.h"
#include "picture.h"
#include "pwm.h"

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
void Display_Init(void);
void Hardware_Init(void);
void Display_Init(void);
void Refresh_Data(void);
void RGB_ShowDemo(void);
u8 temp,humi,hwsend;
char lsens_str[4];
char hw_str[4];
char button_flow_str[4];
char button_led_str[4];
char button_beep_str[4];
u8 light=0;
u8 RGB_flag = 0;
u8 judgetime = 0;
u8 warning = 0;
u8 warning_temp = 30;
u8 warning_gz = 50;
u8 flow_led_flag = 0;
u8 dianji_flag = 0;
u8 tag=0;
u8 value = 0;
char value_[4];
u8 temp_buf[3],humi_buf[3],cam_buf[3];


int main(void)
{
	
	unsigned short timeCount = 0; // 发送间隔变量
//    u8 key;
	unsigned char *dataPtr = NULL;
	LED_Init();
	LED1=0;LED2=0; 
	LED4=0;LED5=0;LED6=0;LED7=0;LED8=0;LED9=0;LED10=0;LED11=0;
	
	Hardware_Init(); // 初始化外围硬件
	RGB_ShowCharNum(15,RGB_COLOR_BLUE);
	ESP8266_Init(); // 初始化ESP8266
		BEEP=1;
		Beep_Set(BEEP_OFF);
	LED1=0;
	Led_Set(LED_ON);
	//RGB
	
	
	
	UsartPrintf(USART_DEBUG,"Connect MQTTs Server...\r\n");
	LCD_ShowString(10,30,tftlcd_data.width,tftlcd_data.height,16,"Connect MQTTs Server...");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO,"CONNECT"))
		delay_ms(500);
	UsartPrintf(USART_DEBUG,"Connect MQTT Server Success\r\n");
	LCD_ShowString(10,50,tftlcd_data.width,tftlcd_data.height,16,"Connect MQTT Server Success");

	while(OneNet_DevLink())
		delay_ms(500);

	OneNET_Subscribe();
	
	while (1)
	{
		//UsartPrintf(USART_DEBUG,"温度=%d °C  湿度=%d %%RH\r\n",temp,humi);
		if(Touch_Key_Scan(0)==1)
		{
			BEEP=!BEEP;
			LED2 = !LED2;
			RGB_ShowCharNum(RGB_flag%16,RGB_COLOR_BLUE);
			delay_ms(100);
		}
		
		//LCD_ShowFontHZ(10,78,value_);
		
		if(++timeCount >= 100)
		{
			DHT11_Read_Data(&temp,&humi);
			Lsens_Read_Data(&light);
			Refresh_Data();
			OneNet_SendData();									//发送数据
		
			timeCount = 0;
			ESP8266_Clear();
		}
		TP_Scan(0);
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(10);
	}
}

void Hardware_Init(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断控制器分组设置
	SysTick_Init(72);
	BEEP_Init();
	Hwjs_Init();   //红外
	Lsens_Init();  //光敏
	KEY_Init();
    TIM3_CH2_PWM_Init(500,72-1); //频率是2Kh
	TIM_SetCompare2(TIM3,10);
	My_EXTI_Init(); // 外部中断初始化	//systick初始化
    TIM4_Init(10000,36000-1);//1000个0.5豪秒==500毫秒
    //TIM3_Init(1000,36000-1);
	Usart1_Init(115200); // 串口1，打印信息用
	Usart3_Init(115200); // 串口3，驱动ESP8266用
	Touch_Key_Init(6);   //计数频率为12M
	TFTLCD_Init();
	Display_Init();
    TP_Init();//触摸屏
	RGB_LED_Init(); //RGB
	while(DHT11_Init())	//检测DS18B20是否存在
	{
		LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"DHT11 Check Error!");
		UsartPrintf(USART_DEBUG, "DHT11 Check Error!\r\n");
		delay_ms(500);		
	}
	
	UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");
	LCD_ShowString(10,10,tftlcd_data.width,tftlcd_data.height,16,"Hardware init OK");
}

void Display_Init(void)
{
	
	FRONT_COLOR=BLACK;
	LCD_ShowFontHZ(10,78,"温度");
	LCD_ShowFontHZ(10,118,"湿度");
	LCD_ShowFontHZ(10,158,"光照");
	LCD_ShowFontHZ(10,198,"红外");
	LCD_ShowFontHZ(10,240,"跑马灯");
	LCD_ShowString(10,280,tftlcd_data.width,tftlcd_data.height,24,"mag");
	LCD_ShowPicture(120,280,32,32,(u8 *)gImage_picture);
	LCD_ShowPicture(160,280,32,32,(u8 *)gImage_picture3);
	LCD_ShowString(10,320,tftlcd_data.width,tftlcd_data.height,24,"num");
	LCD_ShowString(70,80,tftlcd_data.width,tftlcd_data.height,24,":   C");
	LCD_ShowString(70,120,tftlcd_data.width,tftlcd_data.height,24,":   %RH");
	LCD_ShowString(70,160,tftlcd_data.width,tftlcd_data.height,24,":   ");
	LCD_ShowString(70,200,tftlcd_data.width,tftlcd_data.height,24,":   ");
	LCD_ShowFontHZ(10,360,"李昭晨");
	LCD_ShowFontHZ(10,400,"张安森");
	LCD_ShowFontHZ(10,440,"张洲豪");
	LCD_ShowString(170,360,tftlcd_data.width,tftlcd_data.height,24,"202100810205");
	LCD_ShowString(170,400,tftlcd_data.width,tftlcd_data.height,24,"202100810123");
	LCD_ShowString(170,440,tftlcd_data.width,tftlcd_data.height,24,"202100810284");
	FRONT_COLOR=GREEN;
	
}
void Refresh_Data(void)
{
	u8 light=0;
	DHT11_Read_Data(&temp,&humi);
	temp_buf[0]=temp/10+0x30;	
	temp_buf[1]=temp%10+0x30;
	temp_buf[2]='\0';
	LCD_ShowString(85,80,tftlcd_data.width,tftlcd_data.height,24,temp_buf);
	if(LED1==0){
	strcpy(button_led_str, "on");  // 正确
	}
	else{
	strcpy(button_led_str, "of");  // 正确
	}
	if(BEEP==0){
	strcpy(button_beep_str, "on");  // 正确
	}
	else{
	strcpy(button_beep_str, "of");  // 正确
	}
	value = Fire_GET();
	if(value==0){
		strcpy(button_led_str, "yes");
		UsartPrintf(USART_DEBUG,"检测到磁场\r\n");
	}
	else{
		strcpy(button_led_str, "noo");
	}
	//strcpy(button_flow_str, "on");
	humi_buf[0]=humi/10+0x30;	
	humi_buf[1]=humi%10+0x30;
	humi_buf[2]='\0';
	cam_buf[0]=tag/10+0x30;	
	cam_buf[1]=tag%10+0x30;
	cam_buf[2]='\0';
	LCD_ShowString(85,120,tftlcd_data.width,tftlcd_data.height,24,humi_buf);
	light=Lsens_Get_Val();
	sprintf(lsens_str, "%u", light);  // lsens_value
	LCD_ShowString(85,160,tftlcd_data.width,tftlcd_data.height,24,lsens_str);
	sprintf(hw_str, "%u", hwsend);
	LCD_ShowString(85,200,tftlcd_data.width,tftlcd_data.height,24,hw_str);
	LCD_ShowString(85,240,tftlcd_data.width,tftlcd_data.height,24,button_flow_str);
	LCD_ShowString(85,280,tftlcd_data.width,tftlcd_data.height,24,button_led_str);
	LCD_ShowString(85,320,tftlcd_data.width,tftlcd_data.height,24,cam_buf);
	
    if(hw_jsbz==1)	//如果红外接收到
		{
			hw_jsbz=0;	   //清零
			printf("红外接收码 %0.8X\r\n",hw_jsm);	//打印
            if(hw_jsm==0x00FF30CF)
            {
                hwsend=1;
								flow_led_flag = 8;
								strcpy(button_flow_str, "of"); 
            }
            if(hw_jsm==0x00FF18E7)
            {
                hwsend=2;
								flow_led_flag = 0;
								strcpy(button_flow_str, "on");  
            }
            if(hw_jsm==0x00FF7A85)
            {
                hwsend=3;
								
							
								Led_Set(LED_OFF);
								
            }
            if(hw_jsm==0x00FF10EF)
            {
                hwsend=4; 
								
								
								Led_Set(LED_ON);
								
            }
            if(hw_jsm==0x00FF38C7)
            {
                hwsend=5;
								//strcpy(button_beep_str, "of");  // 正确
								Beep_Set(BEEP_OFF);
                
            }
            if(hw_jsm==0x00FF5AA5)
            {
                hwsend=6;
								//strcpy(button_beep_str, "on");  // 正确
								Beep_Set(BEEP_ON);
            }
			hw_jsm=0;					//接收码清零
		}
}


void RGB_ShowDemo(void)
{
	while(1)
	{
		RGB_LED_Red();
		delay_ms(1000);
		RGB_LED_Green();
		delay_ms(1000);
		RGB_LED_Blue();
		delay_ms(1000);
		break;
	}
	RGB_LED_Clear();
}

