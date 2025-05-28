#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_
#include "stm32l4xx_hal.h"
extern TIM_HandleTypeDef htim2;
extern uint8_t RX_Buffer[1];
extern uint32_t rawCounter;
extern uint32_t delay;
extern uint8_t knopStatus;
extern uint8_t servoKnop;

int counterLezen();
int encoderKnopLezen();
int servoKnopLezen();


#endif /* INC_ENCODER_H_ */
