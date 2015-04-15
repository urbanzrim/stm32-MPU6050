/**
 * @file main.c
 * @brief Main program for application
 *
 * @author Urban Zrim
 * @date 7.4.2015
 *
 *
 *  --------------------------------------------------------------------------------
 *  Copyright (c) 2015, Urban Zrim
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in
 *	all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 *  --------------------------------------------------------------------------------
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
	//tim_init(); // Timers are set in the dboardsetup.h for quadcopter
	uart_init();
	i2c_init();
	uint8_t str[25];

	err = MPU6050_Initialization();

    while(1)
    {
    	/* Read data from the sensor */
		err = MPU6050_Get_Gyro_Data(&gyro_xdata, &gyro_ydata, &gyro_zdata);
		err = MPU6050_Get_Accel_Data(&accel_xdata, &accel_ydata, &accel_zdata);

		sprintf(str, "%f", gyro_xdata);
		printf("gyroX: %s", gyro_xdata);
    }
}
