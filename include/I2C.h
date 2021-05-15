#ifndef _I2C_H
#define _I2C_H
	
	#include "stm32f10x.h"
	#include "stdio.h"
	#include "stm32f10x_i2c.h"
	#include "SYSTick.h"
	
	//#define DEBUG
 
	void Init_I2C(void);
	void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction, int timeout);
	void I2C_write(I2C_TypeDef* I2Cx, uint8_t data, int timeout);
	uint8_t I2C_read_ack(I2C_TypeDef* I2Cx, int timeout);
	uint8_t I2C_read_nack(I2C_TypeDef* I2Cx, int timeout);
	void I2C_stop(I2C_TypeDef* I2Cx);
	
	void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t Reg, uint8_t data, int timeout);
	void I2C_Read_Burst(I2C_TypeDef* I2Cx, uint8_t address, uint8_t Reg, int length, uint8_t *recv, int timeout);
#endif
