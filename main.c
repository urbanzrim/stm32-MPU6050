/**
 * @file main.c
 * @brief Main program for application
 *
 * @author Urban Zrim
 * @date 16.4.2015
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

#include <math.h>
#include <stdio.h>
#include "dboardsetup.h"
#include "mpu6050.h"

MPU6050_errorstatus err;
int16_t x;
int16_t y;
int16_t z;
float gyro_xdata;
float gyro_ydata;
float gyro_zdata;
float accel_xdata;
float accel_ydata;
float accel_zdata;
float pitch = 0;
float roll = 0;
float pitchAcc = 0;
float rollAcc = 0;
char str[10];
int i = 0;


int main(void)
{
	gpio_init();
	int_init();
	tim_init();
	uart_init();
	i2c_init();

	err = MPU6050_Initialization();
    while(1)
    {
    	if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
    		    {
    				TIM_ClearFlag(TIM1, TIM_IT_Update);
    				err = MPU6050_Get_Gyro_Data(&gyro_xdata, &gyro_ydata, &gyro_zdata);
    				err = MPU6050_Get_Accel_Data(&accel_xdata, &accel_ydata, &accel_zdata);
    		    }

    	pitch += gyro_xdata/100;
    	roll += gyro_ydata/100;
		pitchAcc = (float)(atan2((accel_xdata), (accel_zdata)))*(float)(180/PI_NUM);
		rollAcc = (float)(atan2((accel_ydata), (accel_zdata)))*(float)(180/PI_NUM);
		pitch = pitch*0.97 + pitchAcc*0.03;
		roll = roll*0.97 + rollAcc*0.03;

		sprintf(str, "%f\n", roll);
		printf("%s", str);
    }
}
