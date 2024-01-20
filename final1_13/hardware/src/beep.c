#include "beep.h"

BEEP_INFO beep_info = {0};

void BEEP_Init(void)	  //端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);   /* 开启GPIO时钟 */

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=BEEP_PIN;		//选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		  //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure); 	 /* 初始化GPIO */
	
	BEEP=0;
	Beep_Set(BEEP_OFF);
}


void Beep_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOB,BEEP_PIN,status == BEEP_ON ? Bit_SET : Bit_RESET);
	
	beep_info.BEEP_Status = status;
	
}
