#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    HAL_UART_OK = 0,
    HAL_UART_ERROR = -1,
    HAL_UART_TIMEOUT = -2
} hal_uart_status_t;

hal_uart_status_t hal_uart_init(uint32_t baudrate);
hal_uart_status_t hal_uart_transmit(const uint8_t *data, size_t len);
hal_uart_status_t hal_uart_receive(uint8_t *data, size_t len, uint32_t timeout_ms);

#endif
