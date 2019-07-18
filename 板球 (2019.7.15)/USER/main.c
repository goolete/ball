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
	 u8 lcd_id[12];			//���LCD ID�ַ���	

	 delay_init();	    	 //��ʱ������ʼ��
	 System_Init();
	 Key_Config();
	 OLED_Init();		    //��ʼ��OLED
	 initial_olcd();		  //��ʼ��
	 clear_screen();			//����
	 while(1)
		{
						 display_number_16x8(1,2,Kp); 				  				       
						 display_number_16x8(1,50,Ki); 				  				     
						 display_number_16x8(1,90,Kd); 				  				     
						 display_number_16x8(3,2,Kpp);				 	       
						 display_number_16x8(3,50,Kip);					  				      
						 display_number_16x8(3,90,Kdp);	
			//clear_screen();			//����
			//disp_string_8x16_16x16(1,1,"ת�ٸ���:");//��ʾ16x16�����ִ���8x16������ַ�����������Ĳ����ֱ�Ϊ��ҳ,��,�ַ���ָ�룩
			//display_number_16x8(1,73,2900);
			//display_graphic_32x32 (1,1+32*3,hanzi1); 					//��ʾ����32x32����ĺ��֣�������Ĳ����ֱ�Ϊ��PAGE,��,�ַ�ָ�룩
			


			//delay_ms(10); //20HZ

		} 

 }

 