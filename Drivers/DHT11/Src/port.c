/*
 * port.c
 *
 *  Created on: Aug 12, 2023
 *      Author: fert2
 */

#include "port.h"
#include "stm32f4xx_ll_tim.h"
#include "API_DHT11.h"
#include "main.h"

static dhtConf_t Dht; // Estructura de configuración del DHT

void Port_dhtPinInit(GPIO_TypeDef *GPIO_port, uint16_t GPIO_Pin,
					 IRQn_Type PIN_EXTI_IRQn, TIM_HandleTypeDef *TIMHandle)
{
	/* Inicialización de la estructura de configuración del DHT */
	Dht.port = GPIO_port;
	Dht.pin = GPIO_Pin;
	Dht.EXTI_IRQn = PIN_EXTI_IRQn;
	Dht.htim = TIMHandle;

	/* Arrancamos el temporizador */
	HAL_TIM_Base_Start(Dht.htim);
}

void configPinAsOutput(void)
{
	/* Configuración del pin como salida de tipo push-pull con velocidad baja */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = Dht.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Dht.port, &GPIO_InitStruct);

	/* Deshabilitamos la interrupción del GPIO */
	HAL_NVIC_DisableIRQ(Dht.EXTI_IRQn);
}

void configPinAsItInput(void)
{
	/* Configuración del pin como entrada con flanco de bajada y pull-up */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = Dht.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(Dht.port, &GPIO_InitStruct);

	/* Configuración de la interrupción del GPIO */
	HAL_NVIC_SetPriority(Dht.EXTI_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(Dht.EXTI_IRQn);
}

void setPin(void)
{
	HAL_GPIO_WritePin(Dht.port, Dht.pin, GPIO_PIN_SET);
}

void resetPin(void)
{
	HAL_GPIO_WritePin(Dht.port, Dht.pin, GPIO_PIN_RESET);
}

uint32_t getTick(void)
{
	return HAL_GetTick();
}

void setTimerCounter(uint32_t count)
{
	LL_TIM_SetCounter(Dht.htim->Instance, count);
}

uint32_t getTimerCouter(void)
{
	return __HAL_TIM_GET_COUNTER(Dht.htim);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == Dht.pin) // Si la fuente de interrupción es el pin de DHT
	{
		dhtFallingEdgePinISRHandler();
	}
}
