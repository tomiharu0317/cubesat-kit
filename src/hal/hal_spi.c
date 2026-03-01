#include "hal_spi.h"
#include "stm32f4xx_hal.h"

#define SPI_TIMEOUT_MS 1000

static SPI_HandleTypeDef hspi1;

hal_spi_status_t hal_spi_init(void)
{
    hspi1.Instance               = SPI1;
    hspi1.Init.Mode              = SPI_MODE_MASTER;
    hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
    hspi1.Init.NSS               = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial     = 10;

    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        return HAL_SPI_ERROR;
    }
    return HAL_SPI_OK;
}

hal_spi_status_t hal_spi_transfer(const uint8_t *tx, uint8_t *rx, size_t len)
{
    HAL_StatusTypeDef status;
    status = HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)tx, rx, (uint16_t)len,
                                     SPI_TIMEOUT_MS);
    if (status == HAL_TIMEOUT) {
        return HAL_SPI_TIMEOUT;
    }
    if (status != HAL_OK) {
        return HAL_SPI_ERROR;
    }
    return HAL_SPI_OK;
}
