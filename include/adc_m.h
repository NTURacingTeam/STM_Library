#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

volatile u16 ADCConvertedValue[10][3];

void DMA_Init123(){

	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能時鐘

	DMA_DeInit(DMA1_Channel1); //將通道一寄存器設為默認值

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (ADC1->DR);//該參數用以定義DMA外設基地址

	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;//該參數用以定義DMA內存基地址(轉換結果保存的地址)

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//該參數規定了外設是作為數據傳輸的目的地還是來源，此處是作為來源

	DMA_InitStructure.DMA_BufferSize = 3 * 10;//定義指定DMA通道的DMA緩存的大小,單位為數據單位。這裡也就是ADCConvertedValue的大小

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//設定外設地址寄存器遞增與否,此處設為不變 Disable

	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//用來設定內存地址寄存器遞增與否,此處設為遞增，Enable

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//數據寬度為16位

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//數據寬度為16位

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循環緩存模式

	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道擁有高優先級 分別4個等級 低、中、高、非常高

	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//使能DMA通道的內存到內存傳輸

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//根據DMA_InitStruct中指定的參數初始化DMA的通道

	DMA_Cmd(DMA1_Channel1, ENABLE);//啟動DMA通道一

}
void Adc_Init123(){

	ADC_InitTypeDef ADC_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

	/*3個IO口的配置（PA0、PA1、PA2）*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*IO和ADC使能時鐘*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

	ADC_InitStructure.ADC_ScanConvMode = ENABLE;

	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //連續轉換

	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

	ADC_InitStructure.ADC_NbrOfChannel = 3;

	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);//通道一轉換結果保存到ADCConvertedValue[0~10][0]

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);//通道二轉換結果保存到ADCConvertedValue[0~10][1]

	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);//通道三轉換結果保存到ADCConvertedValue[0~10][2]

	ADC_DMACmd(ADC1, ENABLE);//開啟ADC的DMA支持

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);

	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);

	while (ADC_GetCalibrationStatus(ADC1));

}

