/**
 * @file API_debounce.c
 * @author Fernando Nicolas Calvet (fernando.n.calvet@gmail.com)
 * @brief Debounce API
 * @version 0.1
 * @date 2023-07-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "API_debounce.h"

/**
 * @brief Enum for the button states
 */
typedef enum
{
    BUTTON_UP,
    BUTTON_FALLING,
    BUTTON_DOWN,
    BUTTON_RISING
} FSM_states_t;

/**
 * @brief Define the debounce duration
 */
#define DEBOUNCE_DURATION 40

/**
 * @brief This function is executed in case of error occurrence.
 */
static void Error_Handler(void)
{
    __disable_irq();
    // TODO: Prender led?
    while (1)
    {
    }
}

/**
 * @brief Callbacks point to this dummy function if NULL pointer is passed when initialization takes place
 */
static void dummy_function(void) {}

/**
 * @brief Register a function to be called when the button is released.
 * @param debounce The debounce struct
 * @param callback The function to be called
 */
void debounceFSM_register_released(debounce_t *debounce, void (*callback)(void))
{
    debounce->released = callback;

    if (debounce->released == NULL)
    {
        debounce->released = &dummy_function;
    }
}

/**
 * @brief Register a function to be called when the button is pressed.
 * @param debounce The debounce struct
 * @param callback The function to be called
 */
void debounceFSM_register_pressed(debounce_t *debounce, void (*callback)(void))
{
    debounce->pressed = callback;

    if (debounce->pressed == NULL)
    {
        debounce->pressed = &dummy_function;
    }
}

/**
 * @brief Initialize the debounce struct.
 * @param debounce The debounce struct
 */
void debounceFSM_init(debounce_t *debounce)
{
    if (debounce->pressed == NULL)
    {
        debounce->pressed = &dummy_function;
    }

    if (debounce->released == NULL)
    {
        debounce->released = &dummy_function;
    }

    delayInit(&debounce->delay, DEBOUNCE_DURATION);
    debounce->state = BUTTON_UP;
}

/**
 * @brief Update the state of the button.
 * @param debounce The debounce struct
 */
void debounceFSM_update(debounce_t *debounce)
{
    switch (debounce->state)
    {
    case BUTTON_UP:
        if (HAL_GPIO_ReadPin(debounce->port, debounce->pin) == 0)
        {
            debounce->state = BUTTON_FALLING;
            delayInit(&debounce->delay, DEBOUNCE_DURATION);
        }
        break;

    case BUTTON_FALLING:
        /* Debounce time finished? */
        if (delayRead(&debounce->delay))
        {
            /* Check if button is still down */
            if (HAL_GPIO_ReadPin(debounce->port, debounce->pin) == 0)
            {
                debounce->state = BUTTON_DOWN;
                /* Call the callback function */
                debounce->pressed();
            }
            else
            {
                debounce->state = BUTTON_UP;
            }
        }
        break;

    case BUTTON_DOWN:
        if (HAL_GPIO_ReadPin(debounce->port, debounce->pin) != 0)
        {
            debounce->state = BUTTON_RISING;
            delayInit(&debounce->delay, DEBOUNCE_DURATION);
        }
        break;

    case BUTTON_RISING:
        /*Debounce time finished? */
        if (delayRead(&debounce->delay))
        {
            /*check if button is still down*/
            if (HAL_GPIO_ReadPin(debounce->port, debounce->pin) != 0)
            {
                debounce->state = BUTTON_UP;
                /*callback to function*/
                debounce->released();
            }
            else
            {
                debounce->state = BUTTON_DOWN;
            }
        }
        break;
    default:
        Error_Handler();
        break;
    }
}
