#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    HAL_I2C_OK = 0,
    HAL_I2C_ERROR = -1,
    HAL_I2C_TIMEOUT = -2
} hal_i2c_status_t;

hal_i2c_status_t hal_i2c_init(void);
hal_i2c_status_t hal_i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);
hal_i2c_status_t hal_i2c_write_reg(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, size_t len);

#endif
