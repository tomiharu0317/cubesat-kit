#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

typedef enum {
    HAL_GPIO_PIN_RESET = 0,
    HAL_GPIO_PIN_SET = 1
} hal_gpio_state_t;

void hal_gpio_write(uint32_t port, uint16_t pin, hal_gpio_state_t state);
hal_gpio_state_t hal_gpio_read(uint32_t port, uint16_t pin);
void hal_gpio_toggle(uint32_t port, uint16_t pin);

#endif
