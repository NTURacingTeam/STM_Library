#include "USART2.h"

char* USART2_Buff;
int Buff_Put_Index = 0;
int Buff_Get_Index = 0;
int Buff_Size = 0;

void USART2_Config_Buffer(int size){
	Buff_Size = size;
	USART2_Buff = (char*) malloc(sizeof(char)*size);
}

char USART2_Get_Buffer(void){
	char c = USART2_Buff[Buff_Get_Index];
	USART2_Buff[Buff_Get_Index] = '\0';
	Buff_Get_Index++;
	if(Buff_Get_Index>Buff_Size)
		Buff_Get_Index = 0;
	if(Buff_Put_Index == Buff_Get_Index){
		Buff_Get_Index--;
		if(Buff_Get_Index<0)
			Buff_Get_Index = Buff_Size;
	}

	return c;
}

void USART2_Put_Buffer(char c){
	USART2_Buff[Buff_Put_Index] = c;
	Buff_Put_Index++;
	if(Buff_Put_Index>Buff_Size)
		Buff_Put_Index = 0;
	if(Buff_Put_Index == Buff_Get_Index)
		Buff_Get_Index++;
}

void init_usart2(void)
{
	// Initialization struct
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// Step 1: Initialize GPIO for Tx and Rx pin
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Tx pin (PA2) initialization as push-pull alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Rx pin (PA3) initialization as input floating
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Step 2: Initialize USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);

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
	
	// Last but not least, enable usart2
	USART_Cmd(USART2, ENABLE);
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

#ifdef PRINTF_USE_USART2
int fputc(int ch, FILE * f){
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
  USART_SendData(USART2, (uint8_t) ch);
  return ch;
}

int fgetc(FILE * f){
	/* wait for getting data */
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)==RESET);
	return(int)USART_ReceiveData(USART2);
}
#endif
