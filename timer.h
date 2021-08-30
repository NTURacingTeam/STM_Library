#include "stm32f10x.h"
#include "stm32f10x_tim.h"

void NVIC_Config_tim3(){
	
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); // ???
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //搶佔優先順序
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;             //子優先順序
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void init_tim3(u16 arr, u16 psc){
	
	TIM_TimeBaseInitTypeDef TIM_Timeinit;                          //結構變數
	
	TIM_Timeinit.TIM_ClockDivision = 0;                            //隨意選取
	TIM_Timeinit.TIM_CounterMode =  TIM_CounterMode_Up;            //計數模式
	TIM_Timeinit.TIM_Period = arr;                                 // 需要根據要計時的長度來計算輸入
	TIM_Timeinit.TIM_Prescaler = psc;                              //預分頻係數 計算輸入
	//TIM_Timeinit.TIM_RepetitionCounter =                         //這個不需要
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);            //使能定時器3時鐘
	TIM_TimeBaseInit(TIM3,&TIM_Timeinit);                          //初始化定時器配置
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                       //開啟定時器中斷 配置nvic
  TIM_Cmd(TIM3,ENABLE);                                          //使能定時器
	
	NVIC_Config_tim3();

}
