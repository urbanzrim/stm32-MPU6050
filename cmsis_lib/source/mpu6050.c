/*
 * mpu6050.c
 *
 *  Created on: 7. apr. 2015
 *      Author: Urban Zrim
 */

#include "mpu6050.h"

uint32_t MPU6050_Timeout = MPU6050_FLAG_TIMEOUT;

void MPU6050_Test(void){

	uint8_t tmp = 0;
	uint8_t buf[100];

	tmp = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, WHO_AM_I, buf, 1);
	if(buf[0] == (uint8_t)0x68) test = 1;
}

MPU6050_errorstatus MPU6050_Read(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{

	/* Test if SDA line busy */
	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
	{
		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
	}

	I2C_TransferHandling(I2C1, SlaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
	{
		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
	}

	if(NumByteToRead>1)
	RegAddr |= 0x80;

	/* Send Register address */
	I2C_SendData(I2C1, (uint8_t)RegAddr);

	/* Wait until TC flag is set */
	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
	{
		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_TX_ERROR;
	}

    I2C_TransferHandling(I2C1, SlaveAddr, NumByteToRead, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    /* Wait until all data are received */
    while (NumByteToRead)
    {
    	/* Wait until RXNE flag is set */
    	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
    	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
    	{
    		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_RX_ERROR;
    	}

    	*pBuffer = I2C_ReceiveData(I2C1);
    	pBuffer++;

    	NumByteToRead--;
    }

    /* Wait until STOP flag is set */
    MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
    {
      if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
    }

    /* Clear STOPF flag */
    I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);

    /* If all operations OK */
    return MPU6050_NO_ERROR;
}

/*
 * @brief Writes bytes to MPU6050
 * @retval @MPU6050_errorstatus
 */
MPU6050_errorstatus MPU6050_Write(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t* pBuffer)
{

	/* Test if SDA line busy */
	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
	{
		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(I2C1, SlaveAddr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
	{
		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
	}

	/* Send Register address */
	I2C_SendData(I2C1, (uint8_t) RegAddr);

	/* Wait until TCR flag is set */
	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
	{
		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(I2C1, SlaveAddr, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);

	/* Wait until TXIS flag is set */
	MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
	{
		if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
	}

	/* Write data to TXDR */
	I2C_SendData(I2C1, *pBuffer);

    /* Wait until STOPF flag is set */
    MPU6050_Timeout = MPU6050_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
    {
      if((MPU6050_Timeout--) == 0) return MPU6050_I2C_ERROR;
    }

    /* Clear STOPF flag */
    I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);

	return MPU6050_NO_ERROR;
}



