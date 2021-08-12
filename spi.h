#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"

void init_spi1(void){
	
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE); //使能SPI1、PA、PC
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure); //PC3配置成推挽输出 （F_CS）
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure); //PA6复用推挽输出（MISO）
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure); //PA5 clk PA7 MOSI

SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双工模式
SPI_InitStructure.SPI_Mode = SPI_Mode_Slave; //SPI主模式
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8bit数据
SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //CLK空闲时为高电平
SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //CLK上升沿采样，因为上升沿是第二个边沿动作，所以也可以理解为第二个边沿采样
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //片选用软件控制
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //SPI频率：72M/4 = 18M
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前
SPI_InitStructure.SPI_CRCPolynomial = 7; //crc7，stm32spi带硬件ecc
SPI_Init(SPI1, &SPI_InitStructure);
SPI_Cmd(SPI1, ENABLE);



}
u8 SPI1_ReadWriteByte(u8 TxData){        
	while((SPI1->SR&(1<<1))== 0){};           
  SPI1->DR = TxData;                  //发送一个byte          
  while((SPI1->SR&(1<<0))==0){};                                                           
  return SPI1->DR;                 //返回收到的数据
}		