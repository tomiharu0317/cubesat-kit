#include "obc.h"
#include <string.h>

static obc_housekeeping_t hk;

int obc_init(void)
{
    uint32_t prev_reboot_count = hk.reboot_count;
    memset(&hk, 0, sizeof(hk));
    hk.reboot_count = prev_reboot_count + 1;
    return 0;
}

void obc_increment_uptime(void)
{
    hk.uptime_seconds++;
}

void obc_update_housekeeping(const eps_telemetry_t *eps)
{
    hk.last_eps = *eps;
    hk.last_tlm_time = hk.uptime_seconds;
}

const obc_housekeeping_t *obc_get_housekeeping(void)
{
    return &hk;
}

int obc_check_health(void)
{
    if (hk.reboot_count >= OBC_MAX_REBOOT_COUNT) {
        return -1;
    }
    if (hk.last_eps.valid == 0) {
        return -1;
    }
    return 0;
}
