#include "usart2.h"
//#include "adc_m.h"
#include "ems22.h"
#include "stdio.h"
//#include "exti.h" 
//#include "timer.h"
#include "spi.h"
//#include "pwm.h"
//#include "delay.h"
//#include "stm32f10x_it.h"


void TIM3_IRQHandler(){
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET){         //獲取狀態定時器的狀態 Upadate更新中斷，是否更新了
//		printf ("assss\n");
//		GPIO_SetBits(GPIOA,GPIO_Pin_5);                      //定時器中斷髮生，將燈滅；
//		delay(100);	
//		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);            //清除中斷標誌
	} 
}

void EXTI3_IRQHandler(){
	
	if( EXTI_GetITStatus(EXTI_Line3) != RESET ){
		EXTI_ClearITPendingBit(EXTI_Line3);
//		printf ("as\n");
//    GPIO_SetBits(GPIOA , GPIO_Pin_5);
//    delay(300);
//	  GPIO_ResetBits(GPIOA , GPIO_Pin_5);
//	  delay(100);
  }
}

int main (void){
	
//	init_adc_m();
  init_gpio_write(GPIOA, GPIO_Pin_5);
//	init_ems22();
	init_usart2();
//	init_exti();
//	init_tim3(2000, 35999);
//	init_pwm(179, 359);
	init_spi1();
	
	printf("b\n");
	
	while (1){
		
//		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//		SPI_I2S_SendData(SPI1, Data);	//发送数据
		
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		u8 Data = SPI_I2S_ReceiveData(SPI1);	  //接收数据
		
		printf("%o\n", Data);
		delay(1000);
		
//		printf ("\n"); 
//		delay(100);
//		printf("ADC channel 0: %d \n", ADC1_Read_m(0));
//		delay(100);
//		printf("ADC channel 1: %d \n", ADC1_Read_m(1)); 
//		delay(100);
//		printf("ADC channel 6: %d \n", ADC1_Read_m(2));
//		delay(500);
		
		
//		printf("ww\n");
//		GPIO_SetBits(GPIOA, GPIO_Pin_5);
//		delay(100);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//		delay(100);

		
//		for(i = 0; i<3; i++){
//			uint16_t t = AD_Value[i];
//			printf("ems22 %d : %d", i+1, ems22Read(i+1));
//			printf("\n");
//			delay(100);
//		}
//		printf("\n");
//		delay(400);	


//		for(int i = 0; i < 179; i++){
//			TIM_SetCompare1(TIM1,i);
//			delay(5);
//		}
	}
}
