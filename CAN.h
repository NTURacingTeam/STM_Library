#ifndef _CAN_H
#define _CAN_H

// Include
#include <assert.h>
#include <stdio.h>

#include "stm32f10x.h"

// Defines
// CAN RX, TX pin define, if you are not using stm32f103c8t6, please modify pins below
#define GPIO_Pin_CAN1_RX GPIO_Pin_8  // PB8
#define GPIO_Pin_CAN1_TX GPIO_Pin_9  // PB9

#define CANx CAN1

#define CAN_Baudrate 250  // 250kBps

/**
 * @brief enable CAN with CANRX at PB8, CANTX at PB9
 *
 * @author 詹侑昕
 * @date 2021-08-06
 */
void CAN_Config() {
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    // GPIO clocks enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // Configure CAN1 RX
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN1_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configure CAN1 TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN1_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Remap CAN1 GPIOs
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);

    // CANx Periph clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // CAN register init
    CAN_DeInit(CANx);
    CAN_StructInit(&CAN_InitStructure);

    // CAN cell init
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;

#if CAN_Baudrate == 1000  // 1MBps
    CAN_InitStructure.CAN_Prescaler = 6;
#elif CAN_Baudrate == 500  // 500KBps
    CAN_InitStructure.CAN_Prescaler = 12;
#elif CAN_Baudrate == 250  // 250KBps
    CAN_InitStructure.CAN_Prescaler = 9;
#elif CAN_Baudrate == 125  // 125KBps
    CAN_InitStructure.CAN_Prescaler = 48;
#elif CAN_Baudrate == 100  // 100KBps
    CAN_InitStructure.CAN_Prescaler = 60;
#elif CAN_Baudrate == 50   // 50KBps
    CAN_InitStructure.CAN_Prescaler = 120;
#elif CAN_Baudrate == 20   // 20KBps
    CAN_InitStructure.CAN_Prescaler = 300;
#elif CAN_Baudrate == 10   // 10KBps
    CAN_InitStructure.CAN_Prescaler = 600;
#else
#error "Please select first the CAN Baudrate in Private defines in main.c "
#endif /* CAN_Baudrate == 1000 */

    // CAN init
    assert(CAN_Init(CANx, &CAN_InitStructure) == CAN_InitStatus_Success);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;

    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

/**
 * @brief to init the CAN RX Message
 *
 * @param RxMes your CAN RX Message
 * @author 詹侑昕
 * @date 2021-08-06
 */
void init_RxMes(CanRxMsg* RxMes) {
    RxMes->StdId = 0x00;
    RxMes->ExtId = 0x00;
    RxMes->IDE = CAN_ID_STD;
    RxMes->DLC = 0;
    RxMes->FMI = 0;
    for (uint8_t i = 0; i < 8; ++i) {
        RxMes->Data[i] = 0x00;
    }
}

/**
 * @brief
 *
 * @author 詹侑昕
 * @date 2021-08-06
 */
void CAN_NVIC_Config(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

#endif