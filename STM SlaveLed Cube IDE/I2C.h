#ifndef I2c_H
#define I2c_H

#pragma once

#include "I2c.h"
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

void LedSlave(); //slave code opstarten om de led te laten branden
void I2CSetup();

#endif
