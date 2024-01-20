#ifndef _led_H
#define _led_H

#include "system.h"

typedef struct
{
	
	_Bool LED_Status;
	
} LED_INFO;

extern LED_INFO led_info;

/*  LED时钟端口、引脚定义 */
#define LED1_PORT 			GPIOB   
#define LED1_PIN 			GPIO_Pin_5
#define LED1_PORT_RCC		RCC_APB2Periph_GPIOB

#define LED2_PORT 			GPIOE   
#define LED2_PIN 			GPIO_Pin_5
#define LED2_PORT_RCC		RCC_APB2Periph_GPIOE

/*  LED时钟端口、引脚定义 */
// #define LED3_PORT 			GPIOC  
// #define LED3_PIN 			GPIO_Pin_13
// #define LED3_PORT_RCC		RCC_APB2Periph_GPIOC

#define LED4_PORT 			GPIOF
#define LED4_PIN 			GPIO_Pin_0
#define LED4_PORT_RCC		RCC_APB2Periph_GPIOF

#define LED5_PORT 			GPIOF  
#define LED5_PIN 			GPIO_Pin_1
#define LED5_PORT_RCC		RCC_APB2Periph_GPIOF

#define LED6_PORT 			GPIOF  
#define LED6_PIN 			GPIO_Pin_2
#define LED6_PORT_RCC		RCC_APB2Periph_GPIOF

#define LED7_PORT 			GPIOF  
#define LED7_PIN 			GPIO_Pin_3
#define LED7_PORT_RCC		RCC_APB2Periph_GPIOF

#define LED8_PORT 			GPIOF  
#define LED8_PIN 			GPIO_Pin_4
#define LED8_PORT_RCC		RCC_APB2Periph_GPIOF

#define LED9_PORT 			GPIOF  
#define LED9_PIN 			GPIO_Pin_5
#define LED9_PORT_RCC		RCC_APB2Periph_GPIOF

#define LED10_PORT 			GPIOF  
#define LED10_PIN 			GPIO_Pin_6
#define LED10_PORT_RCC		RCC_APB2Periph_GPIOF

#define LED11_PORT 			GPIOF  
#define LED11_PIN 			GPIO_Pin_7
#define LED11_PORT_RCC		RCC_APB2Periph_GPIOF



#define LED1 PBout(5)  	
#define LED2 PEout(5)

//位带定义
// #define LED3 	PCout(13)
#define LED4 	PFout(0)  
#define LED5 	PFout(1) 
#define LED6 	PFout(2)  
#define LED7 	PFout(3)
#define LED8 	PFout(4)  
#define LED9 	PFout(5)
#define LED10 	PFout(6)  
#define LED11 	PFout(7)

#define LED_ON 1

#define LED_OFF 0

void LED_Init(void);

void Led_Set(_Bool status);

void LED_Write_Data(u16 GPIO_Pin,u8 data);

#endif
