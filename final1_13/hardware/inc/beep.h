#ifndef _beep_H
#define _beep_H

#include "system.h"

typedef struct
{
	
	_Bool BEEP_Status;
	
} BEEP_INFO;

extern BEEP_INFO beep_info;


/*  蜂鸣器时钟端口、引脚定义 */
#define BEEP_PORT 			GPIOB   
#define BEEP_PIN 			GPIO_Pin_8
#define BEEP_PORT_RCC		RCC_APB2Periph_GPIOB

#define BEEP PBout(8)

#define BEEP_ON 1

#define BEEP_OFF 0

void BEEP_Init(void);

void Beep_Set(_Bool status);

#endif
