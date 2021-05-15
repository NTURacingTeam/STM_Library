#include "SYSTick.h"

struct SYSTime SYSTick_Time;

void SYSTick_Start(void){
	SysTick->LOAD = 72000*10;
	SysTick->VAL=0X00;//clear the tickle
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |      //Check Clock Source
                   SysTick_CTRL_TICKINT_Msk   |      //Enable Interrupt
                   SysTick_CTRL_ENABLE_Msk;          //Enable Systick
	SYSTick_Time.ms = 0;
	SYSTick_Time.sec = 0;
}

void SYSTick_10ms_increment(void){
	SYSTick_Time.ms+=10;
	if(SYSTick_Time.ms>=1000){
		SYSTick_Time.ms = 0;
		SYSTick_Time.sec +=1;
	}
}

struct SYSTime SYSTick_CheckTime(void){
	return SYSTick_Time;
}

void delay_10ms(int delay_in_10ms){
	struct SYSTime StartTime = SYSTick_CheckTime();
	struct SYSTime TempTime;
	int time_pass;
	do{
		TempTime = SYSTick_CheckTime();
		time_pass = (TempTime.sec - StartTime.sec)*1000+(TempTime.ms - StartTime.ms);
		//printf("timepass %d \r\n", time_pass);
	}while(time_pass<delay_in_10ms);
}
