#ifndef _cst716_H
#define _cst716_H


#include "system.h"


//����ݴ��������ӵ�оƬ����(δ����IIC����) 
//IO��������	 
#define CST_RST    				PFout(11)	//FT5206��λ����
#define CST_INT    				PFin(10)	//FT5206�ж�����	

//I2C��д����	
#define CST_CMD_WR 				0X2A    	//д����
#define CST_CMD_RD 				0X2B		//������
  
//���ּĴ������� 
#define CST_DEVIDE_MODE 		0x00   		//ģʽ���ƼĴ���
#define CST_REG_NUM_FINGER      0x02		//����״̬�Ĵ���

#define CST_TP1_REG 			0X03	  	//��һ�����������ݵ�ַ
#define CST_TP2_REG 			0X09		//�ڶ������������ݵ�ַ
 
 
#define	CST_ID_G_LIB_VERSION	0xA1		//�汾		
#define CST_ID_G_MODE 			0xA4   		//FT5206�ж�ģʽ���ƼĴ���
#define CST_ID_G_THGROUP		0x80   		//������Чֵ���üĴ���
#define CST_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���


u8 CST716_WR_Reg(u16 reg,u8 *buf,u8 len);
void CST716_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 CST716_Init(void);
u8 CST716_Scan(u8 mode);

#endif
