/**********************/
/*Owner: NTU FSAE TEAM*/
/*Author: 陳一        */
/*Editor: 黃柏瑞      */
/**********************/

#ifndef _USART2_H
#define _USART2_H

#include "stm32f10x.h"
#include <string.h>
#include "stdio.h"
#include "stdlib.h"

/********************************************/
/*Function: initialize USART2 peripheral    */
/*Variables: None                           */
/*Return: None #TODO: return success or not */
/********************************************/
void USART2_Config(void);

/********************************************/
/*Function: print a byte through USART2     */
/*Variables: that byte                      */
/*Return: that byte #TODO: timeout          */
/********************************************/
void USART2_PutChar(char c);

/********************************************/
/*Function: print a string through USART2   */
/*Variables: string pointer and size        */
/*Return: nothing #TODO: timeout            */
/********************************************/
void USART2_PutString(char *s);

/************************************************************************/
/*Function: assign a queue to store received data from USART2 interrupt */
/*Variables: size: the size of the queue                                */
/*Return: nothing #TODO: timeout                                        */
/************************************************************************/
void USART2_Config_Buffer(int size);

/*******************************************/
/*Function: Get a character from the queue */
/*Variables: None                          */
/*Return: that character                   */
/*******************************************/
char USART2_Get_Buffer(void);

/*******************************************/
/*Function: Put a character into the queue */
/*Variables: that character                */
/*Return: None                             */
/*******************************************/
void USART2_Put_Buffer(char c);

#endif
