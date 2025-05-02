#ifndef I2CVERBINDING_H
#define I2CVERBINDING_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

#define SDA PB7
#define SCL PB6

void I2Csetup(int adres);
void requestdata();

#endif
