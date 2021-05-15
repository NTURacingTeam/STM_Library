#include "gpio.h"
#include "delay.h"
#include "math.h"

#define ems22_gpio      GPIOA
#define ems22_cc1     GPIO_Pin_6
#define ems22_cc2     GPIO_Pin_7
#define ems22_cc3     GPIO_Pin_8
#define ems22_cc4     GPIO_Pin_9
#define ems22_clk     GPIO_Pin_5
#define ems22_data    GPIO_Pin_10

int i;

void init_ems22(void){
	init_gpio_write(ems22_gpio, ems22_cc1);
	init_gpio_write(ems22_gpio, ems22_cc2);
	init_gpio_write(ems22_gpio, ems22_cc3);
	init_gpio_write(ems22_gpio, ems22_cc4);
	init_gpio_write(ems22_gpio, ems22_clk);
	init_gpio_read(ems22_gpio, ems22_data);
	
}

int ems22Read(targetNum){
	uint16_t targetCC;
	int pos = 0;
	int byte;
	switch (targetNum){
		case 1:
			targetCC = ems22_cc1;
			break;
		case 2:
			targetCC = ems22_cc2;
			break;
		case 3:
			targetCC = ems22_cc2;
			break;
		case 4:
			targetCC = ems22_cc2;
			break;
	}
	GPIO_SetBits(ems22_gpio, targetCC);
	delay(1);
	for (i = 0; i<10; i++) {
		GPIO_ResetBits(ems22_gpio, ems22_clk);
		delay(1);
    GPIO_SetBits(ems22_gpio, ems22_clk);
		delay(1);
		byte = GPIO_ReadInputDataBit(ems22_gpio, ems22_data) == 1 ? 1 : 0;
		pos += byte * pow(2, 10-(i+1));
	}  
	
	 GPIO_ResetBits(ems22_gpio, ems22_clk);
	 delay(1);
   GPIO_SetBits(ems22_gpio, ems22_clk);
   delay(1);
	 return pos;
}