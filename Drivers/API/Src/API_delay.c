
#include "API_delay.h"

/**
 * @brief  This function is executed in case of error occurrence.
 */
static void Error_Handler(void)
{
  __disable_irq();
  //TODO: Prender led?
  while (1)
  {
  }
}

void delayInit(delay_t *delay, tick_t duration)
{
  /* ASSERT */
  if(delay == NULL || duration <= 0){
    Error_Handler();
  }

  delay->duration = duration;
  delay->running = false;
  delay->startTime = 0;
}

bool_t delayRead(delay_t *delay)
{
  /* ASSERT */
  if(delay == NULL){
    Error_Handler();
  }

  tick_t timeDifference;
  if (delay->running == false)
  {
    delay->running = true;
    delay->startTime = HAL_GetTick();
    return false;
  }
  else
  {
    /*diferencias entre tiempo actual y el guardado como comienzo*/
    timeDifference = HAL_GetTick() - delay->startTime;
    if (timeDifference < delay->duration)
    {
      return false;
    }
    else
    {
      delay->running = false;
      return true;
    }
  }
}

void delayWrite(delay_t *delay, tick_t duration)
{
  /* ASSERT */
  if(delay == NULL || duration <= 0){
    Error_Handler();
  }  

  delay->duration = duration;
}
