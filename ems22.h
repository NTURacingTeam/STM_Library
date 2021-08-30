#ifndef _GPIO_H
#define _GPIO_H


#include "gpio.h"
#include "delay.h"

#define ems22_gpiox_cc1   GPIOA
#define ems22_pin_cc1     GPIO_Pin_7
#define ems22_gpiox_cc2   GPIOB
#define ems22_pin_cc2     GPIO_Pin_8
#define ems22_gpiox_cc3   GPIOA
#define ems22_pin_cc3     GPIO_Pin_7
#define ems22_gpiox_cc4   GPIOA
#define ems22_pin_cc4     GPIO_Pin_7
#define ems22_gpiox_clk   GPIOA
#define ems22_pin_clk     GPIO_Pin_5
#define ems22_gpiox_data  GPIOA
#define ems22_pin_data    GPIO_Pin_6
#define span              100
uint16_t pos2[2] = {0, 0};

void init_ems22(void)
{
	init_gpio_write(ems22_gpiox_cc1, ems22_pin_cc1);
	init_gpio_write(ems22_gpiox_cc2, ems22_pin_cc2);
	init_gpio_write(ems22_gpiox_cc3, ems22_pin_cc3);
	init_gpio_write(ems22_gpiox_cc4, ems22_pin_cc4);
	init_gpio_write(ems22_gpiox_clk, ems22_pin_clk);
	init_gpio_read(ems22_gpiox_data, ems22_pin_data);
  init_delay();
	GPIO_SetBits(ems22_gpiox_cc1, ems22_pin_cc1);
	GPIO_SetBits(ems22_gpiox_cc1, ems22_pin_cc2);
	GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
}


uint16_t ems22Read(targetNum)
{
	uint16_t target_cc;
	GPIO_TypeDef* target_gpiox;
	uint16_t pos = 0;
	
	switch (targetNum){
		case 1:
			target_gpiox = ems22_gpiox_cc1;
			target_cc = ems22_pin_cc1;
			break;
		case 2:
			target_gpiox = ems22_gpiox_cc2;
			target_cc = ems22_pin_cc2;
			break;
		case 3:
			target_gpiox = ems22_gpiox_cc3;
			target_cc = ems22_pin_cc3;
			break;
		case 4:
			target_gpiox = ems22_gpiox_cc4;
			target_cc = ems22_pin_cc4;
			break;
	}
	
	GPIO_ResetBits(target_gpiox, target_cc);
	delayUs(10);
	
	for (int i = 0; i<10; i++) {
		GPIO_ResetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(10);
    GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(10);
		
		pos = pos << 1;
		pos = pos | GPIO_ReadInputDataBit(ems22_gpiox_data, ems22_pin_data);
	}  
	
	for (int i = 0; i<7; i++) {
		GPIO_ResetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(10);
    GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(10);
	} 
	
	GPIO_SetBits(target_gpiox, target_cc);
	return pos;
}



void ems22Read_2()
{
	
	GPIO_ResetBits(ems22_gpiox_cc1, ems22_pin_cc1);
	GPIO_ResetBits(ems22_gpiox_cc2, ems22_pin_cc2);
	delayUs(span);
	
	pos2[0] = 0;
	pos2[1] = 0;
	
	for (int i = 0; i<10; i++) {
		GPIO_ResetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
    GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
		
		pos2[0] = pos2[0] << 1;
		pos2[0] = pos2[0] | GPIO_ReadInputDataBit(ems22_gpiox_data, ems22_pin_data);
	}
	
	for (int i = 0; i<6; i++) {
		GPIO_ResetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
    GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
	}
	
	GPIO_ResetBits(ems22_gpiox_clk, ems22_pin_clk);
	delayUs(span);
  GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
	delayUs(span);
	
	for (int i = 0; i<10; i++) {
		GPIO_ResetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
    GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
		
		pos2[1] = pos2[1] << 1;
		pos2[1] = pos2[1] | GPIO_ReadInputDataBit(ems22_gpiox_data, ems22_pin_data);
	}  
	
	for (int i = 0; i<6; i++) {
		GPIO_ResetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
    GPIO_SetBits(ems22_gpiox_clk, ems22_pin_clk);
		delayUs(span);
	}

	GPIO_SetBits(ems22_gpiox_cc1, ems22_pin_cc1);
	GPIO_SetBits(ems22_gpiox_cc2, ems22_pin_cc2);

}

#endif