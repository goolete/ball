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
						 display_number_16x8(1,2,Kp); 				  				       
						 display_number_16x8(1,50,Ki); 				  				     
						 display_number_16x8(1,90,Kd); 				  				     
						 display_number_16x8(3,2,Kpp);				 	       
						 display_number_16x8(3,50,Kip);					  				      
						 display_number_16x8(3,90,Kdp);	
			//clear_screen();			//清屏
			//disp_string_8x16_16x16(1,1,"转速给定:");//显示16x16点阵汉字串或8x16点阵的字符串，括号里的参数分别为（页,列,字符串指针）
			//display_number_16x8(1,73,2900);
			//display_graphic_32x32 (1,1+32*3,hanzi1); 					//显示单个32x32点阵的汉字，括号里的参数分别为（PAGE,列,字符指针）
			


			//delay_ms(10); //20HZ

		} 

 }

 