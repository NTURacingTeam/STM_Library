#include "i2c.h"

#define	ACCEL_XOUT_H	0x3B    //加速度计X轴数据高位
#define	ACCEL_XOUT_L	0x3C    //加速度计X轴数据低位
#define	ACCEL_YOUT_H	0x3D    //以此类推
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H	0x41    //温度传感器数据
#define	TEMP_OUT_L	0x42
#define	GYRO_XOUT_H	0x43    //陀螺仪X轴数据高位
#define	GYRO_XOUT_L	0x44	
#define	GYRO_YOUT_H	0x45
#define	GYRO_YOUT_L	0x46
#define	GYRO_ZOUT_H	0x47
#define	GYRO_ZOUT_L	0x48
#define	PWR_MGMT_1	0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I	0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0 	//MPU6050模块AD0引脚接低电平时的地址



void init_mpu6050()
{
	init_i2c();
	
	I2C_WriteByte(PWR_MGMT_1,0x00);     //解除休眠状态
  I2C_WriteByte(SMPLRT_DIV,0x07);     //陀螺仪采样率1kHz
  I2C_WriteByte(CONFIG,0x02);         //设置低通滤波器
  I2C_WriteByte(GYRO_CONFIG,0x18);    //陀螺仪量程2000deg/s
  I2C_WriteByte(ACCEL_CONFIG,0x08);   //加速度量程4g
}