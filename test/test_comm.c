#include "unity.h"
#include "comm.h"
#include "mock_hal_uart.h"

void setUp(void) {}
void tearDown(void) {}

void test_comm_calc_crc(void) {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t crc = comm_calc_crc(data, 4);
    /* XOR: 0x01 ^ 0x02 ^ 0x03 ^ 0x04 = 0x04 */
    TEST_ASSERT_EQUAL_HEX8(0x04, crc);
}

void test_comm_serialize_frame(void) {
    comm_frame_t frame = {
        .sync = COMM_SYNC_WORD,
        .seq_num = 1,
        .bus_voltage_mv = 3300,
        .current_ma = 150,
        .status = 0x01,
        .crc = 0x00
    };
    uint8_t buf[COMM_FRAME_SIZE];
    size_t written = comm_serialize_frame(&frame, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_size_t(COMM_FRAME_SIZE, written);
    /* Check sync word big-endian: 0xBEEF */
    TEST_ASSERT_EQUAL_HEX8(0xBE, buf[0]);
    TEST_ASSERT_EQUAL_HEX8(0xEF, buf[1]);
}

void test_comm_serialize_frame_buffer_too_small(void) {
    comm_frame_t frame = { .sync = COMM_SYNC_WORD };
    uint8_t buf[5]; /* too small */
    size_t written = comm_serialize_frame(&frame, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_size_t(0, written);
}

void test_comm_init_success(void) {
    hal_uart_init_ExpectAndReturn(115200, HAL_UART_OK);
    TEST_ASSERT_EQUAL_INT(0, comm_init(115200));
}

void test_comm_send_telemetry_success(void) {
    eps_telemetry_t tlm = {
        .bus_voltage_mv = 3300,
        .current_ma = 150,
        .valid = 1
    };
    hal_uart_transmit_ExpectAnyArgsAndReturn(HAL_UART_OK);
    TEST_ASSERT_EQUAL_INT(0, comm_send_telemetry(&tlm, 0x01));
}
