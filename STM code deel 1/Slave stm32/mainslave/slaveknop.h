#ifndef SLAVEKNOP_H
#define SLAVEKNOP_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

int Slave1_Knop_Waarde = 0;

void knopSetup();
int leesstatus();

#endif
