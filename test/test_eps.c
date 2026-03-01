#include "unity.h"
#include "eps.h"
#include "mock_hal_i2c.h"
#include "test_helpers.h"

void setUp(void) {}
void tearDown(void) {}

void test_eps_init_success(void) {
    hal_i2c_init_ExpectAndReturn(HAL_I2C_OK);
    hal_i2c_write_reg_ExpectAnyArgsAndReturn(HAL_I2C_OK);
    TEST_ASSERT_EQUAL_INT(0, eps_init());
}

void test_eps_init_i2c_fail(void) {
    hal_i2c_init_ExpectAndReturn(HAL_I2C_ERROR);
    TEST_ASSERT_EQUAL_INT(-1, eps_init());
}

void test_eps_read_telemetry_success(void) {
    eps_telemetry_t tlm;
    /* Bus voltage register read: returns 2 bytes big-endian */
    uint8_t voltage_data[2] = {0x1A, 0x00};
    hal_i2c_read_reg_ExpectAnyArgsAndReturn(HAL_I2C_OK);
    hal_i2c_read_reg_ReturnMemThruPtr_data(voltage_data, 2);

    /* Current register read */
    uint8_t current_data[2] = {0x00, 0x96}; /* 150 */
    hal_i2c_read_reg_ExpectAnyArgsAndReturn(HAL_I2C_OK);
    hal_i2c_read_reg_ReturnMemThruPtr_data(current_data, 2);

    TEST_ASSERT_EQUAL_INT(0, eps_read_telemetry(&tlm));
    TEST_ASSERT_EQUAL_UINT8(1, tlm.valid);
    /* (0x1A00 >> 3) * 4 = 0x340 * 4 = 3328 mV */
    TEST_ASSERT_EQUAL_UINT16(3328, tlm.bus_voltage_mv);
    TEST_ASSERT_EQUAL_INT16(150, tlm.current_ma);
}

void test_eps_read_telemetry_i2c_fail(void) {
    eps_telemetry_t tlm;
    hal_i2c_read_reg_ExpectAnyArgsAndReturn(HAL_I2C_ERROR);
    TEST_ASSERT_EQUAL_INT(-1, eps_read_telemetry(&tlm));
    TEST_ASSERT_EQUAL_UINT8(0, tlm.valid);
}
