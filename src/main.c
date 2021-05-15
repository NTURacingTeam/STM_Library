#include "stm32f10x.h"
#include "GPIO.h"
#include "exti.h"
#include "usart1.h"
#include "USART2.h"
/* below header haven't been documented nor exampled*/
/*
#include "adc.h"
#include "PWM.h"
#include "I2C.h"
#include "MPU_9250.h"
#include "SYSTick.h"
*/

/* below variables is not used in following examples*/
//struct SYSTime NowTime;
//extern __IO uint16_t ADC_ConvertedValue;
//float ADC_ConvertedValueLocal;

void delay_ms(u16 nms);

int main(void)
{
	/* Blink*/
	/* uncomment to try on this!*/
	init_gpio_write(GPIOE, GPIO_Pin_5);
	while(1){
  	set_gpio(GPIOE, GPIO_Pin_5, ON);
		delay_ms(500);
		set_gpio(GPIOE, GPIO_Pin_5, OFF);
		delay_ms(500);
	}
	
	/* GPIO.h example */
	/* uncomment to try on this!*/
	/*
	init_gpio_read(GPIOE, GPIO_Pin_4);
	init_gpio_write(GPIOE, GPIO_Pin_5);
	while(1){
		if(scan_gpio(GPIOE, GPIO_Pin_4)){
			set_gpio(GPIOE, GPIO_Pin_5, ON);
		}
		else{
			set_gpio(GPIOE, GPIO_Pin_5, OFF);
		}
	}
	*/
	
	/* USART 1 example*/
	/* Please use Putty and USB-tty to try on this*/
	/* You can use printf/scanf because we define fputc/fgetc in *.c*/
	/* Also, remember to select MicroLib (In "options for target...->target")*/
	/* We use PRINTF_USE_USART1 macro to choose which USART we use to redirect printf/scanf*/
	/* uncomment to try on this!*/
	/*
	#define PRINTF_USE_USART1
	int num;
	USART1_config();
	while(1){
  	printf("\r\nPlease input a num:");
		scanf("%d", &num);
		printf("\r\nThe num you type is %d \r\n", num);
		delay_ms(500);
	}
	*/
	
	/* USART1 and DMA1 example*/
	/* Hello there!! General Kenobi...*/
	/* uncomment to try on this!*/
	/*
	char SendBuff[16] = "Hello there!! \r\n";//16 words
	char Hello[16] = "general kenobi\r\n";
	char Gener[16] = "Hello there!! \r\n";
	USART1_config();
	USART1_DMA_config(SendBuff, 16);
	int flg = 1;
	while(1){
		if(flg){
			for(int i=0;i<16;i++)
				SendBuff[i] = Hello[i];
			flg=0;
		}
		else{
			for(int i=0;i<16;i++)
				SendBuff[i] = Gener[i];
			flg=1;
		}
		
		delay_ms(500);
	}
	*/
	
	/* USART2 and interrupt example */
	/* When we type a char in Putty, interrupt happens and put the char into a queue*/
	/* In each while loop, we take one char out of the queue*/
	/* We can access the queue using USART2_Get_Buffer*/
	/*
	USART2_Config();
	USART2_PutChar('A');//you can see a A on Putty
	USART2_Config_Buffer(5);
	while(1){
		USART2_PutChar(USART2_Get_Buffer());//should print what you had just typed
		delay_ms(500);
	}
	*/
	
	/* below codes haven't been documented nor exampled*/
	/*
	USART1_config();
	SYSTick_Start();
	//printf(" SYS \r\n");
	//KEY_GPIO_Config();
	//delay_10ms(500);
	NowTime.sec = 0;
	NowTime.ms = 0;
	printf(" OK \r\n");
	while(1){
		delay_10ms(500);
		NowTime = SYSTick_CheckTime();
		printf("now time %d, %d \r\n", NowTime.sec, NowTime.ms);
	}
	
	//Init_MPU();
	printf(" OK \r\n");
	while(1){
		//Update_All();
		printf(" ax: %f, ay: %f, az: %f, gx: %f, gy: %f, gz: %f, mx: %f, my: %f, mz: %f \r\n",
		Read_MPU_Data(Acc, X),
		Read_MPU_Data(Acc, Y),
		Read_MPU_Data(Acc, Z),
		Read_MPU_Data(Gro, X),
		Read_MPU_Data(Gro, Y),
		Read_MPU_Data(Gro, Z),
		Read_MPU_Data(Mgm, X),
		Read_MPU_Data(Mgm, Y),
		Read_MPU_Data(Mgm, Z)
		);
		printf("mx: %f, my: %f, mz: %f \r\n",
		Read_MPU_Data(Mgm, X),
		Read_MPU_Data(Mgm, Y),
		Read_MPU_Data(Mgm, Z)
		);
		LEDE5(ON);
		delay_10ms(200);
		LEDE5(OFF);
		delay_10ms(200);
	}
	//ADC1_Init();
	
	Init_I2C();
	printf(" I2C OK \r\n");
	while(1){
		I2C_start(I2C1, SLAVE_ADDRESS,  I2C_Direction_Transmitter);
		I2C_write(I2C1, 0x3B);
		I2C_start(I2C1, SLAVE_ADDRESS,  I2C_Direction_Receiver);
		for(int i=0;i<5;i++){
			acc_byte[i] = I2C_read_ack(I2C1);
		}
		acc_byte[5] = I2C_read_nack(I2C1);
		I2C_stop(I2C1);
		printf(" ACC byte 1: %d \r\n", acc_byte[0]);
		delay_ms(500);
	}
	*/
}

void delay_ms(u16 nms)
{
 u32 temp;
 SysTick->LOAD = 9000*nms;
 SysTick->VAL=0X00;//clear the tickle
 SysTick->CTRL=0X01;//enable
 do
 {
  temp=SysTick->CTRL;//read the tickle
 }while((temp&0x01)&&(!(temp&(1<<16))));//wait for the end
    SysTick->CTRL=0x00; //close it
    SysTick->VAL =0X00; //clear it
}

