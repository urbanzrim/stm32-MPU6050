/*
 * main.c
 *
 *  Created on: 7. apr. 2015
 *      Author: Urban Zrim
 */

#include <stdio.h>
#include "dboardsetup.h"
#include "mpu6050.h"

int main(void)
{
	MPU6050_errorstatus err;
	float gyro_xdata;
	float gyro_ydata;
	float gyro_zdata;
	float accel_xdata;
	float accel_ydata;
	float accel_zdata;
	gpio_init();
	//tim_init();
	uart_init();
	i2c_init();
	int16_t str_gyro[200];
	int16_t str_accel[200];
	int i = 0;

	err = MPU6050_Initialization();

    while(1)
    {

    	while(i<200){
			err = MPU6050_Get_Gyro_Data(&gyro_xdata, &gyro_ydata, &gyro_zdata);
			err = MPU6050_Get_Accel_Data(&accel_xdata, &accel_ydata, &accel_zdata);
			str_gyro[i] = gyro_xdata;
			str_accel[i] = accel_xdata;
			i++;
    	}

    }
}
