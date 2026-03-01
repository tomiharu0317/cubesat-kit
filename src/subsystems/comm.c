#include "comm.h"
#include "hal_uart.h"

static uint16_t seq_counter = 0;

int comm_init(uint32_t baudrate)
{
    hal_uart_status_t status;
    status = hal_uart_init(baudrate);
    if (status != HAL_UART_OK) {
        return -1;
    }
    seq_counter = 0;
    return 0;
}

uint8_t comm_calc_crc(const uint8_t *data, size_t len)
{
    uint8_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
    }
    return crc;
}

size_t comm_serialize_frame(const comm_frame_t *frame, uint8_t *buf, size_t buf_size)
{
    if (buf_size < COMM_FRAME_SIZE) {
        return 0;
    }

    /* Serialize in big-endian order */
    buf[0] = (uint8_t)(frame->sync >> 8);
    buf[1] = (uint8_t)(frame->sync & 0xFF);
    buf[2] = (uint8_t)(frame->seq_num >> 8);
    buf[3] = (uint8_t)(frame->seq_num & 0xFF);
    buf[4] = (uint8_t)(frame->bus_voltage_mv >> 8);
    buf[5] = (uint8_t)(frame->bus_voltage_mv & 0xFF);
    buf[6] = (uint8_t)((uint16_t)frame->current_ma >> 8);
    buf[7] = (uint8_t)((uint16_t)frame->current_ma & 0xFF);
    buf[8] = frame->status;
    buf[9] = frame->crc;

    return COMM_FRAME_SIZE;
}

int comm_send_telemetry(const eps_telemetry_t *tlm, uint8_t status)
{
    comm_frame_t frame;
    uint8_t buf[COMM_FRAME_SIZE];
    size_t len;

    frame.sync           = COMM_SYNC_WORD;
    frame.seq_num        = seq_counter++;
    frame.bus_voltage_mv = tlm->bus_voltage_mv;
    frame.current_ma     = tlm->current_ma;
    frame.status         = status;
    frame.crc            = 0; /* placeholder for serialization */

    /* Serialize frame to compute CRC over first 9 bytes */
    len = comm_serialize_frame(&frame, buf, sizeof(buf));
    if (len == 0) {
        return -1;
    }

    /* Calculate CRC over all bytes except the CRC field itself */
    buf[COMM_FRAME_SIZE - 1] = comm_calc_crc(buf, COMM_FRAME_SIZE - 1);

    if (hal_uart_transmit(buf, COMM_FRAME_SIZE) != HAL_UART_OK) {
        return -1;
    }

    return 0;
}
