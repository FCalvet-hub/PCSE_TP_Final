

//#include <stm32f4xx_hal_uart.h>
#include "API_uart.h"
#include <string.h>

static UART_HandleTypeDef huartApi;

#define UART_TX_TIMEOUT 1000

static void Error_Handler()
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}



bool uartInit(void)
{
    huartApi.Instance = USART2;
    huartApi.Init.BaudRate = 115200;
    huartApi.Init.WordLength = UART_WORDLENGTH_8B;
    huartApi.Init.StopBits = UART_STOPBITS_1;
    huartApi.Init.Parity = UART_PARITY_NONE;
    huartApi.Init.Mode = UART_MODE_TX_RX;
    huartApi.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huartApi.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huartApi) != HAL_OK)
    {
        //Error_Handler();
    	return false;
    }

    // Imprimir la configuración de USART
    char buf[100];
    snprintf(buf, sizeof(buf), "Configuración de USART:\r\n");
    HAL_UART_Transmit(&huartApi, (uint8_t *)buf, strlen(buf), UART_TX_TIMEOUT);
    snprintf(buf, sizeof(buf), "  Velocidad en baudios: %d\r\n", (int)huartApi.Init.BaudRate);
    HAL_UART_Transmit(&huartApi, (uint8_t *)buf, strlen(buf), UART_TX_TIMEOUT);
    snprintf(buf, sizeof(buf), "  Bits de datos: %d\r\n", 8);
    HAL_UART_Transmit(&huartApi, (uint8_t *)buf, strlen(buf), UART_TX_TIMEOUT);
    snprintf(buf, sizeof(buf), "  Bits de parada: %d\r\n", 1);
    HAL_UART_Transmit(&huartApi, (uint8_t *)buf, strlen(buf), UART_TX_TIMEOUT);
    snprintf(buf, sizeof(buf), "  Paridad: %d\r\n", 0);
    HAL_UART_Transmit(&huartApi, (uint8_t *)buf, strlen(buf), UART_TX_TIMEOUT);
    return true;
}

void uartSendString(uint8_t *pstring)
{
    if (pstring == NULL)
    {
        Error_Handler();
    }

    // TODO: Ver de implementar un manejador de errores en caso de que no este inicializada la variable

    HAL_UART_Transmit(&huartApi, pstring, strlen((char *)pstring), UART_TX_TIMEOUT);
}

void uartSendStringSize(uint8_t *pstring, uint16_t size)
{
    if (pstring == NULL)
    {
        Error_Handler();
    }

    //chequeo de largo correcto
    if (strlen( (char *)pstring) < size )
    {
        Error_Handler();
    }    

    HAL_UART_Transmit(&huartApi, pstring, size, UART_TX_TIMEOUT);
}

void uartReceiveStringSize(uint8_t *pstring, uint16_t size) {
    if (pstring == NULL)
    {
        Error_Handler();
    }


    HAL_UART_Receive(&huartApi, pstring, size, UART_TX_TIMEOUT);
}
