#ifndef _SYSTICK_H
#define _SYSTICK_H

	#include "stm32f10x.h"
	#include "usart1.h"
	
	struct SYSTime{
		uint16_t sec;
		uint16_t ms;
	};
	void SYSTick_Start(void);
	void SYSTick_10ms_increment(void);
	struct SYSTime SYSTick_CheckTime(void);
	void delay_10ms(int delay_in_10ms);

#endif
