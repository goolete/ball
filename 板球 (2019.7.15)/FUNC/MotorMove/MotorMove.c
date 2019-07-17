#include "stm32f10x.h"
#include "string.h"
#include "math.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "DataScope_DP.h"
#include "exti.h"
#include "init.h"

float Error_X,SumError_X,DError_X,LastError_X=0;
float Error_XP,SumError_XP,DError_XP,LastError_XP=0;

float Error_Y,SumError_Y,DError_Y,LastError_Y=0;
float Error_YP,SumError_YP,DError_YP,LastError_YP=0;

int32_t PWM_X,PWM_XP,PWM_X_Sum;
int32_t PWM_Y,PWM_YP,PWM_Y_Sum;	


extern u8 Urxbuf[2];
int mode_select=0;
int light1 = 0;
int light2 = 0;



#define  PWM_MAX     1800
#define  PWM_MIN     1200
#define  PWM_X_initial  1550
#define  PWM_Y_initial  1450
#define  SumError_MAX   20000
#define  SumError_MIN   -20000
#define  SumError_P_MAX   20000
#define  SumError_P_MIN   -20000


void Motors_move(float set_X,float set_Y)
{

	
	static uint32_t Count = 0;	
	static float t = 0;	
	u8 X,XP,Y,YP;

	X=Urxbuf[0];
	Y=Urxbuf[1];

//	t+=0.05;
//	if(t>=3.14)  t=-3.14; 
	
	/********** X����PID���� ***************************/
	Error_X = X - set_X;         											//λ�û�
	SumError_X += Error_X;				          		
	DError_X = Error_X - LastError_X; 			   	
	LastError_X = Error_X;
	if(SumError_X>=SumError_MAX) SumError_X=SumError_MAX;
	else if(SumError_X<=SumError_MIN) SumError_X=SumError_MIN;
	PWM_X=(int32_t)(10* Error_X + 0.0* SumError_X + 0.0* DError_X);

	Error_XP = XP-0;         					 									//�ٶȻ�
	SumError_XP += Error_XP;				          	
	DError_XP = Error_XP - LastError_XP; 			  
	LastError_XP = Error_XP;
	if(SumError_XP>=SumError_P_MAX) SumError_XP=SumError_P_MAX;
	else if(SumError_XP<=SumError_P_MIN) SumError_XP=SumError_P_MIN;
	PWM_XP=(int32_t)(0.0* Error_XP + 0.0* SumError_XP + 0.0* DError_XP);
		
	PWM_X_Sum = PWM_X_initial + PWM_X + PWM_XP;
	if(PWM_X_Sum>=PWM_MAX) PWM_X_Sum=PWM_MAX;
	else if(PWM_X_Sum<=PWM_MIN) PWM_X_Sum=PWM_MIN;
	
	PWM_X_Sum=PWM_X_initial;
	TIM_SetCompare3(TIM3,PWM_X_Sum);	

	
	/********** Y����PID���� ***************************/
	Error_Y = set_Y -Y ;       						  						//λ�û�		
	SumError_Y += Error_Y;				          		
	DError_Y = Error_Y - LastError_Y; 			   	
	LastError_Y = Error_Y;
	if(SumError_Y>=SumError_MAX) SumError_Y=SumError_MAX;
	else if(SumError_Y<=SumError_MIN) SumError_Y=SumError_MIN;
	PWM_Y=(int32_t)(0* Error_Y + 0.0* SumError_Y + 0* DError_Y);

	Error_YP = YP-0;         					 										//�ٶȻ�
	SumError_YP += Error_YP;				          	
	DError_YP = Error_YP - LastError_YP; 			  
	LastError_YP = Error_YP;
	if(SumError_YP>=SumError_P_MAX) SumError_YP=SumError_P_MAX;
	else if(SumError_YP<=SumError_P_MIN) SumError_YP=SumError_P_MIN;
	PWM_YP=(int32_t)(0.0* Error_YP + 0.0* SumError_YP + 0.0* DError_YP);
		
	PWM_Y_Sum = PWM_Y_initial + PWM_Y + PWM_YP;
	if(PWM_Y_Sum>=PWM_MAX) PWM_Y_Sum=PWM_MAX;
	else if(PWM_Y_Sum<=PWM_MIN) PWM_Y_Sum=PWM_MIN;

//	PWM_Y_Sum=PWM_Y_initial;
	TIM_SetCompare4(TIM3,PWM_Y_Sum);	



 
	if((fabs(Error_Y)<1.0)&&(fabs(Error_X)<1.0))					//�ж��Ƿ��ȶ����趨�㣬�ȶ�������̵Ƴ�����ʾ		
	{
		Count++;
		if(Count>=100)		
		{
			light1 = 1;
			light2 = 0;
			Count = 100;
		}
	}
	else  
	{				
		light1 = 0;
		light2 = 0;
		Count = 0;
	}

}



void Mode_1()
{
		static float set_X,set_Y;
		set_X=121;
		set_Y=117;
		Motors_move(set_X,set_Y);			
			
			
			
			
}

void TIM5_IRQHandler(void)   //TIM5�ж�
{

	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			
		switch(mode_select)	//������Ŀѡ����
		{	
			case 1: Mode_1(); break;

			
			default:break;
		}


		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}

