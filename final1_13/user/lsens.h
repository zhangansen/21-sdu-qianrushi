#ifndef _lsens_H
#define _lsens_H


#include "system.h"  


void Lsens_Init(void); 				//��ʼ������������
u8 Lsens_Get_Val(void);				//��ȡ������������ֵ
u8 Lsens_Read_Data(u8 *light);
#endif 
