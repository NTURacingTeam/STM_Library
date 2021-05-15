#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

volatile u16 ADCConvertedValue[10][3];

void DMA_Init123(){

	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//�ϯ����

	DMA_DeInit(DMA1_Channel1); //�N�q�D�@�H�s���]���q�{��

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (ADC1->DR);//�ӰѼƥΥH�w�qDMA�~�]��a�}

	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;//�ӰѼƥΥH�w�qDMA���s��a�}(�ഫ���G�O�s���a�})

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�ӰѼƳW�w�F�~�]�O�@���ƾڶǿ骺�ت��a�٬O�ӷ��A���B�O�@���ӷ�

	DMA_InitStructure.DMA_BufferSize = 3 * 10;//�w�q���wDMA�q�D��DMA�w�s���j�p,��쬰�ƾڳ��C�o�̤]�N�OADCConvertedValue���j�p

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�]�w�~�]�a�}�H�s�����W�P�_,���B�]������ Disable

	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�Ψӳ]�w���s�a�}�H�s�����W�P�_,���B�]�����W�AEnable

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�ƾڼe�׬�16��

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ƾڼe�׬�16��

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //�u�@�b�`���w�s�Ҧ�

	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA�q�D�֦����u���� ���O4�ӵ��� �C�B���B���B�D�`��

	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�ϯ�DMA�q�D�����s�줺�s�ǿ�

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//�ھ�DMA_InitStruct�����w���Ѽƪ�l��DMA���q�D

	DMA_Cmd(DMA1_Channel1, ENABLE);//�Ұ�DMA�q�D�@

}
void Adc_Init123(){

	ADC_InitTypeDef ADC_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

	/*3��IO�f���t�m�]PA0�BPA1�BPA2�^*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*IO�MADC�ϯ����*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

	ADC_InitStructure.ADC_ScanConvMode = ENABLE;

	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //�s���ഫ

	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

	ADC_InitStructure.ADC_NbrOfChannel = 3;

	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);//�q�D�@�ഫ���G�O�s��ADCConvertedValue[0~10][0]

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);//�q�D�G�ഫ���G�O�s��ADCConvertedValue[0~10][1]

	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);//�q�D�T�ഫ���G�O�s��ADCConvertedValue[0~10][2]

	ADC_DMACmd(ADC1, ENABLE);//�}��ADC��DMA���

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);

	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);

	while (ADC_GetCalibrationStatus(ADC1));

}

