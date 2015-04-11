/*
 * mpu6050.c
 *
 *  Created on: 7. apr. 2015
 *      Author: Urban Zrim
 */

#include "mpu6050.h"

uint32_t MPU6050_Timeout = MPU6050_FLAG_TIMEOUT;


MPU6050_errorstatus MPU6050_Initialization(void){

	MPU6050_errorstatus errorstatus;

	/* Set Clock source for the chip
	 * possible values @pwr_mngt_1
	 */
	errorstatus = MPU6050_Set_Clock(MPU6050_PLL_X_GYRO);
	if(errorstatus != 0) return errorstatus;

	/* Set Gyroscope's full scope range
	 * possible values @gyro_scale_range
	 */
	errorstatus = MPU6050_Gyro_Set_Range(MPU6050_GYRO_250);
	if(errorstatus != 0) return errorstatus;

	/* Set Accelerometer's full scope range
	 * possible values @accel_scale_range
	 */
	errorstatus = MPU6050_Accel_Set_Range(MPU6050_ACCEL_2g);
	if(errorstatus != 0) return errorstatus;

	return MPU6050_NO_ERROR;
}

MPU6050_errorstatus MPU6050_Test(void){

	MPU6050_errorstatus errorstatus;
	uint8_t tmp;

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, WHO_AM_I, &tmp, 1);
	if(tmp != (uint8_t)0x68){
		return errorstatus;
	}
	return MPU6050_NO_ERROR;
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

uint8_t MPU6050_Gyro_Get_Range(void){

	MPU6050_errorstatus errorstatus;
	uint8_t tmp;

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, GYRO_CONFIG, &tmp, 1);
	if(errorstatus != 0){
		return 1;
	}
	else return tmp;

}

MPU6050_errorstatus MPU6050_Gyro_Set_Range(MPU6050_Gyro_Range range){

	MPU6050_errorstatus errorstatus;

	errorstatus = MPU6050_Write((MPU6050_ADDRESS & 0x7f) << 1, GYRO_CONFIG, &range);
	if(errorstatus != 0){
		return errorstatus;
	}
	else return MPU6050_NO_ERROR;

}

uint8_t MPU6050_Accel_Get_Range(void){

	MPU6050_errorstatus errorstatus;
	uint8_t tmp;

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_CONFIG, &tmp, 1);
	if(errorstatus != 0){
		return 1;
	}
	else return tmp;

}

MPU6050_errorstatus MPU6050_Accel_Set_Range(MPU6050_Accel_Range range){

	MPU6050_errorstatus errorstatus;

	errorstatus = MPU6050_Write((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_CONFIG, &range);
	if(errorstatus != 0){
		return errorstatus;
	}
	else return MPU6050_NO_ERROR;

}

MPU6050_errorstatus  MPU6050_Set_Clock(MPU6050_Clock_Select clock){

	MPU6050_errorstatus errorstatus;

	errorstatus = MPU6050_Write((MPU6050_ADDRESS & 0x7f) << 1, PWR_MGMT_1, &clock);
	if(errorstatus != 0){
		return errorstatus;
	}
	else return MPU6050_NO_ERROR;

}

uint16_t MPU6050_Get_Temperature(void){

	MPU6050_errorstatus errorstatus;
	uint8_t temp_low;
	uint8_t temp_high;
	uint16_t temp;

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, TEMP_OUT_L, &temp_low, 1);
	if(errorstatus != 0){
		return 1;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, TEMP_OUT_H, &temp_high, 1);
	if(errorstatus != 0){
		return 1;
	}

	temp = temp_low + temp_high;

	return temp;

}

MPU6050_errorstatus MPU6050_Get_Gyro_Data(uint16_t* X, uint16_t* Y, uint16_t* Z){

	MPU6050_errorstatus errorstatus;

	uint8_t xlow, xhigh, ylow, yhigh, zlow, zhigh;

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, GYRO_XOUT_L, &xlow, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, GYRO_XOUT_H, &xhigh, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, GYRO_YOUT_L, &ylow, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, GYRO_YOUT_H, &yhigh, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, GYRO_ZOUT_L, &zlow, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, GYRO_ZOUT_H, &zhigh, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	*X = xlow + xhigh;
	*Y = ylow + yhigh;
	*Z = zlow + zhigh;

	return MPU6050_NO_ERROR;
}

MPU6050_errorstatus MPU6050_Get_Accel_Data(uint16_t* X, uint16_t* Y, uint16_t* Z){

	MPU6050_errorstatus errorstatus;

	uint8_t xlow, xhigh, ylow, yhigh, zlow, zhigh;

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_XOUT_L, &xlow, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_XOUT_H, &xhigh, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_YOUT_L, &ylow, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_YOUT_H, &yhigh, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_ZOUT_L, &zlow, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	errorstatus = MPU6050_Read((MPU6050_ADDRESS & 0x7f) << 1, ACCEL_ZOUT_H, &zhigh, 1);
	if(errorstatus != 0){
		return errorstatus;
	}

	*X = xlow + xhigh;
	*Y = ylow + yhigh;
	*Z = zlow + zhigh;

	return MPU6050_NO_ERROR;
}





