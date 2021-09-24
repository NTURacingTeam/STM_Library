// Include
#include "CAN.h"
#include "delay.h"
#include "stm32f10x.h"
#include "usart2.h"

// Defines
#define STDID 0x091
#define EXDID 0x091

CanTxMsg TxMes;
CanRxMsg RxMes;

// will be triggered when receive data
void USB_LP_CAN1_RX0_IRQHandler(void);

int main() {
    // init your usart to print something
    init_usart2();

    CAN_NVIC_Config();
    CAN_Config();
    // Interupt and flag managemment functions
    CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);

    init_RxMes(&RxMes);

    TxMes.StdId = STDID;
    TxMes.RTR = CAN_RTR_DATA;
    TxMes.IDE = CAN_ID_STD;
    TxMes.DLC = 8;

    while (1) {
        // Transmit
        // TxMes.Data[0] ~ TxMes.Data[7]
        TxMes.Data[0] = 0x47;
        /*
        TxMes.Data[1] = 0x??;
        .
        .
        .
        TxMes.Data[7] = 0x??;
        */
        uint8_t TransmitMailBox = 0;
        TransmitMailBox = CAN_Transmit(CANx, &TxMes);
        while (CAN_TransmitStatus(CANx, TransmitMailBox) != CAN_TxStatus_Ok) {
            continue;
        }
        // Debug
        // printf("flag 1:%d 2:%d 3:%d state:%d\n", CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP0), CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP1), CAN_GetFlagStatus(CANx, CAN_FLAG_RQCP2), CAN_TransmitStatus(CANx, 0));
        printf("trasmit success\r\n");
        delayMs(200);
    }
}

void USB_LP_CAN1_RX0_IRQHandler(void) {
    CAN_Receive(CANx, CAN_FIFO0, &RxMes);
    printf("The address is %x, DLC is %d, and the data is %x.\r\n", RxMes.StdId, RxMes.DLC, RxMes.Data[0]);
    for (int i = 0; i < RxMes.DLC; ++i) {
        printf(" %x", RxMes.Data[i]);
    }
    printf("\r\n");
}