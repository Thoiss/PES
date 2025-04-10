#include "I2c.h"

void LedSlave() {
	uint8_t data = 0;

	while(1) {
		HAL_I2C_Slave_Receive(&hi2c1, &data, 1, HAL_MAX_DELAY);
		if (data == 1) {
		HAL_GPIO_WritePin(LEDJE_GPIO_Port, LEDJE_Pin, 1);
		HAL_Delay(500);
		}
		if (data == 0) {
		HAL_GPIO_WritePin(LEDJE_GPIO_Port, LEDJE_Pin, 0);
		HAL_Delay(500);
		}

	}

}
