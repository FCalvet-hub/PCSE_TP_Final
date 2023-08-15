#ifndef API_DEBOUNCE_H
#define API_DEBOUNCE_H

#include <stdint.h>
#include <stm32f4xx_hal.h>
#include <stdbool.h>
#include "API_delay.h"

typedef struct {
  GPIO_TypeDef *port;      /*!< Button pin port */
  uint16_t pin;           /*!< Button pin */
  delay_t delay;          /*!< Debounce delay */
  uint8_t state;          /*!< Button state */
  void (*released)(void); /*!< Callback to function when button is released */
  void (*pressed)(void);  /*!< Callback to function when button is pressed */
} debounce_t;

/**
 * Initialize the debounce struct.
 * @param debounce The debounce struct
 */
void debounceFSM_init(debounce_t *debounce);

/**
 * Update the state of the button.
 * @param debounce The debounce struct
 */
void debounceFSM_update(debounce_t *debounce);

/**
 * Register a callback function to be called when the button is released.
 * @param debounce The debounce struct
 * @param callback The callback function to be called
 */
void debounceFSM_register_released(debounce_t *debounce, void (*callback)(void));

/**
 * Register a callback function to be called when the button is pressed.
 * @param debounce The debounce struct
 * @param callback The callback function to be called
 */
void debounceFSM_register_pressed(debounce_t *debounce, void (*callback)(void));

#endif