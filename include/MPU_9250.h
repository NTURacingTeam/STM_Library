#ifndef _MPU_H
#define _MPU_H
	
	#include "I2C.h"
	#include "usart1.h"
 
	#define MPU_ADDRESS 104 << 1
	#define MGM_ADDRESS 12 << 1
	#define TIMEOUT 200
	
	#define X 1
	#define Y 2
	#define Z 3
	#define Acc 4
	#define Tmp 5
	#define Gro 6
	#define Mgm 7
	#define Err 0
		
	void Init_MPU(void);
	void Update_Acc(void);
	void Update_Tmp(void);
	void Update_Gro(void);
	void Update_Mgm(void);
	void Update_All(void);
	
	float Read_MPU_Data(uint8_t Type, uint8_t Axis);
#endif
