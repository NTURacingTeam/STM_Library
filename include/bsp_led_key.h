#ifndef _BSP_LED_KEY_H
#define _BSP_LED_KEY_H
	
	#include "stm32f10x.h"
	#include "usart1.h"
	
	#define ON 0
	#define OFF 1
	
	void LEDA8(int a);
	void LEDA9(int a);
	void LEDE5(int a);
	void LED_GPIO_Config(void);
	void KEY_GPIO_Config(void);
	uint8_t keyscan(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
#endif
