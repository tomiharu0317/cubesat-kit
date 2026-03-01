#ifndef OBC_H
#define OBC_H

#include "eps.h"
#include <stdint.h>

#define OBC_MAX_REBOOT_COUNT 10

typedef struct {
    uint32_t uptime_seconds;
    uint32_t reboot_count;
    uint32_t last_tlm_time;
    eps_telemetry_t last_eps;
} obc_housekeeping_t;

int obc_init(void);
void obc_increment_uptime(void);
void obc_update_housekeeping(const eps_telemetry_t *eps);
const obc_housekeeping_t *obc_get_housekeeping(void);
int obc_check_health(void);

#endif
