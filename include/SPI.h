/**********************/
/*Owner: NTU FSAE TEAM*/
/*Author: 黃柏瑞      */
/*Editor: 黃柏瑞      */
/**********************/

#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"

/**********************************************************/
/*Function: initialize SPI peripheral                     */
/*Variables: trans_mode: SPI_Mode_Master or SPI_Mode_Slave*/
/*           clock_mode: 0/1/2/3                          */
/*           mode | CPOL | CPHA                           */
/*              0 |    0 |    0                           */
/*              1 |    0 |    1                           */
/*              2 |    1 |    0                           */
/*              3 |    1 |    1                           */
/*           prescaler: prescaler for fPCLK(should be 72M)*/
/*           if SPI_BaudRatePrescaler_2                   */
/*           then SPI baudrate = 36M                      */
/*           example:                                     */
/*SPI_Config(SPI_Mode_Master, 0, SPI_BaudRatePrescaler_2);*/
/*Return: None #TODO: return success or not               */
/**********************************************************/
void SPI_Config(uint16_t trans_mode, uint16_t clock_mode, uint16_t prescaler);

void SPI_Interrupt_Config(void);

uint8_t SPI_Send(uint8_t TxData);

uint8_t SPI_Receive(void);

#endif
