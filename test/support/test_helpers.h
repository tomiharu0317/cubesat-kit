#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

/* Common test constants */
#define TEST_INA219_ADDR     0x40
#define TEST_BUS_VOLTAGE_RAW 0x1A00  /* ~3.3V: (0x1A00 >> 3) * 4 = 3328 mV */
#define TEST_CURRENT_RAW     150

#endif
