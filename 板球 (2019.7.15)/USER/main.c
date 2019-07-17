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
#include "oled.h"

unsigned char i;          
unsigned char Send_Count; 
extern u8 Urxbuf[2];
extern float Error_X,SumError_X,DError_X;
extern int32_t PWM_X;
extern float Kp,Ki,Kd;
extern float Kpp,Kip,Kdp;
extern int num;
	
 int main(void)
 {	
	 u8 lcd_id[12];			//存放LCD ID字符串	

	 delay_init();	    	 //延时函数初始化
	 System_Init();
	 Key_Config();
	 OLED_Init();		    //初始化OLED
	 initial_olcd();		  //初始化
	 clear_screen();			//清屏
	 while(1)
		{
			switch(num)
				{ 
						case 0: display_number_16x8(2,1,Kp); break;					  				       
						case 1: display_number_16x8(2,1,Ki); break;					  				     
						case 2: display_number_16x8(2,1,Kd); break;					  				     
						case 3: display_number_16x8(2,1,Kpp); break;				 	       
						case 4: display_number_16x8(2,1,Kip); break;					  				      
						case 5: display_number_16x8(2,1,Kdp); break;					  				      
						case 6: display_number_16x8(2,1,7); break;					 			       
						case 7: display_number_16x8(2,1,8); break;								 		       
						case 8: display_number_16x8(2,1,9); break;							 				     	
						case 9: display_number_16x8(2,1,10); break;						 				      		
						case 10: display_number_16x8(2,1,11); break;					 				      
						case 11: display_number_16x8(2,1,12); break;							 				      	
						case 12: display_number_16x8(2,1,13); break;							 				       	
						case 13: display_number_16x8(2,1,14); break;					 				      
						case 14: display_number_16x8(2,1,15); break;		
						case 15: display_number_16x8(2,1,16); break;	
						default: break;				      
				}
				
			//clear_screen();			//清屏
			//disp_string_8x16_16x16(1,1,"转速给定:");//显示16x16点阵汉字串或8x16点阵的字符串，括号里的参数分别为（页,列,字符串指针）
			//display_number_16x8(1,73,2900);
			//display_graphic_32x32 (1,1+32*3,hanzi1); 					//显示单个32x32点阵的汉字，括号里的参数分别为（PAGE,列,字符指针）
			
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

			delay_ms(10); //20HZ

		} 

 }

 