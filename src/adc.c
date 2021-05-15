#include "adc.h"
#define ADC1_DR_Address ((u32)0x40012400+0x4c)
__IO uint16_t ADC_ConvertedValue;

static void ADC1_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable DMA Clock*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* ENABLE ADC1 and GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA, ENABLE);
	/* Configure PC.01 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	/* PC1, No need to set up the GPIO speed when being input */
}

static void ADC1_Mode_Config(void){
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/* DMA Channel Configuration */
	DMA_DeInit(DMA1_Channel1);
	/* ADC Address */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	/* Memory Address */
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) &ADC_ConvertedValue;
	/* DMA direction from outside to inside */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/* Buffer Size */
	DMA_InitStructure.DMA_BufferSize = 1;
	/* Peripheral and memory address is fixed */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	/* Half Word Size */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	/* Circular Transmission Mode */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	/* Priority High */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* Memory to Memory */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/* Initialization */
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA Channel 1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC configuration*/
	/* Indipendent ADC Mode */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	/* No scan mode for only one ADC gateway */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	/* ADC non stop conversion */
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/* No External Trigger */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/* Data Alignment */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/* # of gateway */
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	/* Initialization */
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/* Set up ADC clock, here is 9MHz.
	(72/8 = 9, and for ADC, its clock must not be bigger than 14 MHz) */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	/* Set up ADC 11th gateway to 55.5 stm's sampling period */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	/*  Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* ENABLE ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* Reset Calibration register */
	ADC_ResetCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	/* Calibrate */
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	/* Start */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC1_Init(void){
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}
