/*
 * main.c
 *
 *  Created on: 7. apr. 2015
 *      Author: Urban Zrim
 */

#include <stdio.h>
#include "dboardsetup.h"


int main(void)
{

	uint8_t i2c;
	gpio_init();
	tim_init();
	uart_init();
	i2c_init();

    while(1)
    {

    	MPU6050_Test();
    }
}
