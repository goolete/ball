#include "stm32f10x.h"
#include "MKEY.h"
#include "sys.h"
#include "init.h"
#include "delay.h"
#include "init.h"
#include "MotorMove.h"
#include "oled.h"

extern float Kp,Ki,Kd;
extern float Kpp,Kip,Kdp;
extern unsigned char hanzi1[];
int num;
/**
** PB5-PB8,DD��?��?3??�� PB10-PB14,��D��?��?��?
**/
void Key_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;   
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   
	 /********4DD��?3?*********/
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	 /********4��D��?��?*********/
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

int KeyScan(void)
{
   uchar KeyVal;
	
   GPIO_Write(GPIOB, (GPIOB->ODR & 0xf0ff | 0x0f00));	 //2?1��??����12??0xf0ff��??����?PB8��?PB11��?2?��?3????��
	  
	 if((GPIOB->IDR & 0xf000)==0x0000)   								 //��?1?PB12��?PB15��??a��??��??��D���?�����??
		  return -1;
	 else
	 {	
	    delay_ms(5);    //?������5ms����???��
	    if((GPIOB->IDR & 0xf000)==0x0000)
	    return -1;
	 }
	 
	 GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0100));	//????PB8????	 
	 switch(GPIOB->IDR & 0xf000)	//�̨���?DD��?�����̨���???��?��?������?2?��?��?
	 {
     	case 0x1000: KeyVal=1; break;
			case 0x2000: KeyVal=2;	break;
			case 0x4000: KeyVal=3;	break;
			case 0x8000: KeyVal=4;	break;
   }
	 while((GPIOB->IDR & 0xf000)	> 0);     //�̨���y���?��������?
	 
	 GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0200));	//????PB9????
		switch(GPIOB->IDR & 0xf000)		        
		{
			case 0x1000: KeyVal=5;	break;
			case 0x2000: KeyVal=6;	break;
			case 0x4000: KeyVal=7;	break;
			case 0x8000: KeyVal=8;	break;
		}
    while((GPIOB->IDR & 0xf000)	> 0);
		
	  GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0400));	//????PB10????
		switch(GPIOB->IDR & 0xf000)		        
		{
			case 0x1000: KeyVal=9;	break;
			case 0x2000: KeyVal=10;	break;
			case 0x4000: KeyVal=11;	break;
			case 0x8000: KeyVal=12;	break;
		}
    while((GPIOB->IDR & 0xf000)	> 0);
		
	  GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0800));	//????PB11????
		switch(GPIOB->IDR & 0xf000)		        
		{
			case 0x1000: KeyVal=13;	break;
			case 0x2000: KeyVal=14;	break;
			case 0x4000: KeyVal=15;	break;
			case 0x8000: KeyVal=16;	break;
		}
		while((GPIOB->IDR & 0xf000)	> 0);
	  
		return KeyVal;
}

/************************************
���?������?��?a��o		0  1  2  3 
							4  5  6  7
							8  9  10 11
							12 13 14 15 
************************************/
void GetKeyVal(void) 
{
	  num = KeyScan();
	  switch(num)
	  { 			  				      
				case 1: printf("1\n"); break;					  				       
				case 2: printf("2\n"); break;					  				     
				case 3: printf("3\n"); break;					  				     
				case 4: printf("A\n"); break;				 	       
				case 5: printf("4\n"); break;					  				      
				case 6: printf("5\n"); break;					  				      
				case 7: printf("6\n"); break;					 			       
				case 8: printf("B\n"); break;								 		       
				case 9: printf("7\n"); break;							 				     	
			  case 10: printf("8\n"); break;						 				      		
				case 11: printf("9\n"); break;					 				      
				case 12: printf("C\n"); break;							 				      	
			  case 13: printf("*\n"); break;							 				       	
				case 14: printf("0\n"); break;					 				      
				case 15: printf("#\n"); break;		
				case 16: printf("D\n"); break;					
    }
}

void TIM6_IRQHandler(void)   //TIM5�ж�
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		
		GetKeyVal();   //�������ɨ�躯��
		
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}
