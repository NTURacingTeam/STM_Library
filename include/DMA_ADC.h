/**********************/
/*Owner: NTU FSAE TEAM*/
/*Author: 陳品堯      */
/*Editor: 黃柏瑞      */
/**********************/
#ifndef _DMA_ADC_H
#define _DMA_ADC_H

#include "stm32f10x.h"

/********************************************/
/*Function: get the memory that is connected*/
/*          to DMA1                         */
/*Variables: None                           */
/*Return: the address of an u16 array(len=3)*/
/********************************************/
u16* DMA_ADC1_get_address(void);

/********************************************/
/*Function: initialize DMA1 peripheral      */
/*Variables: None                           */
/*Return: None #TODO: return success or not */
/********************************************/
void DMA_ADC1_DMA_Init(void);

/********************************************/
/*Function: initialize ADC1 peripheral      */
/*Variables: None                           */
/*Return: None #TODO: return success or not */
/********************************************/
void DMA_ADC1_ADC_Init(void);

/********************************************/
/*Function: start DMA1 and ADC1             */
/*Variables: None                           */
/*Return: None #TODO: return success or not */
/********************************************/
void DMA_ADC1_Start(void);

#endif
