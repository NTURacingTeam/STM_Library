#ifndef _GPIO_H
#define _GPIO_H

#include "stm32f10x.h"
#include "misc.h"
#include "gpio.h"
#include "stm32f10x_exti.h"


void EXTI_Config(){
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3); // 管腳選擇
	EXTI_ClearITPendingBit(EXTI_Line3); 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  //此行應該在 main ???
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn; //通道
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void init_exti(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	init_gpio_write(GPIOA, GPIO_Pin_5);
	init_gpio_read(GPIOC, GPIO_Pin_3);
	EXTI_Config();
	NVIC_Config();
}

#endif