#include "Encoder.h"
#include "main.h"
int counterLezen() {
    int waarde = __HAL_TIM_GET_COUNTER(&htim2);

    if (waarde > 127) {
        waarde = 127;
        __HAL_TIM_SET_COUNTER(&htim2, 127);
    }
    if (waarde < 2) {
        waarde = 2;
        __HAL_TIM_SET_COUNTER(&htim2, 6);
    }

    rawCounter = waarde;
    return waarde;
}
int encoderKnopLezen(){
	knopStatus = !HAL_GPIO_ReadPin(Knop_GPIO_Port, Knop_Pin);
	if(knopStatus == 1){
		HAL_Delay(100);
		knopStatus = !HAL_GPIO_ReadPin(Knop_GPIO_Port, Knop_Pin);
	}

	return knopStatus;
}
int servoKnopLezen(){
	servoKnop = !HAL_GPIO_ReadPin(servoKnop_GPIO_Port, servoKnop_Pin);
	return servoKnop;
	}
