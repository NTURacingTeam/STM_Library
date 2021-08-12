#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStruct;

void init_gpio_read(GPIO_TypeDef * gpiox, uint16_t pinRead){
	
	uint32_t RCC_APB2Periph_GPIOX;
	if(gpiox == GPIOA){		RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOA;	}
	else if( gpiox == GPIOB ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOB; }	
	else if( gpiox == GPIOC ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOC; }
	else if( gpiox == GPIOD ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOD; }		
	else if( gpiox == GPIOE ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOE; }
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);

	GPIO_InitStruct.GPIO_Pin = pinRead;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(gpiox, &GPIO_InitStruct);
	
}
void init_gpio_write(GPIO_TypeDef * gpiox, uint16_t pinWrite){
	
	uint32_t RCC_APB2Periph_GPIOX;
	if(gpiox == GPIOA){		RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOA;	}
	else if( gpiox == GPIOB ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOB; }	
	else if( gpiox == GPIOC ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOC; }
	else if( gpiox == GPIOD ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOD; }		
	else if( gpiox == GPIOE ){ RCC_APB2Periph_GPIOX = RCC_APB2Periph_GPIOE; }	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = pinWrite;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(gpiox, &GPIO_InitStruct);
	
}
