/**********************/
/*Owner: NTU FSAE TEAM*/
/*Author: 黃柏瑞      */
/*Editor: 黃柏瑞      */
/**********************/

#ifndef _USART1_H
#define _USART1_H

#include "stm32f10x.h"
#include "stdio.h"

/********************************************/
/*Function: initialize USART1 peripheral    */
/*Variables: None                           */
/*Return: None #TODO: return success or not */
/********************************************/
void USART1_config(void);

/********************************************/
/*Function: print a byte through USART1     */
/*Variables: that byte                      */
/*Return: that byte #TODO: timeout          */
/********************************************/
int USART1_PutChar(int ch);

/********************************************/
/*Function: wait for a byte from USART1     */
/*Variables: none                           */
/*Return: that byte #TODO: timeout          */
/********************************************/
int usart_receive(void);

/********************************************/
/*Function: print a string through USART1   */
/*Variables: string pointer and size        */
/*Return: nothing #TODO: timeout            */
/********************************************/
void USART1_PutString(char* str, int size);

/**********************************************************/
/*Function: configure SendBuff<->USART1 Pipeline using DMA*/
/*Variables: SendBuff; After configuration, USART1 will   */
/*automatically sends data stored in SendBuff. Also, one  */
/*can change data in SendBuff at anytime; size: the size  */ 
/*of SendBuff, should be 1~65532 #Note: must be called    */
/*after USART1_config!                                    */
/*Return: nothing                                         */
/**********************************************************/
void USART1_DMA_config(char * SendBuff, int size);
	
#endif
