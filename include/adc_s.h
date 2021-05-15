#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"

void init_adc1_s()
{
  // Initialization struct
	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 1: Initialize ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 3;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);
	
	// Step 2: Initialize GPIOA (PA0)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint16_t ADC1_Read_s(int c)
{
	uint16_t ch;
	switch(c){
		case 0:
			ch = ADC_Channel_0;
		  break;
		case 1:
			ch = ADC_Channel_1;
		  break;
		case 2:
			ch = ADC_Channel_2;
		  break;
		case 3:
			ch = ADC_Channel_3;
		  break;
		case 4:
			ch = ADC_Channel_4;
		  break;
		case 5:
			ch = ADC_Channel_5;
		  break;
		case 17:
			ch = ADC_Channel_17;
		  break;
	}	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_7Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); // Start ADC conversion
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // Wait until ADC conversion finished
	return ADC_GetConversionValue(ADC1);
}
