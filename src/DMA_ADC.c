#include "DMA_ADC.h"
#define ADC1_DR_Address ((u32)0x40012400+0x4c)
u16 ADCConvertedValue[3];

void DMA_ADC1_DMA_Init(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//¨Ï¯à®ÉÄÁ
	//DMA_DeInit(DMA1_Channel1); //±N³q¹D¤@±H¦s¾¹³]¬°Àq»{­È
	DMA_Cmd(DMA1_Channel1, DISABLE);	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//(uint32_t) ADC1->DR;//¸Ó°Ñ¼Æ¥Î¥H©w¸qDMA¥~³]°ò¦a§}
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ADCConvertedValue;//¸Ó°Ñ¼Æ¥Î¥H©w¸qDMA¤º¦s°ò¦a§}(Âà´«µ²ªG«O¦sªº¦a§})
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//¸Ó°Ñ¼Æ³W©w¤F¥~³]¬O§@¬°¼Æ¾Ú¶Ç¿éªº¥Øªº¦aÁÙ¬O¨Ó·½¡A¦¹³B¬O§@¬°¨Ó·½
	DMA_InitStructure.DMA_BufferSize = 3;//©w¸q«ü©wDMA³q¹DªºDMA½w¦sªº¤j¤p,³æ¦ì¬°¼Æ¾Ú³æ¦ì¡C³o¸Ì¤]´N¬OADCConvertedValueªº¤j¤p
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//³]©w¥~³]¦a§}±H¦s¾¹»¼¼W»P§_,¦¹³B³]¬°¤£ÅÜ Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//¥Î¨Ó³]©w¤º¦s¦a§}±H¦s¾¹»¼¼W»P§_,¦¹³B³]¬°»¼¼W¡AEnable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//¼Æ¾Ú¼e«×¬°16¦ì
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//¼Æ¾Ú¼e«×¬°16¦ì
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //¤u§@¦b´`Àô½w¦s¼Ò¦¡
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA³q¹D¾Ö¦³°ªÀu¥ý¯Å ¤À§O4­Óµ¥¯Å §C¡B¤¤¡B°ª¡B«D±`°ª
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//¨Ï¯àDMA³q¹Dªº¤º¦s¨ì¤º¦s¶Ç¿é
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//®Ú¾ÚDMA_InitStruct¤¤«ü©wªº°Ñ¼Æªì©l¤ÆDMAªº³q¹D
}

void DMA_ADC1_ADC_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/*3­ÓIO¤fªº°t¸m¡]PA0¡BPA1¡BPA2¡^*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*IO©MADC¨Ï¯à®ÉÄÁ*/
	//ADC cycle should be 12MHz (72/6)
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //³sÄòÂà´«
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 3;
	ADC_Init(ADC1, &ADC_InitStructure);
	//It should take 12+240 ADC cycles to complete a sample (21 us)
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
	ADC_DMACmd(ADC1, ENABLE);//¶}±ÒADCªºDMA¤ä«ù
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));
}

void DMA_ADC1_Start(){
	DMA_ADC1_DMA_Init();
	DMA_ADC1_ADC_Init();
	DMA_Cmd(DMA1_Channel1, ENABLE);//±Ò°ÊDMA³q¹D¤@
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u16* DMA_ADC1_get_address(){
	return ADCConvertedValue;
}
