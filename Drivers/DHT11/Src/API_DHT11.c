/*
 * API_DHT11.c
 *
 *  Created on: 31 jul. 2023
 *      Author: fcalvet
 */

#include "API_DHT11.h"
#include "port.h"
#include <stdbool.h>
#include <stdio.h>

#define DELAY_START_COMM 18	  // uS
#define TIMEOUT_RX_DATA 20	  // uS
#define START_MCU_TIME 18	  // uS
#define DHT_TIME_THRESHOLD 85 // uS
#define DHT_DATA_BIT_LEN 40	  // uS

typedef union
{
	/* Estructura de datos del sensor DHT */
	struct
	{
		uint8_t checksum; // Checksum
		uint8_t t_dec;	  // Temperatura decimal
		uint8_t t_int;	  // Temperatura entero
		uint8_t h_dec;	  // Humedad decimal
		uint8_t h_int;	  // Humedad entero
	};
	/* Representación de los datos como un número de 64 bits */
	uint64_t raw;
} dhtDataFormat_t;

typedef enum
{
	/* Estados del FSM del DHT */
	INIT,			 // Inicialización
	IDLE,			 // Inactivo
	STARTCOMM_INIT,	 // Inicialización de la comunicación
	STARTCOMM_DELAY, // Retardo de la comunicación
	STARTCOMM_END,	 // Fin de la comunicación
	RX_DATA,		 // Recepción de datos
	EVAL_DATA		 // Evaluación de datos
} FSM_states;

static uint8_t state;
static dhtDataFormat_t dhtStoredData;

volatile dhtDataFormat_t dhtBinData;
volatile uint8_t contador = 0;

void dhtInit()
{
	// pinInit();

	/* Configuramos el pin como salida */
	configPinAsOutput();

	/* Primer estado de la FSM */
	state = INIT;
}

void dhtStartComm(void)
{
	// Si ya esta corriendo la FSM, no volvemos a inicializar
	if (state == IDLE)
	{
		state = STARTCOMM_INIT;
	}
}

/**
 * @brief Función para verificar si el checksum de los datos del sensor DHT es correcto.
 *
 * @param data Datos del sensor DHT.
 *
 * @return true si el checksum es correcto, false en caso contrario.
 */
bool isDhtChecksumOk()
{
	uint8_t chsum = 0; // Suma de comprobación

	// Calculamos la suma de comprobación de los datos
	chsum += dhtBinData.t_int;
	chsum += dhtBinData.t_dec;
	chsum += dhtBinData.h_int;
	chsum += dhtBinData.h_dec;

	// Comparamos la suma de comprobación calculada con la suma de comprobación del sensor
	return (dhtBinData.checksum == chsum);
}

void storeData()
{
	dhtStoredData.t_int = dhtBinData.t_int;
	dhtStoredData.t_dec = dhtBinData.t_dec;
	dhtStoredData.h_int = dhtBinData.h_int;
}

/**
 * @brief Función que implementa el FSM del sensor DHT.
 *
 * La función se encarga de controlar el estado del sensor DHT y de realizar las acciones necesarias en cada estado.
 *
 * @param void
 *
 * @return void
 */
void dhtFSM_update(void)
{
	static uint32_t last_time, countdown;

	switch (state)
	{
	case INIT:
		// Configuramos el pin del GPIO como salida y lo ponemos a nivel alto
		configPinAsOutput();
		setPin();
		state = IDLE;
		break;

	case IDLE:
		// Esperando...
		break;

	case STARTCOMM_INIT:
		state = STARTCOMM_DELAY;
		// Reseteamos el contador
		contador = 0;
		// Ponemos el pin del GPIO a nivel bajo
		resetPin();
		// Almacenamos el tiempo actual
		last_time = getTick();
		break;

	case STARTCOMM_DELAY:
		// Calculamos el tiempo transcurrido desde el último estado
		countdown = getTick() - last_time;
		// Si el tiempo transcurrido es mayor que el retardo de inicio de comunicación,
		// cambiamos al estado STARTCOMM_END
		if ((countdown) >= DELAY_START_COMM)
		{
			setPin();
			dhtBinData.raw = 0;
			configPinAsItInput();
			// Reiniciamos el contador del temporizador
			setTimerCounter(0);
			state = STARTCOMM_END;
		}
		break;

	case STARTCOMM_END:
		// Almacenamos el tiempo actual
		last_time = getTick();

		state = RX_DATA;
		break;

	case RX_DATA:
		// Calculamos el tiempo transcurrido desde el último estado
		countdown = getTick() - last_time;
		// Si ya recibimos todos los datos o se termino el tiempo para recibirlos
		if (((countdown) >= TIMEOUT_RX_DATA))
		{ //|| (contador >= DHT_DATA_BIT_LEN)
			state = EVAL_DATA;
		}
		break;

	case EVAL_DATA:
		// Si el checksum del dato binario es correcto,
		// almacenamos los datos
		if (isDhtChecksumOk())
		{
			storeData();
		}
		else
		{
			// ERROR
		}
		storeData();

		state = INIT;
		break;

	default:
		state = INIT;
		break;
	}
}

dhtTemp_t dhtGetTemp(void)
{
	dhtTemp_t temp;

	temp.enteros = dhtStoredData.t_int;
	temp.decimals = dhtStoredData.t_dec;

	return temp;
}

uint8_t dhtGetHum(void)
{
	return dhtStoredData.h_int;
}

volatile uint32_t aux = 0;
/*
 * Función de callback que se invoca cuando se produce una interrupción en el GPIO
 *
 * Parámetros:
 *   GPIO_Pin: Pin del GPIO que ha generado la interrupción.
 */
void dhtFallingEdgePinISRHandler(void)
{
	// Desplazamos el bit mas significativo del dato binario a la izquierda
	dhtBinData.raw <<= 1;
	aux = getTimerCouter();
	// Si el valor del temporizador es mayor que DHT_TIME_THRESHOLD, el bit más significativo del dato binario se establece a 1,
	// en caso contrario se establece a 0
	dhtBinData.raw |= (getTimerCouter() > DHT_TIME_THRESHOLD) ? 1 : 0;

	// Incrementamos el contador
	contador++;
	// Reiniciamos el contador del temporizador
	setTimerCounter(0);
}
