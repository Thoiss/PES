#include "RFID.h"

uint8_t status;
	uint8_t str[16];
	uint8_t sNum[5];
	char *msg1 = "Reading From Card\r\n";
	char *msg2 = "Reading From Tag\r\n";
	char *msg3 = "Place card to read\r\n";

uint8_t Data = 0; //variabele voor RFID Data
char revbuf[50];

void scanPas() {

status = MFRC522_Request(PICC_REQIDL, str);
	  	 if (status == MI_OK) {
	  		status = MFRC522_Anticoll(str);
	  		memcpy(sNum, str, 5);
	  		HAL_Delay(200);
	  		{
	  	int len = snprintf(revbuf, sizeof revbuf, "\r\n RFID Waarde: %d %d %d %d %d\n", sNum[0], sNum[1], sNum[2], sNum[3], sNum[4]);
	  	HAL_UART_Transmit(&huart2, (uint8_t*)revbuf, len, HAL_MAX_DELAY);

	  	  }
	  	 HAL_Delay(800);
	    }
}
