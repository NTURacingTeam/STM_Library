#include "SPI.h"

void SPI_Config(uint16_t trans_mode, uint16_t clock_mode, uint16_t prescaler) {
    // Enable RCC clock for SPI
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /*Enable GPIOs
     *NSS:  PA4
     *SCK:  PA5
     *MISO: PA6
     *MOSI: PA7
    Check Ref. Man. Table 25
    **/
    // Enable RCC clock for GPIO
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // Connect the pin to the desired peripherals' Alternate Function
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_SPI1);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
    // Init PA15, no need for software managed NSS
    // Init PA5
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    if (trans_mode == SPI_Mode_Master)
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    else
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    // Init PA6
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    if (trans_mode == SPI_Mode_Master)
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    else
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    // Init PA7
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    if (trans_mode == SPI_Mode_Master)
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    else
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Init SPI
    SPI_InitTypeDef SPI_InitStruct;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = trans_mode;
    // SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    if (clock_mode == 0) {
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    } else if (clock_mode == 1) {
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    } else if (clock_mode == 2) {
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    } else {
        SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    }
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = prescaler;
    // SPI_InitStruct.SPI_BaudRatePrescaler =  SPI_BaudRatePrescaler_2;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_InitStruct.SPI_CRCPolynomial = ;
    SPI_Init(SPI1, &SPI_InitStruct);
    if (trans_mode == SPI_Mode_Master) SPI_Cmd(SPI1, ENABLE);
}

void SPI_Interrupt_Config() {
    SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);  // make SPI1 receive interrupt enable

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;            // Configure SPI1 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // Set the priority group of SPI1 interrupt
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         // Set the subpriority inside the group
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            // Globally enable SPI1 interrupt
    NVIC_Init(&NVIC_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI_Send(uint8_t TxData) {
    uint8_t retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)  //检查发送缓存空标志位设置与否
    {
        retry++;
        if ((retry / 10) > 255) return 0;
    }
    SPI_I2S_SendData(SPI1, TxData);  //通过外设SPI1发送一个数据
    return 1;
}

uint8_t SPI_Receive(void) {
    uint8_t retry = 0;
    retry = 0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)  //检查接受缓存非空标志位设置与否
    {
        retry++;
        if (retry > 200) return 0;
    }
    return SPI_I2S_ReceiveData(SPI1);  //返回通过SPI1最近接收的数据
}
