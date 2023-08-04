/*
 * API_DHT11.h
 *
 *  Created on: 31 jul. 2023
 *      Author: fcalvet
 */

#ifndef API_INC_API_DHT11_H_
#define API_INC_API_DHT11_H_

#include "stm32f4xx_hal.h"


void dhtFSM(void);
void dhtStartComm(void);
void dhtInit(GPIO_TypeDef* GPIO_port, uint16_t GPIO_Pin, IRQn_Type PIN_EXTI_IRQn, TIM_HandleTypeDef *TIMHandle);


#endif /* API_INC_API_DHT11_H_ */
