#ifndef __init_h
#define __init_h
#include "sys.h"
#include "stdio.h"	
void RCC_Configuration(void);
void My_USART1_Init(void);
void My_USART2_Init(void);
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);	

void TIM5_Interrupt_Config(void);
void NVIC_Configuration(void);
void System_Init(void);
void TIM4_Interrupt_Config(void);
void My_GPIO_init(void);
void Adc_Init(void);
u16 Get_Adc_Average(u8 ch,u8 times);
u16 Get_Adc(u8 ch);
#endif

