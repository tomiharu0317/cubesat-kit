#include "mission.h"
#include "eps.h"
#include "comm.h"
#include "obc.h"
#include "hal_gpio.h"
#include "FreeRTOS.h"
#include "task.h"

/* STM32F407 GPIOD base address for LED toggling */
#define GPIOD_BASE_ADDR 0x40020C00UL
#define LED_PIN         12

void mission_init(void)
{
    eps_init();
    comm_init(115200);
    obc_init();
}

void mission_task(void *params)
{
    eps_telemetry_t tlm;
    (void)params;

    mission_init();

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        obc_increment_uptime();

        if (eps_read_telemetry(&tlm) == 0) {
            obc_update_housekeeping(&tlm);

            if (obc_check_health() == 0) {
                comm_send_telemetry(&tlm, 0x00);
            }
        }

        hal_gpio_toggle(GPIOD_BASE_ADDR, (1U << LED_PIN));
    }
}
