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
	int delay;
	MPU6050_errorstatus err;
	float data;
	gpio_init();
	tim_init();
	uart_init();
	i2c_init();

	/* MPU6050 Initialization/Configuration
	 * For changing configuration parameters, go to mpu6050.c
	 */
	//burek = MPU6050_Set_Clock();

	err = MPU6050_Initialization();
    while(1)
    {
    	data = MPU6050_Get_Gyro_Data();

    }
}
