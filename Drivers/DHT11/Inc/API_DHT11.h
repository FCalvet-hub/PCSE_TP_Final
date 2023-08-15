/**
 * @file API_DHT11.h
 * @author Fernando Nicolas Calvet (fernando.n.calvet@gmail.com)
 * @brief Biblioteca para la lectura de datos del sensor DHT11.
 *
 * La biblioteca proporciona funciones para la inicialización, lectura de datos y
 * obtención de la temperatura y humedad del sensor DHT11.
 *
 * @version 0.1
 * @date 2023-08-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef API_INC_API_DHT11_H_
#define API_INC_API_DHT11_H_

#include "port.h"

/**
 * @brief Estructura para almacenar la temperatura del sensor DHT.
 *
 * La estructura contiene dos campos:
 * enteros: Los enteros de la temperatura.
 * decimales: Los decimales de la temperatura.
 */
typedef struct
{
	uint8_t enteros;
	uint8_t decimals;
} dhtTemp_t;

/**
 * @brief Función que implementa el FSM del sensor DHT.
 *
 * La función se encarga de controlar el estado del sensor DHT y de realizar las acciones necesarias en cada estado.
 */
void dhtFSM_update(void);

/**
 * @brief Función para iniciar la comunicación con el sensor DHT.
 */
void dhtStartComm(void);

/**
 * @brief Función para inicializar el sensor DHT.
 */
void dhtInit();

/**
 * @brief Función para obtener la temperatura del sensor DHT.
 *
 * @return dhtTemp_t Estructura con los datos de la temperatura.
 */
dhtTemp_t dhtGetTemp(void);

/**
 * @brief Función para obtener la humedad del sensor DHT.
 *
 * @return uint8_t Humedad del sensor DHT.
 */
uint8_t dhtGetHum(void);

void dhtFallingEdgePinISRHandler(void);

#endif /* API_INC_API_DHT11_H_ */
