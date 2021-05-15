#include "PWM.h"
#include "usart1.h"
//arr: automatic refill
//psc: prescaler value
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// Enable TIM3 clock
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //Enable GPIO clock
	                                                                     	

   //Set up PA6 as PP output
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                                                       
	printf("GPIO \r\n");
	
	//Set up Timer 1
	TIM_TimeBaseStructure.TIM_Period = arr; //how many count before reset	 80K
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //prescaler
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM upward count
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //Initialize Timer 1
	printf("Timer \r\n");

	//Set up PWM mode
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //select mode
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //compare and output
	TIM_OCInitStructure.TIM_Pulse = 0; //
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //Initialize OC 1
	printf("PWM \r\n");
	
	//TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //
	
	TIM_Cmd(TIM3, ENABLE);  //
	printf("Done \r\n");
 }
