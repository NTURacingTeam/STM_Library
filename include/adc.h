#ifndef _ADC_H
#define _ADC_H

	#include "stm32f10x.h"
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_adc.h"

	static void ADC1_GPIO_Config(void);
	static void ADC1_Mode_Config(void);
	void ADC1_Init(void);
	
#endif
