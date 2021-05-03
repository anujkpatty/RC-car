/*
 * GccApplication4.c
 *
 * Created: 4/14/2021 11:47:42 PM
 * Author : anuj
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 38400
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include <avr/io.h>		
#include <util/delay.h>		
#include <inttypes.h>		
#include <stdlib.h>		
#include <stdio.h>	
#include "MPU6050_res_define.h"	
#include "I2C_Master_H_file.h"	
#include "uart.h"	


float Acc_x,Acc_y,Acc_z;

//Accelerometer Initialization
void Acc_Init()		
{
	_delay_ms(150);		
	I2C_Start_Wait(0xD0);	
	I2C_Write(SMPLRT_DIV);	//Write to sample rate register
	I2C_Write(0x07);	//1KHz sample rate
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);	//Write to power management register 
	I2C_Write(0x01);	//X axis gyroscope reference frequency 
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);	//Write to Configuration register 
	I2C_Write(0x00);	
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);	//Write to Gyro configuration register
	I2C_Write(0x18);	
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);	//Write to interrupt enable register
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);	 //I2C start with device write address
	I2C_Write(ACCEL_XOUT_H); 
	I2C_Repeated_Start(0xD1);
}

void Read_RawValue()
{
	MPU_Start_Loc();									
	Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	I2C_Stop();
}

int main(void)
{
	char float_x[10], float_y[10], float_z[10], data[8];
	float Xa,Ya,Za;
	I2C_Init();		
	Acc_Init();		
	UART_init(BAUD_PRESCALER);
	
	while(1)
	{
		Read_RawValue();

		//Divide raw value by sensitivity scale factor
		Xa = Acc_x/16384.0;
		Ya = Acc_y/16384.0;
		Za = Acc_z/16384.0;

		dtostrf( Xa, 3, 2, float_x );
		dtostrf( Ya, 3, 2, float_y );
		dtostrf( Za, 3, 2, float_z );
		
		//mapping speed
		if (Ya < -0.2 && Ya > -0.35 && Xa > -0.2 && Xa < 0.2) {
			sprintf(data, "f1");
		} else if (Ya <= -0.35 && Ya > -0.50 && Xa > -0.2 && Xa < 0.2) {
			sprintf(data, "f2");
		} else if (Ya <= -0.5 && Xa > -0.2 && Xa < 0.2) {
			sprintf(data, "f3");
		} else if (Ya > -0.2 && Ya < 0.2 && Xa < -0.2) {
			sprintf(data, "r1");
		} else if (Ya <= -0.35 && Ya > -0.50 && Xa < -0.2) {
			sprintf(data, "r2");
		} else if (Ya <= -0.5 && Xa < -0.2) {
			sprintf(data, "r3");
		} else if (Ya > 0.2 && Ya < 0.35 && Xa > -0.2 && Xa < 0.2) {
			sprintf(data, "b1");
		} else if (Ya >= 0.35 && Ya < 0.50 && Xa > -0.2 && Xa < 0.2) {
			sprintf(data, "b2");
		} else if (Ya >= 0.5 && Xa > -0.2 && Xa < 0.2) {
			sprintf(data, "b3");
		} else if (Ya > -0.2 && Ya < 0.2 && Xa > 0.2) {
			sprintf(data, "l1");
		} else if (Ya <= -0.35 && Ya > -0.50 && Xa > 0.2) {
			sprintf(data, "l2");
		} else if (Ya <= -0.5 && Xa > 0.2) {
			sprintf(data, "l3");
		} else if (Ya >= 0.35 && Ya < 0.50 && Xa > 0.2) {
			sprintf(data, "d2");
		} else if (Ya >= 0.5 && Xa > 0.2) {
			sprintf(data, "d3");
		} else if (Ya >= 0.35 && Ya < 0.50 && Xa < -0.2) {
			sprintf(data, "g2");
		} else if (Ya >= 0.5 && Xa < -0.2) {
			sprintf(data, "g3");
		} else {
			sprintf(data, "st");
		}
		//bluetooth communication
		UART_putstring(data);
		
		
		_delay_ms(50);

	}
}

