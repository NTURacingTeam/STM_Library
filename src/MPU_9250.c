#include "MPU_9250.h"
	
uint8_t acc_byte[6];
uint8_t tmp_byte[2];
uint8_t gro_byte[6];
uint8_t mgm_byte[6];
uint8_t all_byte[14];

int16_t ax,ay,az,tmp,gx,gy,gz,mx,my,mz;

void Init_MPU(void){
	Init_I2C();
	I2C_Write(I2C1, MPU_ADDRESS, 0x37, 0x02, TIMEOUT);
}

void Update_Acc(void){
	I2C_Read_Burst(I2C1, MPU_ADDRESS, 0x3B, 6, acc_byte, TIMEOUT);
	ax = acc_byte[0]<<8|acc_byte[1];
	ay = acc_byte[2]<<8|acc_byte[3];
	az = acc_byte[4]<<8|acc_byte[5];
}

void Update_Tmp(void){
	I2C_Read_Burst(I2C1, MPU_ADDRESS, 0x41, 2, tmp_byte, TIMEOUT);
	tmp = tmp_byte[0]<<8|tmp_byte[1];
}

void Update_Gro(void){
	I2C_Read_Burst(I2C1, MPU_ADDRESS, 0x43, 6, gro_byte, TIMEOUT);
	gx = gro_byte[0]<<8|gro_byte[1];
	gy = gro_byte[2]<<8|gro_byte[3];
	gz = gro_byte[4]<<8|gro_byte[5];
}

void Update_Mgm(void){
	uint8_t ST1;
	I2C_Write(I2C1, MGM_ADDRESS, 0x0A, 0x01, TIMEOUT);
	do{
		I2C_Read_Burst(I2C1, MGM_ADDRESS, 0x02, 1, &ST1, TIMEOUT);
	}while(!(ST1 & 0x01));
	I2C_Read_Burst(I2C1, MGM_ADDRESS, 0x03, 6, mgm_byte, TIMEOUT);
	mx = mgm_byte[3]<<8|mgm_byte[2];
	my = mgm_byte[1]<<8|mgm_byte[0];
	mz = -(mgm_byte[5]<<8|mgm_byte[4]);	
}

void Update_All(void){
	I2C_Read_Burst(I2C1, MPU_ADDRESS, 0x3B, 14, all_byte, TIMEOUT);
	//printf("updated \r\n");
	ax = all_byte[0]<<8|all_byte[1];
	ay = all_byte[2]<<8|all_byte[3];
	az = all_byte[4]<<8|all_byte[5];
	tmp = all_byte[6]<<8|all_byte[7];
	gx = all_byte[8]<<8|all_byte[9];
	gy = all_byte[10]<<8|all_byte[11];
	gz = all_byte[12]<<8|all_byte[13];
	/*
	for(int i=0;i<14;i++){
		if(i<6){acc_byte[i] = all_byte[i];}
		else if((i>=6)&&(i<8)){tmp_byte[i-6] = all_byte[i];}
		else if((i>=8)&&(i<14)){gro_byte[i-8] = all_byte[i];}
		else{continue;}
	}
	*/
	Update_Mgm();
}
	
float Read_MPU_Data(uint8_t Type, uint8_t Axis){
	switch(Type){
		case Acc:
			switch(Axis){
				case X:
					return (float) ax/16384;//g
				case Y:
					return (float) ay/16384;		
				case Z:
					return (float) az/16384;		
			}
		case Tmp:
			return (float) tmp+21;
		case Gro:
			switch(Axis){
				case X:
					return (float) gx/131;//deg/s			
				case Y:
					return (float) gy/131;//deg/s
				case Z:
					return (float) gz/131;//deg/s
			}
		case Mgm:
			switch(Axis){
				case X:
					return (float) mx*0.6;//uT
				case Y:
					return (float) my*0.6;//uT
				case Z:
					return (float) mz*0.6;//uT
			}
	}
	return Err;
}
