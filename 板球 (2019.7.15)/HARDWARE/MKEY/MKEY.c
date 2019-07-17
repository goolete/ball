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
** PB5-PB8,DD￡?ê?3??￡ PB10-PB14,áD￡?ê?è?
**/
void Key_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;   
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   
	 /********4DDê?3?*********/
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	 /********4áDê?è?*********/
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

int KeyScan(void)
{
   uchar KeyVal;
	
   GPIO_Write(GPIOB, (GPIOB->ODR & 0xf0ff | 0x0f00));	 //2?1ü??óà12??0xf0ff￡??èè?PB8μ?PB11è?2?ê?3????￡
	  
	 if((GPIOB->IDR & 0xf000)==0x0000)   								 //è?1?PB12μ?PB15è??aá??ò??óD°′?ü°′??
		  return -1;
	 else
	 {	
	    delay_ms(5);    //?óê±5msè￥???ˉ
	    if((GPIOB->IDR & 0xf000)==0x0000)
	    return -1;
	 }
	 
	 GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0100));	//????PB8????	 
	 switch(GPIOB->IDR & 0xf000)	//μúò?DD￡?óéμíμ???￡?í?àí￡?2?×?ê?
	 {
     	case 0x1000: KeyVal=1; break;
			case 0x2000: KeyVal=2;	break;
			case 0x4000: KeyVal=3;	break;
			case 0x8000: KeyVal=4;	break;
   }
	 while((GPIOB->IDR & 0xf000)	> 0);     //μè′y°′?üêí·?
	 
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
°′?ü±í?ì?a￡o		0  1  2  3 
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

void TIM6_IRQHandler(void)   //TIM5中断
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		
		GetKeyVal();   //矩阵键盘扫描函数
		
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
}
