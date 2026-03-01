#ifndef EPS_H
#define EPS_H

#include <stdint.h>

/* INA219 I2C address (A0=A1=GND) */
#define INA219_ADDR 0x40

/* INA219 registers */
#define INA219_REG_BUS_VOLTAGE 0x02
#define INA219_REG_CURRENT     0x04
#define INA219_REG_CONFIG      0x00

typedef struct {
    uint16_t bus_voltage_mv;
    int16_t  current_ma;
    uint8_t  valid;
} eps_telemetry_t;

int eps_init(void);
int eps_read_telemetry(eps_telemetry_t *tlm);

#endif
