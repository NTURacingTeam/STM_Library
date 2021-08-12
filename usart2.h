#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include <string.h>
#include "stdio.h"

GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

int fputc(int ch, FILE * f){
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
  USART_SendData(USART2, (uint8_t) ch);
  return ch;
}

void init_usart2()
{
	// Initialization struct
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// Step 1: Initialize USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);
	
	// Step 2: Initialize GPIO for Tx and Rx pin
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Tx pin (PA2) initialization as push-pull alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Rx pin (PA3) initialization as input floating
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Step 3: Enable USART receive interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	// Step 4: Initialize NVIC for USART IRQ
	// Set NVIC prority group to group 4 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// Set System Timer IRQ at higher priority
	NVIC_SetPriority(SysTick_IRQn, 0);
	// Set USART2 IRQ at lower priority
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
void USART2_PutChar(char c)
{
	// Wait until transmit data register is empty
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
	// Send a char using USART2
	USART_SendData(USART2, c);
}

void USART2_PutString(char *s)
{
	// Send a string
	while (*s)
	{
		USART2_PutChar(*s++);
	}
}