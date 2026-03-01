#include "unity.h"
#include "obc.h"

void setUp(void) {
    obc_init();
}
void tearDown(void) {}

void test_obc_init(void) {
    const obc_housekeeping_t *hk = obc_get_housekeeping();
    TEST_ASSERT_EQUAL_UINT32(0, hk->uptime_seconds);
    TEST_ASSERT_EQUAL_UINT32(1, hk->reboot_count);
}

void test_obc_increment_uptime(void) {
    obc_increment_uptime();
    obc_increment_uptime();
    const obc_housekeeping_t *hk = obc_get_housekeeping();
    TEST_ASSERT_EQUAL_UINT32(2, hk->uptime_seconds);
}

void test_obc_update_housekeeping(void) {
    eps_telemetry_t eps = {
        .bus_voltage_mv = 3300,
        .current_ma = 100,
        .valid = 1
    };
    obc_update_housekeeping(&eps);
    const obc_housekeeping_t *hk = obc_get_housekeeping();
    TEST_ASSERT_EQUAL_UINT16(3300, hk->last_eps.bus_voltage_mv);
    TEST_ASSERT_EQUAL_UINT8(1, hk->last_eps.valid);
}

void test_obc_check_health_ok(void) {
    eps_telemetry_t eps = { .valid = 1 };
    obc_update_housekeeping(&eps);
    TEST_ASSERT_EQUAL_INT(0, obc_check_health());
}

void test_obc_check_health_invalid_eps(void) {
    /* No eps update yet, last_eps.valid defaults to 0 */
    const obc_housekeeping_t *hk = obc_get_housekeeping();
    /* After init, last_eps.valid = 0 */
    TEST_ASSERT_EQUAL_INT(-1, obc_check_health());
}

void test_obc_check_health_too_many_reboots(void) {
    /* Call init repeatedly to increment reboot_count */
    for (int i = 0; i < OBC_MAX_REBOOT_COUNT; i++) {
        obc_init();
    }
    eps_telemetry_t eps = { .valid = 1 };
    obc_update_housekeeping(&eps);
    TEST_ASSERT_EQUAL_INT(-1, obc_check_health());
}
