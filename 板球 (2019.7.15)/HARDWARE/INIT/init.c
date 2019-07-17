#include "stm32f10x_flash.h"
#include "string.h"
#include "sys.h"
#include "math.h"
#include "delay.h"
#include "led.h"
#include "init.h"
#include "sys.h"
#include "exti.h"
#include "key.h"
#include "MotorMove.h"

//#define  PWM_START   50.0

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;                                       //�����ⲿ���پ�������״̬ö�ٱ���

    RCC_DeInit();                                                       //��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
    RCC_HSEConfig(RCC_HSE_ON);                                          //���ⲿ���پ���
    HSEStartUpStatus = RCC_WaitForHSEStartUp();                         //�ȴ��ⲿ����ʱ��׼����
    if(HSEStartUpStatus == SUCCESS)                                     //�ⲿ����ʱ���Ѿ�׼���
    {
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);           //����FLASHԤ�����幦��
        FLASH_SetLatency(FLASH_Latency_2);                              //flash��������ʱ

        RCC_HCLKConfig(RCC_SYSCLK_Div1);                                //����AHB(HCLK)ʱ�ӵ���==SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);                                 //����APB2(PCLK2)��==AHBʱ��
        RCC_PCLK1Config(RCC_HCLK_Div2);                                 //����APB1(PCLK1)��==AHB1/2ʱ��

        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);            //����PLLCLK == �ⲿ���پ���ʱ�� * 9 = 72MHz
        RCC_PLLCmd(ENABLE);                                             //ʹ��PLLʱ��

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)              //�ȴ�PLLʱ�Ӿ���
        {
        }
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                      //����ϵͳʱ�� = PLLʱ��
        while(RCC_GetSYSCLKSource() != 0x08)                            //���PLLʱ���Ƿ���Ϊϵͳʱ��
        {
        }
    }
}

void My_USART1_Init(void)					//PA9��PA10   �������
{
	 GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_DeInit(USART1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					//����������ж����ã�������Ӧ����  ������
    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);
}

void My_USART2_Init(void)					//PA2��PA3  OpenMV����
{
	  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��UGPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART3ʱ��
	//USART2_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);     //��ʼ������3
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���3 
}




void TIM3_PWM_Init(u16 arr,u16 psc)                           // PWM���  TIM3_CH3��CH4��PB0��PB1)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	

		TIM_TimeBaseStructure.TIM_Period = arr; 
		TIM_TimeBaseStructure.TIM_Prescaler =psc; 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 


		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE �����ʹ��	

		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	 
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	 
		TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	}

	
	
void TIM4_Interrupt_Config(void)                                                  //���ڵƹ�Ŀ���(10ms)   ��ʱ���ж� TIM4
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10000;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM4, ENABLE);
}

void TIM5_Interrupt_Config(void)                                                  //���ڶ����ڵ�PID����(10ms)			��ʱ���ж� TIM5
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10000;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM5,TIM_FLAG_Update);
    TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM5, ENABLE);
}

void TIM6_Interrupt_Config(void)                                                  //���ھ������ˢ��(10ms)			��ʱ���ж� TIM5
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10000;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM6,TIM_FLAG_Update);
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM6, ENABLE);
}


/*************************************************************************************************************************/
void NVIC_Configuration(void)                                           //�����ж����ȼ�  
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //OpenMV ���ݽ������ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //PID���㶨ʱ�ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //�ƹⶨʱ�ж����ȼ�	
   	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		//�������ˢ���ж����ȼ�	
   	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void System_Init(void)
{

    RCC_Configuration();
		NVIC_Configuration();
		My_USART1_Init();
		My_USART2_Init();
	
		LED_Init();
		KEY_Init();
		EXTIX_Init();
		TIM3_PWM_Init(19999,72);		//50Hz
		TIM4_Interrupt_Config();
		TIM5_Interrupt_Config();
		TIM6_Interrupt_Config();

}


