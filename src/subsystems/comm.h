#ifndef COMM_H
#define COMM_H

#include "eps.h"
#include <stdint.h>
#include <stddef.h>

/* Telemetry frame format:
 * [SYNC(2)] [SEQ(2)] [VOLTAGE(2)] [CURRENT(2)] [STATUS(1)] [CRC(1)]
 * Total: 10 bytes */
#define COMM_FRAME_SIZE 10
#define COMM_SYNC_WORD  0xBEEF

typedef struct {
    uint16_t sync;
    uint16_t seq_num;
    uint16_t bus_voltage_mv;
    int16_t  current_ma;
    uint8_t  status;
    uint8_t  crc;
} comm_frame_t;

int comm_init(uint32_t baudrate);
int comm_send_telemetry(const eps_telemetry_t *tlm, uint8_t status);
size_t comm_serialize_frame(const comm_frame_t *frame, uint8_t *buf, size_t buf_size);
uint8_t comm_calc_crc(const uint8_t *data, size_t len);

#endif
