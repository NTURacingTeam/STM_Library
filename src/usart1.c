#include "usart1.h"

void USART1_config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/*start up the clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/*config PA9(Tx) & PA10(Rx)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*config usart1*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx| USART_Mode_Tx; 
	USART_Init(USART1, &USART_InitStructure);
	
	/*start usart1 with the configuration we just "init"*/
	USART_Cmd(USART1,ENABLE);
}

void USART1_DMA_config(char * SendBuff, int size){
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	/*start up the clock*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/*config the DMA*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;//should be (u32)&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) SendBuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	/*start dma1 with the configuration we just "init"*/
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

int USART1_PutChar(int ch){
	USART_SendData(USART1, (uint8_t) ch);
	//wait for sending data
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	return ch;
}

void USART1_PutString(char* str, int size){
	for(int i=0;i<size;i++){
		USART1_PutChar((int) str[i]);
	}
}

int usart_receive(void){
	//wait for receiving data
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	return (int) USART_ReceiveData(USART1);
}

#ifdef PRINTF_USE_USART1
int fputc(int ch, FILE * f){
	/* Send a byte to the USART */
	USART_SendData(USART1, (uint8_t) ch);
	/* wait for sending completion */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
	return(ch);
}

int fgetc(FILE * f){
	/* wait for getting data */
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET);
	return(int)USART_ReceiveData(USART1);
}
#endif
