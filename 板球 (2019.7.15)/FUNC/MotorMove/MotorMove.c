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

float Kp,Ki,Kd;
float Kpp,Kip,Kdp;



extern u8 Urxbuf[4];
int mode_select=0;
int light1 = 0;
int light2 = 0;



//#define  PWM_MAX_X     1700
//#define  PWM_MIN_X     1350
//#define  PWM_MAX_Y     1700
//#define  PWM_MIN_Y     1350

#define  PWM_X_initial  1530
#define  PWM_Y_initial  1430
#define  PWM_M  150
#define  SumError_MAX   20000
#define  SumError_MIN   -20000
#define  SumError_P_MAX   20000
#define  SumError_P_MIN   -20000


void Motors_move(float set_X,float set_Y)
{
	static uint32_t Count = 0;	
	static float t = 0;	
	u8 X,XP,Y,YP;

	
	Kp = 5.5;
	Ki = 0.0;
	Kd = 30;
	
	Kpp = 0.0;
	Kip = 0.0;
	Kdp = 0.0;
	
	X=Urxbuf[0];
	Y=Urxbuf[1];
	XP=Urxbuf[2];
	YP=Urxbuf[3];

//	t+=0.05;
//	if(t>=3.14)  t=-3.14; 
	
	/********** X方向PID控制 ***************************/
	Error_X = X - set_X;         											//位置环
	SumError_X += Error_X;				          		
	DError_X = Error_X - LastError_X; 			   	
	LastError_X = Error_X;
	if(SumError_X>=SumError_MAX) SumError_X=SumError_MAX;
	else if(SumError_X<=SumError_MIN) SumError_X=SumError_MIN;
	PWM_X=(int32_t)(Kp* Error_X + Ki* SumError_X + Kd* DError_X);

	Error_XP = XP-0;         					 									//速度环
	SumError_XP += Error_XP;				          	
	DError_XP = Error_XP - LastError_XP; 			  
	LastError_XP = Error_XP;
	if(SumError_XP>=SumError_P_MAX) SumError_XP=SumError_P_MAX;
	else if(SumError_XP<=SumError_P_MIN) SumError_XP=SumError_P_MIN;
	PWM_XP=(int32_t)(Kpp* Error_XP + Kip* SumError_XP + Kdp* DError_XP);
		
	PWM_X_Sum = PWM_X_initial + PWM_X + PWM_XP;
	if(PWM_X_Sum>=PWM_X_initial+PWM_M) PWM_X_Sum=PWM_X_initial+PWM_M;
	else if(PWM_X_Sum<=PWM_X_initial-PWM_M) PWM_X_Sum=PWM_X_initial-PWM_M;
	
	PWM_X_Sum=PWM_X_initial;
	TIM_SetCompare3(TIM3,PWM_X_Sum);	

	
	/********** Y方向PID控制 ***************************/
	Error_Y = set_Y -Y ;       						  						//位置环		
	SumError_Y += Error_Y;				          		
	DError_Y = Error_Y - LastError_Y; 			   	
	LastError_Y = Error_Y;
	if(SumError_Y>=SumError_MAX) SumError_Y=SumError_MAX;
	else if(SumError_Y<=SumError_MIN) SumError_Y=SumError_MIN;
	PWM_Y=(int32_t)(Kp* Error_Y + Ki  * SumError_Y + Kd* DError_Y);  //PI=0.00001

	Error_YP = YP-0;         					 										//速度环
	SumError_YP += Error_YP;				          	
	DError_YP = Error_YP - LastError_YP; 			  
	LastError_YP = Error_YP;
	if(SumError_YP>=SumError_P_MAX) SumError_YP=SumError_P_MAX;
	else if(SumError_YP<=SumError_P_MIN) SumError_YP=SumError_P_MIN;
	PWM_YP=(int32_t)(Kpp* Error_YP + Kip* SumError_YP + Kdp* DError_YP);
		
	PWM_Y_Sum = PWM_Y_initial + PWM_Y + PWM_YP;
	if(PWM_Y_Sum>=PWM_Y_initial+PWM_M) PWM_Y_Sum=PWM_Y_initial+PWM_M;
	else if(PWM_Y_Sum<=PWM_Y_initial-PWM_M) PWM_Y_Sum=PWM_Y_initial-PWM_M;

//	PWM_Y_Sum=PWM_Y_initial - PWM_M;
	TIM_SetCompare4(TIM3,PWM_Y_Sum);	

//	GPIO_ResetBits(GPIOA,GPIO_Pin_8);

 /*
	if((fabs(Error_Y)<1.0)&&(fabs(Error_X)<1.0))					//判断是否稳定在设定点，稳定后给出绿灯常亮提示		
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
*/
}



void Mode_1()
{
		static float set_X,set_Y;
		float set_X1,set_X2,set_Y1,set_Y2;
		const float priod = 5000;  					//直线运动时长(毫秒)
		static uint32_t MoveTimeCnt = 0;
		static uint32_t Count = 0;
		static int A =0;
		set_X1=116;
		set_Y1=205;
		set_X2=29;
		set_Y2=121;
		
		set_X = set_X1;
		set_Y = set_Y1;

	
	if(set_Y==set_Y1)										//判断是否稳定在直线起始位置，稳定后给出红灯常亮提示
			{	
				if((fabs(Error_Y)<10)&&(fabs(Error_X)<10))
				{
					Count++;
					if(Count>=100)		
					{
						light1 = 0;
						light2 = 1;
					}
					if(Count>=300)		
					{
						A=1;
						Count = 300;
					}
				}
				else  
				{				
					light1 = 0;
					light2 = 0;
					Count = 0;
				}
			}
			else 	light2 = 0;

	/*		
				
			if(A) 															//稳定在直线起始位置后，设定到达终止位置的线性变化目标函数
			{	
			if(MoveTimeCnt >= priod)	MoveTimeCnt = priod;	
			set_Y= set_Y1 + (set_Y2 - set_Y1)*MoveTimeCnt/priod;
			set_X= set_X1 + (set_X2 - set_X1)*MoveTimeCnt/priod;			
			MoveTimeCnt += 10;
			}
			
			if(set_Y==set_Y2)									//判断是否稳定在直线终止位置，稳定后给出绿灯常亮提示
			{	
				if((fabs(Error_Y)<10)&&(fabs(Error_X)<10))
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
			else 	light1 = 0;
*/
			
	
		Motors_move(set_X,set_Y);			
			
			
			
			
}

void TIM5_IRQHandler(void)   //TIM5中断
{

	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			
		switch(mode_select)	//根据题目选择函数
		{	
			case 1: Mode_1(); break;

			
			default:break;
		}


		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
}

