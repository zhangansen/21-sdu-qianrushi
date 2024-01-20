#include "cst716.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "SysTick.h" 
#include "string.h" 


//д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 CST716_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(CST_CMD_WR);	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
//����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void CST716_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(CST_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(CST_CMD_RD);   	//���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����     
} 

u8 CIP[5]; //������Ŵ���IC
//��ʼ��������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
u8 CST716_Init(void)
{
	u8 temp[5];  		
 	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PF�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 // PF11�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);//PF11�������
	GPIO_SetBits(GPIOF,GPIO_Pin_11);//����
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 // PB2�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_Init(GPIOF, &GPIO_InitStructure);//PF10��������
	GPIO_SetBits(GPIOF,GPIO_Pin_10);//����		

	CT_IIC_Init();      	//��ʼ����������I2C����  
	CST_RST=0;				//��λ
	delay_ms(20);
 	CST_RST=1;				//�ͷŸ�λ		    
	delay_ms(50);  
	
	CST716_RD_Reg(0xA7,&temp[0],1);
	printf("temp[0]=%d\r\n",temp[0]);
	
	temp[0]=0;
	CST716_WR_Reg(CST_DEVIDE_MODE,temp,1);	//������������ģʽ 
	CST716_WR_Reg(CST_ID_G_MODE,temp,1);		//��ѯģʽ 
	temp[0]=22;								//������Чֵ��22��ԽСԽ����	
	CST716_WR_Reg(CST_ID_G_THGROUP,temp,1);	//���ô�����Чֵ
	temp[0]=12;								//�������ڣ�����С��12�����14
	CST716_WR_Reg(CST_ID_G_PERIODACTIVE,temp,1); 
	//��ȡ�汾�ţ��ο�ֵ��0x3003
	CST716_RD_Reg(CST_ID_G_LIB_VERSION,&temp[0],2);  
//	if(temp[0]==0X30&&temp[1]==0X03)//�汾:0X3003
	{ 
		printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
	} 
    return 0;
}

const u16 CST716_TPX_TBL[5]={CST_TP1_REG,CST_TP2_REG};
u8 g_gt_tnum=2;      //Ĭ��֧�ֵĴ���������(2�㴥��)

//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 CST716_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
    u16 tempsta;
	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{
        CST716_RD_Reg(CST_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬  
        
		if((mode&0XF)&&((mode&0XF)<=g_gt_tnum))
		{
			temp=0XFF<<(mode&0XF);//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tempsta=tp_dev.sta;    //���浱ǰ��tp_dev.staֵ
            tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
            tp_dev.x[g_gt_tnum-1]=tp_dev.x[0];//���津��0������,���������һ����
            tp_dev.y[g_gt_tnum-1]=tp_dev.y[0];

            delay_ms(4);    //��Ҫ����ʱ������������Ϊ�а�������

			for(i=0;i<g_gt_tnum;i++)
			{
				if(tp_dev.sta&(1<<i))		//������Ч?
				{
					CST716_RD_Reg(CST716_TPX_TBL[i],buf,4);	//��ȡXY����ֵ 
					if(tp_dev.touchtype&0X01)//����
					{
						tp_dev.y[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
						tp_dev.x[i]=(((u16)(buf[2]&0X0F)<<8)+buf[3]);
					}else
					{
						tp_dev.x[i]=(((u16)(buf[0]&0X0F)<<8)+buf[1]);
						tp_dev.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
					} 
                    
					printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
            if(tp_dev.x[0]>tftlcd_data.width||tp_dev.y[0]>tftlcd_data.height)  //�Ƿ�����(���곬����)
            {
                if((mode&0XF)>1)   // ��������������,�򸴵ڶ�����������ݵ���һ������.
                {
                    tp_dev.x[0]=tp_dev.x[1];
                    tp_dev.y[0]=tp_dev.y[1];
                    t=0;  // ����һ��,��������������10��,�Ӷ���������� 
                }
                else        // �Ƿ�����,����Դ˴�����(��ԭԭ����) 
                {
                    tp_dev.x[0]=tp_dev.x[g_gt_tnum-1];
                    tp_dev.y[0]=tp_dev.y[g_gt_tnum-1];
                    mode=0X80;
                    tp_dev.sta=tempsta;   // �ָ�tp_dev.sta 
                }
            }
            else t=0;      // ����һ��,��������������10��,�Ӷ���������� 
		}
	}
 
	if((mode&0X1F)==0)//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~TP_PRES_DOWN;	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}
	}
    
	if(t>240)t=10;//���´�10��ʼ����
	return res;
}
