/*
 * I2c_slave.c
 *
 *  Created on: May 14, 2025
 *      Author: kevin
 */

#include "main.h"
#include "i2c_slave.h"

#define RxSIZE 6
uint8_t RxData[RxSIZE];
int count =0;
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
	if(TransferDirection == I2C_DIRECTION_TRANSMIT){
		HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData,  RxSIZE, I2C_FIRST_AND_LAST_FRAME);
	}
	else{
		Error_Handler();
	}

}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
count++;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}
