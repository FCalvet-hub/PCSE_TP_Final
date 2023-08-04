/*
 * API_DHT11.c
 *
 *  Created on: 31 jul. 2023
 *      Author: fcalvet
 */


#include "API_DHT11.h"
#include <stdbool.h>
#include "stm32f4xx_ll_tim.h"
#include <stdio.h>

#define DELAY_START 18

typedef struct{
  uint16_t pin;
  GPIO_TypeDef *port;
  IRQn_Type EXTI_IRQn;
  TIM_HandleTypeDef *htim;
} dhtConf_t;

static dhtConf_t Dht;

static void configPinAsOutput(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_NVIC_DisableIRQ(Dht.EXTI_IRQn);
	GPIO_InitStruct.Pin = Dht.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Dht.port, &GPIO_InitStruct);
}

static void configPinAsItInput(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_GPIO_EXTI_CLEAR_IT(Dht.pin);
	GPIO_InitStruct.Pin = Dht.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(Dht.port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(Dht.EXTI_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(Dht.EXTI_IRQn);
}

void dhtInit(GPIO_TypeDef* GPIO_port, uint16_t GPIO_Pin, IRQn_Type PIN_EXTI_IRQn, TIM_HandleTypeDef *TIMHandle)
{
	Dht.port = GPIO_port;
	Dht.pin = GPIO_Pin;
	Dht.EXTI_IRQn = PIN_EXTI_IRQn;
	Dht.htim = TIMHandle;
	configPinAsOutput();
	HAL_TIM_Base_Start(Dht.htim);
}

typedef union  {
    struct {
        uint8_t checksum;
        uint8_t t_dec;
        uint8_t t_int;
        uint8_t h_dec;
        uint8_t h_int;
    };
    uint64_t raw;
}dhtDataFormat_t;


typedef enum {
	IDLE,
	STARTCOMM_INIT,
	STARTCOMM_DELAY,
	STARTCOMM_END,
	RX_DATA,
	EVAL_DATA
}FSM_states;

static uint8_t state;
static bool isRunning=0;

#define START_MCU_TIME 18 //uS

void dhtStartComm(void){
	if(!isRunning){
		state = STARTCOMM_INIT;
	}
}

bool isDhtChecksumOk(dhtDataFormat_t data)
{
	return (data.t_int + data.t_dec + data.h_int + data.h_dec) == data.checksum;
}

storeData(dhtDataFormat_t data){

}


volatile dhtDataFormat_t dhtBinData;


static volatile uint8_t contador = 0;
static volatile uint16_t dhtTimer[50];

void dhtFSM(void){
	static uint32_t last_time, countdown;

	switch (state) {
		case IDLE:
			configPinAsOutput();
			HAL_GPIO_WritePin(Dht.port, Dht.pin, GPIO_PIN_SET);

			break;
		case STARTCOMM_INIT:
			state = STARTCOMM_DELAY;
			contador = 0;
			HAL_GPIO_WritePin(Dht.port, Dht.pin, GPIO_PIN_RESET);
			last_time = HAL_GetTick();
			break;
		case STARTCOMM_DELAY:
			countdown = HAL_GetTick() - last_time;
			if((countdown) >= DELAY_START)
			{
				HAL_GPIO_WritePin(Dht.port, Dht.pin, GPIO_PIN_SET);
				dhtBinData.raw = 0;
				configPinAsItInput();
				LL_TIM_SetCounter(Dht.htim->Instance,0);
				state = STARTCOMM_END;
			}
			break;
		case STARTCOMM_END:
			last_time = HAL_GetTick();
			state = RX_DATA;
			break;
		case RX_DATA:
			countdown = HAL_GetTick() - last_time;
			if((countdown) >= 100)
			{
				state = IDLE;
				if(isDhtChecksumOk(dhtBinData))
				{
					storeData(dhtBinData);
				}

//				if(contador > 40){
//					HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//				}
//				printf("%08X\r\n", dhtBinData);

				//TODO: Hacer el checksum
			}
			break;
		case EVAL_DATA:

			break;
		default:
			break;
	}
}



typedef enum {
	RX_ACK,
	RX_BIT
}FSM_IRQ;

// EXTI Line9 External Interrupt ISR Handler CallBackFun
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Dht.pin) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
    	dhtBinData.raw <<= 1;
    	dhtBinData.raw |= (__HAL_TIM_GET_COUNTER(Dht.htim) > 85) ? 1 : 0;
    	//dhtTimer[contador] = __HAL_TIM_GET_COUNTER(Dht.htim);
    	//contador++;
    	LL_TIM_SetCounter(Dht.htim->Instance,0);
    }
}
