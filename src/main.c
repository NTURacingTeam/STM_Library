#include "stm32f10x.h"
#include "GPIO.h"
#include "exti.h"
#include "usart1.h"
#include "USART2.h"
#include "SYSTick.h"
#include "DMA_ADC.h"
#include "SPI.h"
#include "NRF24L01.h"

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
	/*
	init_gpio_write(GPIOC, GPIO_Pin_13);
	while(1){
  	set_gpio(GPIOC, GPIO_Pin_13, ON);
		delay_ms(500);
		set_gpio(GPIOC, GPIO_Pin_13, OFF);
		delay_ms(500);
	}
	*/
	
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
	
	/* SysTick Usage Example
	 * Demostrate the usage of SysTick Delay
	 */
	/*
	USART1_config();
	printf("SysTick Example \r\n");
	struct SYSTime NowTime;
	SYSTick_Start();
	while(1){
		SYSTick_delay_ms(500);
		NowTime = SYSTick_CheckTime();
		printf("now time %d, %d, %d \r\n", NowTime.sec, NowTime.ms, NowTime.us);
	}
	*/
	
	/* ADC1 and DMA example
	 * PA0 for adc0, PA1 for adc1, PA2 for adc2
	 */
	/*
	USART1_config();
	u16* adc = DMA_ADC1_get_address();
	DMA_ADC1_Start();
	printf("ADC started \r\n");
	while(1){
		float adc0 = (float) adc[0]/4096*3.3;
		float adc1 = (float) adc[1]/4096*3.3;
		float adc2 = (float) adc[2]/4096*3.3;
		printf("ADC ch0: %f, ch1: %f, ch2: %f \r\n", adc0, adc1, adc2);
		delay_ms(500);
	}
	*/
	
	/* below codes haven't been documented nor exampled*/
	/*
	USART1_config();
	//printf(" SYS \r\n");
	//KEY_GPIO_Config();
	
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
	/*
	int master = 1;
	USART1_config();
	if (master){
		SPI_Config(SPI_Mode_Master, 0, SPI_BaudRatePrescaler_2);
		//SPI_Interrupt_Config();
		int i=0;
		while(1){
			i=i+1;
			SPI_I2S_SendData(SPI1,(uint16_t) i);
			printf("SPIed \r\n");
			delay_ms(1);
		}
	}
	else{
		SPI_Config(SPI_Mode_Slave, 0, SPI_BaudRatePrescaler_2);
		SPI_Interrupt_Config();
		while(1){}
	}
	*/
	//nrf test
	/*RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOB, GPIO_Pin_1);*/
	
	USART1_config();
	uint8_t rxbuf[32];
	uint8_t txbuf[32];
	for(int i=0;i<32;i++){
		txbuf[i] = 0xAB;
	}
	NRF_Init();
	delay_ms(100);
	int tflg = 1;
	int f = NRF24L01_Check();
	if(f==0){
		printf("nrf connect success \r\n");
		if(tflg == 0){
			NRF24L01_RX_Mode();
			while(1){
				NRF24L01_RxPacket(rxbuf);
				printf("receive: ");
				for(int i=0;i<32;i++){
					printf("%c",(char) rxbuf[i]);
				}
				printf("\r\n");
			}
		}
		else{
			NRF24L01_TX_Mode();
			while(1){
				//u8 txresult = NRF24L01_TxPacket(txbuf);
				//printf("transmit result: %d \r\n",txresult);
				printf("PB10: %d\r\n",GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10));
			}
			delay_ms(500);
		}

	}
	else if(f==1){
		printf("nrf connect failed \r\n");
	}
	else{
		printf("wrong return \r\n");
	}
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

