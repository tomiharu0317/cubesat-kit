#include "eps.h"
#include "hal_i2c.h"

/* INA219 config: 32V range, 320mA, 12-bit, continuous shunt and bus */
#define INA219_CONFIG_VALUE 0x399F

int eps_init(void)
{
    hal_i2c_status_t status;
    uint8_t config[2];

    status = hal_i2c_init();
    if (status != HAL_I2C_OK) {
        return -1;
    }

    /* Write config register (big-endian) */
    config[0] = (uint8_t)(INA219_CONFIG_VALUE >> 8);
    config[1] = (uint8_t)(INA219_CONFIG_VALUE & 0xFF);

    status = hal_i2c_write_reg(INA219_ADDR, INA219_REG_CONFIG, config, 2);
    if (status != HAL_I2C_OK) {
        return -1;
    }

    return 0;
}

int eps_read_telemetry(eps_telemetry_t *tlm)
{
    hal_i2c_status_t status;
    uint8_t buf[2];
    uint16_t raw_voltage;
    int16_t raw_current;

    tlm->valid = 0;

    /* Read bus voltage register */
    status = hal_i2c_read_reg(INA219_ADDR, INA219_REG_BUS_VOLTAGE, buf, 2);
    if (status != HAL_I2C_OK) {
        return -1;
    }

    raw_voltage = (uint16_t)((buf[0] << 8) | buf[1]);
    /* Shift right 3 to remove status bits, multiply by 4 to get mV */
    tlm->bus_voltage_mv = (raw_voltage >> 3) * 4;

    /* Read current register */
    status = hal_i2c_read_reg(INA219_ADDR, INA219_REG_CURRENT, buf, 2);
    if (status != HAL_I2C_OK) {
        return -1;
    }

    raw_current = (int16_t)((buf[0] << 8) | buf[1]);
    tlm->current_ma = raw_current;

    tlm->valid = 1;
    return 0;
}
