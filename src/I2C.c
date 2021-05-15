#include "I2C.h"

#ifdef DEBUG
	struct SYSTime StartTime;
	struct SYSTime TempTime;
	int time_pass=0;
	#define DEBUG_CHECK_FLG(x){\
		StartTime = SYSTick_CheckTime();\
		time_pass=0;\
		do{\
			TempTime = SYSTick_CheckTime();\
			time_pass = (TempTime.sec - StartTime.sec)*1000+(TempTime.ms - StartTime.ms);\
		}\
    while(x&&(!(time_pass>=timeout)));\
		if(time_pass>=timeout){printf("time pass %d line number %d \r\n", time_pass, __LINE__);}\
	}
#else
	#define DEBUG_CHECK_FLG(x){while(x);}
#endif


/*
from
https://lolikitty.pixnet.net/blog/post/164994648
*/

void Init_I2C(void) { 
    I2C_InitTypeDef I2C_InitStructure; 
    GPIO_InitTypeDef GPIO_InitStructure;
 
    /* start I2C */
    I2C_Cmd(I2C1,ENABLE);
    /* start I2C1 RCC clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 
    /* set SDA & SCL pins for I2C1 */
    /* PB6 = SCL ,PB7 = SDA */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
 
    /* set I2C1 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; // I2C address for STM32
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000 ; // I2C clock speed 400K 
    I2C_Init(I2C1, &I2C_InitStructure);
}
 
/* send "start signal"
 * send to "Slave Device" address + R/W bit
 * 
 * variables:
 * I2Cx --> I2C peripherals, ex:I2C1
 * address --> 7 bit address of Slave Device
 * direction:
 * I2C_Direction_Tranmitter: send mode
 * I2C_Direction_Receiver: receive mode
 */
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, int timeout){
    // wait for I2C1 to be free...
		DEBUG_CHECK_FLG(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
    // send signal to I2C1
    I2C_GenerateSTART(I2Cx, ENABLE);
		// wait for I2C1 EV5 --> check start signal from Slave Device 
		DEBUG_CHECK_FLG(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    // Send slave Address for write 
    I2C_Send7bitAddress(I2Cx, address, direction);
 
    /* wait for I2C1 EV6, and choose I2C direction:
    * I2C_Direction_Tranmitter or
    * I2C_Direction_Receiver
    */ 
    if(direction == I2C_Direction_Transmitter){
			DEBUG_CHECK_FLG((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)));
    } else if(direction == I2C_Direction_Receiver){
			DEBUG_CHECK_FLG((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)));
    }
}
 
/* send a byte to Slave Device
 *
 * variables:
 *I2Cx --> I2C peripherals, ex:I2C1
 *data --> what to send
 */
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data, int timeout)
{
	I2C_SendData(I2Cx, data);
  // what for I2C1 EV8_2 --> already send a byte
	DEBUG_CHECK_FLG((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));
}
 
/* read a byte from Slave Device
 * and ask for a continuous byte
 */
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx, int timeout){
	uint8_t data=0;
  // start and check configuration
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  // wait for receiving a byte
	DEBUG_CHECK_FLG((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)));
  // read the byte from I2C register
  data = I2C_ReceiveData(I2Cx);
  return data;
}
 
/* read a byte from Slave Device
 * and don't ask for a continuous byte
 */
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx, int timeout){
    uint8_t data=0;
    // disabe acknowledge of received data
    // nack also generates stop condition after last byte received
    // see reference manual for more info
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);
    // wait for receiving a byte
    DEBUG_CHECK_FLG((!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)));
    // read the byte from I2C register
    data = I2C_ReceiveData(I2Cx);
    return data;
}
 
/* send stop signal
 * release I2C bus
 */
void I2C_stop(I2C_TypeDef* I2Cx){
    // stop I2C1
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t Reg, uint8_t data, int timeout){
		I2C_start(I2Cx, address,  I2C_Direction_Transmitter, timeout);
		I2C_write(I2Cx, Reg, timeout);
		I2C_write(I2Cx, data, timeout);
		I2C_stop(I2Cx);
}

void I2C_Read_Burst(I2C_TypeDef* I2Cx, uint8_t address, uint8_t Reg, int length, uint8_t *recv, int timeout){
		I2C_start(I2Cx, address,  I2C_Direction_Transmitter, timeout);
		//printf(" i2c start \r\n");
		I2C_write(I2Cx, Reg, timeout);
		//printf(" write \r\n");
		I2C_start(I2Cx, address,  I2C_Direction_Receiver, timeout);
		//printf(" i2c start 2 \r\n");
		for(int i=0;i<(length-1);i++){
			recv[i] = I2C_read_ack(I2Cx, timeout);
			//printf(" i2c read # %d \r\n", i);
		}
		recv[(length-1)] = I2C_read_nack(I2Cx, timeout);
		//printf(" i2c read nck \r\n");
		I2C_stop(I2Cx);
		//printf(" stop \r\n");
}
