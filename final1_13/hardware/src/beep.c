#include "beep.h"

BEEP_INFO beep_info = {0};

void BEEP_Init(void)	  //�˿ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_PORT_RCC,ENABLE);   /* ����GPIOʱ�� */

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=BEEP_PIN;		//ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		  //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(BEEP_PORT,&GPIO_InitStructure); 	 /* ��ʼ��GPIO */
	
	BEEP=0;
	Beep_Set(BEEP_OFF);
}


void Beep_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOB,BEEP_PIN,status == BEEP_ON ? Bit_SET : Bit_RESET);
	
	beep_info.BEEP_Status = status;
	
}
