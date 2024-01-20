#include "time.h"
#include "Led.h"
#include "SysTick.h"
#include "beep.h"

extern u8 temp;
extern u8 warning;
extern u8 flow_led_flag;
extern u8 warning_temp;
extern u8 warning_gz;
extern u8 light;
void TIM4_Init(u16 per,u16 psc){
   //时钟使能
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    TIM_TimeBaseInitStructure.TIM_Period=per;
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不分屏
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//定时器中断配置
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM4,ENABLE);
    
}

void TIM3_Init(u16 per,u16 psc){
    //时钟使能
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseInitStructure.TIM_Period=per;
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不分屏
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//定时器中断配置
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM3,ENABLE);
}

void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
    {
        if(temp>=warning_temp&&warning == 1){
        Beep_Set(BEEP_ON);
        }
				else if(temp<warning_temp&&warning == 1){
				Beep_Set(BEEP_OFF);
				}
				
				if(flow_led_flag == 0){
						flow_led_flag++;
						
						LED4 = 0;
						LED5 = 1;
						LED6 = 1;
						LED7 = 1;
						LED8 = 1;
						LED9 = 1;
						LED10 = 1;
						LED11 = 1;
				}
				else if(flow_led_flag == 1){
						flow_led_flag++;
						
						LED4 = 1;
						LED5 = 0;
						LED6 = 1;
						LED7 = 1;
						LED8 = 1;
						LED9 = 1;
						LED10 = 1;
						LED11 = 1;
						
				}
				else if(flow_led_flag == 2){
						flow_led_flag++;
						LED4 = 1;
						LED5 = 1;
						LED6 = 0;
						LED7 = 1;
						LED8 = 1;
						LED9 = 1;
						LED10 = 1;
						LED11 = 1;					
						
				}
				else if(flow_led_flag == 3){
						flow_led_flag++;
						LED4 = 1;
						LED5 = 1;
						LED6 = 1;
						LED7 = 0;
						LED8 = 1;
						LED9 = 1;
						LED10 = 1;
						LED11 = 1;					
						
				}
				else if(flow_led_flag == 4){
						flow_led_flag++;
						LED4 = 1;
						LED5 = 1;
						LED6 = 1;
						LED7 = 1;
						LED8 = 0;
						LED9 = 1;
						LED10 = 1;
						LED11 = 1;					
						
				}
				else if(flow_led_flag == 5){
						flow_led_flag++;
						LED4 = 1;
						LED5 = 1;
						LED6 = 1;
						LED7 = 1;
						LED8 = 1;
						LED9 = 0;
						LED10 = 1;
						LED11 = 1;						
						
				}
				else if(flow_led_flag == 6){
						flow_led_flag++;
						LED4 = 1;
						LED5 = 1;
						LED6 = 1;
						LED7 = 1;
						LED8 = 1;
						LED9 = 1;
						LED10 = 0;
						LED11 = 1;				
						
				}
				else if(flow_led_flag == 7){
						flow_led_flag = 0;
						LED4 = 1;
						LED5 = 1;
						LED6 = 1;
						LED7 = 1;
						LED8 = 1;
						LED9 = 1;
						LED10 = 1;
						LED11 = 0;
						
				}
				else if(flow_led_flag == 8){
						LED4 = 1;
						LED5 = 1;
						LED6 = 1;
						LED7 = 1;
						LED8 = 1;
						LED9 = 1;
						LED10 = 1;
						LED11 = 1;
				}
				else{
						flow_led_flag = 0;
						LED4 = 0;
						LED5 = 0;
						LED6 = 0;
						LED7 = 0;
						LED8 = 0;
						LED9 = 0;
						LED10 = 0;
						LED11 = 0;
						
				}
                
                if(light>=warning_gz&&warning == 1){
                LED1=1;
        }
				else if(light<warning_gz&&warning == 1){
				LED1=0;
				}
				
				
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

//void TIM3_IRQHandler(void)
//{
//    if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
//    {
//        Beep_Set(BEEP_OFF);
//        delay_ms(10000);
//    }
//    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//}