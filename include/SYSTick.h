/**********************/
/*Owner: NTU FSAE TEAM*/
/*Author: 黃柏瑞      */
/*Editor: 黃柏瑞      */
/**********************/
#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "stm32f10x.h"
#include "usart1.h"

/********************************************/
/*Structure: record the current time, access*/
/*it via SYSTick_CheckTime method           */
/********************************************/
struct SYSTime {
    uint16_t sec;
    uint16_t ms;
    uint16_t us;
};

/********************************************/
/*Function: initialize SysTick peripheral   */
/*Variables: None                           */
/*Return: None #TODO: return success or not */
/********************************************/
void SYSTick_Start(void);

/********************************************/
/*Function: for SysTick interrupt           */
/*Variables: None                           */
/*Return: None #TODO: return success or not */
/********************************************/
void SYSTick_10ms_increment(void);

/********************************************/
/*Function: return current system time      */
/*Variables: None                           */
/*Return: SYSTime struct                    */
/********************************************/
struct SYSTime SYSTick_CheckTime(void);

/********************************************/
/*Function: delay for a period              */
/*Variables: time to delay in ms            */
/*Return: None #TODO: return success or not */
/********************************************/
void SYSTick_delay_ms(int delay_in_ms);

/********************************************/
/*Function: delay for a period              */
/*Variables: time to delay in ms            **/
/*Return: None #TODO: return success or not */
/********************************************/
void SYSTick_delay_us(int delay_in_us);

#endif
