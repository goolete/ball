#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "MotorMove.h"
#include "init.h"
#include "led.h"
#include "DataScope_DP.h"
#include "exti.h"
#include "key.h"
#include "math.h"
#include "MKEY.h"
#include "lcd.h"

unsigned char i;          
unsigned char Send_Count; 
extern u8 Urxbuf[2];
extern float Error_X,SumError_X,DError_X;
extern int32_t PWM_X;

 int main(void)
 {	

	 delay_init();	    	 //ÑÓÊ±º¯Êý³õÊ¼»¯
	 System_Init();
	 Key_Config();
	 
	 while(1)
		{
			GetKeyVal();   //¾ØÕó¼üÅÌÉ¨Ãèº¯Êý
			
//			a+=0.1;
//			if(a>=3.14)  a=-3.14; 
//			DataScope_Get_Channel_Data(sin(a), 1 );
//			DataScope_Get_Channel_Data(cos(a), 2 );
//			DataScope_Get_Channel_Data(Urxbuf[0] , 3 ); 
//			DataScope_Get_Channel_Data(Urxbuf[1] , 4 );   
//			DataScope_Get_Channel_Data(Error_X , 5 );
//			DataScope_Get_Channel_Data(SumError_X , 6 );
//			DataScope_Get_Channel_Data(DError_X , 7 );
//			DataScope_Get_Channel_Data(PWM_X , 8 ); 
//			DataScope_Get_Channel_Data(0 , 9 );  
//			DataScope_Get_Channel_Data(0 , 10);
//			Send_Count = DataScope_Data_Generate(10);
//			for( i = 0 ; i < Send_Count; i++) 
//			{
//			while((USART1->SR&0X40)==0);  
//			USART1->DR = DataScope_OutPut_Buffer[i]; 
//			}
//			delay_ms(10); //20HZ
		} 

 }

 