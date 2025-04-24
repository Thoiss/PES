#include "I2c.h"

void LedSlave() {

    while (1) {
    	uint8_t RX_Buffer = 0;

    		  if (HAL_I2C_Slave_Receive(&hi2c1, &RX_Buffer, 1, 1000) == HAL_OK) {
    		      char buffer[50];
    		      sprintf(buffer, "Ontvangen data: %d\r\n", RX_Buffer);

    		      if (RX_Buffer == 1) {
    		          HAL_GPIO_WritePin(LEDJE_GPIO_Port, LEDJE_Pin, GPIO_PIN_SET);
    		      } else if (RX_Buffer == 0) {
    		          HAL_GPIO_WritePin(LEDJE_GPIO_Port, LEDJE_Pin, GPIO_PIN_RESET);
    		      }
    		  }

    }
}

