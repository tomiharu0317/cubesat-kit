#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    HAL_SPI_OK = 0,
    HAL_SPI_ERROR = -1,
    HAL_SPI_TIMEOUT = -2
} hal_spi_status_t;

hal_spi_status_t hal_spi_init(void);
hal_spi_status_t hal_spi_transfer(const uint8_t *tx, uint8_t *rx, size_t len);

#endif
