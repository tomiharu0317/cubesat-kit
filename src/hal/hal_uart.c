#include "hal_uart.h"
#include "stm32f4xx_hal.h"

#define UART_TX_TIMEOUT_MS 1000

static UART_HandleTypeDef huart2;

hal_uart_status_t hal_uart_init(uint32_t baudrate)
{
    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = baudrate;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        return HAL_UART_ERROR;
    }
    return HAL_UART_OK;
}

hal_uart_status_t hal_uart_transmit(const uint8_t *data, size_t len)
{
    HAL_StatusTypeDef status;
    status = HAL_UART_Transmit(&huart2, (uint8_t *)data, (uint16_t)len,
                               UART_TX_TIMEOUT_MS);
    if (status == HAL_TIMEOUT) {
        return HAL_UART_TIMEOUT;
    }
    if (status != HAL_OK) {
        return HAL_UART_ERROR;
    }
    return HAL_UART_OK;
}

hal_uart_status_t hal_uart_receive(uint8_t *data, size_t len, uint32_t timeout_ms)
{
    HAL_StatusTypeDef status;
    status = HAL_UART_Receive(&huart2, data, (uint16_t)len, timeout_ms);
    if (status == HAL_TIMEOUT) {
        return HAL_UART_TIMEOUT;
    }
    if (status != HAL_OK) {
        return HAL_UART_ERROR;
    }
    return HAL_UART_OK;
}
