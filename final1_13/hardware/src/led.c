#include "led.h"

LED_INFO led_info = {0};

/*******************************************************************************
* 连接方式     : PF0-PF7,接到八个跑马灯，任意接没有先后顺序
* 注    意		 : 
*******************************************************************************/

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(LED1_PORT_RCC|LED2_PORT_RCC,ENABLE);
	
	
	// RCC_APB2PeriphClockCmd(LED3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED4_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED5_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED6_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED7_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED8_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED9_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED10_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(LED11_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(LED1_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	GPIO_SetBits(LED1_PORT,LED1_PIN);   //将LED端口拉高，熄灭所有LED
	
	GPIO_InitStructure.GPIO_Pin=LED2_PIN;  //选择你要设置的IO口
	GPIO_Init(LED2_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	GPIO_SetBits(LED2_PORT,LED2_PIN);   //将LED端口拉高，熄灭所有LED
	
	//GPIO_InitStructure.GPIO_Pin=LED3_PIN; 
	//GPIO_Init(LED3_PORT,&GPIO_InitStructure);
	//GPIO_SetBits(LED3_PORT,LED3_PIN);
	
	GPIO_InitStructure.GPIO_Pin=LED4_PIN; 
	GPIO_Init(LED4_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED4_PORT,LED4_PIN);
	
	GPIO_InitStructure.GPIO_Pin=LED5_PIN; 
	GPIO_Init(LED5_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED5_PORT,LED5_PIN);
	
	GPIO_InitStructure.GPIO_Pin=LED6_PIN; 
	GPIO_Init(LED6_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED6_PORT,LED6_PIN);
	
	GPIO_InitStructure.GPIO_Pin=LED7_PIN; 
	GPIO_Init(LED7_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED7_PORT,LED7_PIN);
	
	GPIO_InitStructure.GPIO_Pin=LED8_PIN; 
	GPIO_Init(LED8_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED8_PORT,LED8_PIN);
	
	GPIO_InitStructure.GPIO_Pin=LED9_PIN; 
	GPIO_Init(LED9_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED9_PORT,LED9_PIN);
	
	GPIO_InitStructure.GPIO_Pin=LED10_PIN; 
	GPIO_Init(LED10_PORT,&GPIO_InitStructure);
	GPIO_SetBits(LED10_PORT,LED10_PIN);
	
	//GPIO_InitStructure.GPIO_Pin=LED11_PIN; 
	//GPIO_Init(LED11_PORT,&GPIO_InitStructure);
	//GPIO_SetBits(LED11_PORT,LED11_PIN);
	
	
	GPIO_InitStructure.GPIO_Pin=LED11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入模式
	GPIO_Init(LED11_PORT,&GPIO_InitStructure);	
	
	//GPIO_InitStructure.GPIO_Pin=LED11_PIN;  //选择你要设置的IO口
	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 //设置推挽输出模式
	//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	//GPIO_Init(LED11_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	//GPIO_SetBits(LED11_PORT,LED11_PIN);   //将LED端口拉高，熄灭所有LED
	

	Led_Set(LED_OFF);
}

void Led_Set(_Bool status)
{
	
	GPIO_WriteBit(LED1_PORT,LED1_PIN,status == LED_ON ? Bit_RESET : Bit_SET);
	
	led_info.LED_Status = status;
	
}
//LED端口8位数据同时操作，不影响高位
//写入数据到8位端口，数据低位对应端口低引脚
//GPIO_Pin：8位端口低位引脚
//data：写入数据
void LED_Write_Data(u16 GPIO_Pin,u8 data)
{
	u8 i,j=GPIO_Pin;    
	for(i=0;i<8;i++)
	{
		if(data&0x01)
			GPIO_WriteBit(GPIOA, j<<i ,Bit_SET); 
		else
			GPIO_WriteBit(GPIOA, j<<i ,Bit_RESET); 
		data = data >> 1 ; 
	}
}
void Fire_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //????ê?è??￡ê?
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
 
/*数据读取函数*/
u8 Fire_GET() 
{
	u8 fire_value;
	while(1)
	{
		fire_value=GPIO_ReadInputDataBit(GPIOD,  GPIO_Pin_3);
		return  fire_value;
	}
}
