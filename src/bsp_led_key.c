#include "bsp_led_key.h"

#define KEY_ON 1
#define KEY_OFF 0

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);//it's E here
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//input pull up
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void LEDA8(int a){
	if (a == ON){
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		//usart_print_string("\r\n led on \r\n",12);
	}
	else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		//usart_print_string("\r\n led off \r\n",13);
	}
}

void LEDA9(int a){
	if (a == ON){
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		//usart_print_string("\r\n led on \r\n",12);
	}
	else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		//usart_print_string("\r\n led off \r\n",13);
	}
}

void LEDE5(int a){
	if (a == ON){
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
		//usart_print_string("\r\n led on \r\n",12);
	}
	else{
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		//usart_print_string("\r\n led off \r\n",13);
	}
}

uint8_t keyscan(GPIO_TypeDef* GPIOx, u16 GPIO_Pin){
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
		return KEY_ON;
	else
		return KEY_OFF;
}
