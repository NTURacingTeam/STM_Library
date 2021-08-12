#include "SYSTick.h"

struct SYSTime SYSTick_Time;
int last10ms = 0;

void SYSTick_Start(void) {
    SysTick->LOAD = 72000 * 10;
    SysTick->VAL = 0X00;  // clear the tickle
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |  // Check Clock Source
                    SysTick_CTRL_TICKINT_Msk |    // Enable Interrupt
                    SysTick_CTRL_ENABLE_Msk;      // Enable Systick
    SYSTick_Time.ms = 0;
    SYSTick_Time.sec = 0;
}

void SYSTick_10ms_increment(void) {
    SYSTick_Time.ms += 10;
    if (SYSTick_Time.ms >= 1000) {
        SYSTick_Time.ms = 0;
        SYSTick_Time.sec += 1;
    }
}

struct SYSTime SYSTick_CheckTime(void) {
    struct SYSTime temp;
    temp.sec = SYSTick_Time.sec;
    temp.ms = SYSTick_Time.ms + (10000 - SysTick->VAL / 72) / 1000;
    temp.us = (10000 - SysTick->VAL / 72) % 1000;
    // printf("us: %d \r\n",(10000 - SysTick->VAL/72)/1000);
    return temp;
}

void SYSTick_delay_ms(int delay_in_ms) {
    struct SYSTime StartTime = SYSTick_CheckTime();
    struct SYSTime TempTime;
    int time_pass;
    do {
        TempTime = SYSTick_CheckTime();
        time_pass = (TempTime.sec - StartTime.sec) * 1000 + (TempTime.ms - StartTime.ms);
        // printf("timepass %d \r\n", time_pass);
    } while (time_pass < delay_in_ms);
}

void SYSTick_delay_us(int delay_in_us) {
    struct SYSTime StartTime = SYSTick_CheckTime();
    struct SYSTime TempTime;
    int time_pass;
    do {
        TempTime = SYSTick_CheckTime();
        time_pass = (TempTime.ms - StartTime.ms) * 1000 + (TempTime.us - StartTime.us);
        // printf("timepass %d \r\n", time_pass);
    } while (time_pass < delay_in_us);
}
