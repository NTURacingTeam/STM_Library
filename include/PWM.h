#ifndef _PWM_H
#define _PWM_H

	#include "stm32f10x.h"
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_tim.h"

	void TIM3_PWM_Init(u16 arr,u16 psc);
	
#endif
