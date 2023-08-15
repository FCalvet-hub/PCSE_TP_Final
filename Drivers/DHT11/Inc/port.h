/*
 * port.h
 *
 *  Created on: Aug 13, 2023
 *      Author: fert2
 */

#ifndef API_INC_PORT_H_
#define API_INC_PORT_H_

#include <stm32f4xx_ll_tim.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>

/**
 * @brief Estructura que define la configuración del DHT
 *
 * @param pin Número del pin del GPIO
 * @param port Puerto del GPIO
 * @param EXTI_IRQn Interrupción del GPIO
 * @param htim Handle del temporizador
 */
typedef struct
{
	uint16_t pin;			 // Número del pin del GPIO
	GPIO_TypeDef *port;		 // Puerto del GPIO
	IRQn_Type EXTI_IRQn;	 // Interrupción del GPIO
	TIM_HandleTypeDef *htim; // Handle del temporizador
} dhtConf_t;

/**
 * @brief Inicializa el pin del DHT
 *
 * @param GPIO_port Puerto del GPIO
 * @param GPIO_Pin Número del pin del GPIO
 * @param PIN_EXTI_IRQn Interrupción del GPIO
 * @param TIMHandle Handle del temporizador
 */
void Port_dhtPinInit(GPIO_TypeDef *GPIO_port, uint16_t GPIO_Pin,
					 IRQn_Type PIN_EXTI_IRQn, TIM_HandleTypeDef *TIMHandle);

/**
 * @brief Configura el pin como salida
 */
void configPinAsOutput(void);

/**
 * @brief Configura el pin como entrada
 */
void configPinAsItInput(void);

/**
 * @brief Pone el pin a alto
 */
void setPin(void);

/**
 * @brief Pone el pin a bajo
 */
void resetPin(void);

/**
 * @brief Obtiene el valor del temporizador
 *
 * @return Valor del temporizador
 */
uint32_t getTick(void);

/**
 * @brief Setea el valor del contador del temporizador
 *
 * @param count Valor del contador
 */
void setTimerCounter(uint32_t count);

/**
 * @brief Obtiene el valor del contador del temporizador
 *
 * @return Valor del contador
 */
uint32_t getTimerCouter(void);

#endif /* API_INC_PORT_H_ */
