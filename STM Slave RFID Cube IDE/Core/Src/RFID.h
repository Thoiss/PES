#ifndef RFID_H
#define RFID_H

#include "stm32l4xx_hal.h"
#include "MFRC522.h"
#include <string.h>
#include <stdio.h>

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;

void scanPas();


#endif
