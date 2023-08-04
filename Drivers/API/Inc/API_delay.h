/**
 * @file API_delay.h
 * @author Fernando Nicolas Calvet (fernando.n.calvet@gmail.com)
 * @brief Modulo que implementa funciones para que los delay no bloqueantes
 * @version 0.1
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef API_DELAY_H
#define API_DELAY_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"	//ES ESTO RECOMENDABLE??? YA NO ES ESTA DENTRO DE LAS LIBRERIAS STANDARD, HARIA AL MODULO POCO PORTABLE?

typedef uint32_t tick_t; // stdint

typedef bool bool_t;	  // stdbool

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

typedef struct{
  uint16_t pin;
  GPIO_TypeDef *port;
  delay_t *delay;
} ledConf_t;

/**
 * @brief Delay struct inicialization
 *        Note: call it allways before any other ralated delay function
 *
 * @param delay pointer to delay_t struct
 * @param duration delay duration in [ms]
 */
void delayInit( delay_t * delay, tick_t duration );

/**
 * @brief Read if configurated delay is completed
 *
 * @param delay point to delay struct,
 * @return bool_t true if completed, false if not
 */
bool_t delayRead( delay_t * delay );

/**
 * @brief Change existing delay
 *
 * @param delay points to delay_t struct
 * @param duration delay duration in [ms]
 */
void delayWrite( delay_t * delay, tick_t duration );

#endif //API_DELAY_H
