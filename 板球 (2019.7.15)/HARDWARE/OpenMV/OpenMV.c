#include "stm32f10x.h"
#include "string.h"
#include "OpenMV.h"
int X,XP,Y,YP;
u8 Urxbuf[4],Usart_Flag=0;

void USART2_IRQHandler(void)
{	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	  
		u16 temp;
				static u8 count,last_data,last_last_data;
				temp=USART2->DR;
				   if(Usart_Flag==0)
						{	
						if(last_data==0xfe &&last_last_data==0xff) 
						Usart_Flag=1,count=0;	
						}
					 if(Usart_Flag==1)
						{	
							Urxbuf[count]=temp;     
							count++;                
							if(count==4)Usart_Flag=0;
						}
						last_last_data=last_data;
						last_data=temp;
		X=Urxbuf[0]+Urxbuf[1];
		Y=Urxbuf[2]+Urxbuf[3];

	}  											 
} 
