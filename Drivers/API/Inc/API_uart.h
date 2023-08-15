/**
 * @file API_uart.h
 * @author Fernando Nicolas Calvet (fernando.n.calvet@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef API_UART_H
#define API_UART_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Inicializa el periférico UART.
 *
 * @return `true` si se inicializó correctamente, `false` en caso contrario.
 */
bool uartInit();

/**
 * @brief Envía una cadena de caracteres completa.
 *
 * @param pstring Puntero a la cadena de caracteres.
 */
void uartSendString(uint8_t * pstring);

/**
 * @brief Envía una cadena de caracteres de un tamaño especificado.
 *
 * @param pstring Puntero a la cadena de caracteres.
 * @param size Tamaño de la cadena de caracteres.
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size);

/**
 * @brief Recibe una cadena de caracteres de un tamaño especificado.
 *
 * @param pstring Puntero a la cadena de caracteres.
 * @param size Tamaño de la cadena de caracteres.
 */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

#endif
