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
	uint16_t xdata;
	uint16_t ydata;
	uint16_t zdata;
	gpio_init();
	tim_init();
	uart_init();
	i2c_init();

	err = MPU6050_Initialization();
    while(1)
    {
    	err = MPU6050_Get_Accel_Data(&xdata, &ydata, &zdata);
    	printf("%i  ", xdata);
    	printf("%i  ", ydata);
    	printf("%i\n", zdata);

    }
}
