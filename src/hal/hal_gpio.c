#include "hal_gpio.h"
#include "stm32f4xx_hal.h"

void hal_gpio_write(uint32_t port, uint16_t pin, hal_gpio_state_t state)
{
    GPIO_TypeDef *gpio_port = (GPIO_TypeDef *)port;
    HAL_GPIO_WritePin(gpio_port, pin, (GPIO_PinState)state);
}

hal_gpio_state_t hal_gpio_read(uint32_t port, uint16_t pin)
{
    GPIO_TypeDef *gpio_port = (GPIO_TypeDef *)port;
    GPIO_PinState st = HAL_GPIO_ReadPin(gpio_port, pin);
    return (st == GPIO_PIN_SET) ? HAL_GPIO_PIN_SET : HAL_GPIO_PIN_RESET;
}

void hal_gpio_toggle(uint32_t port, uint16_t pin)
{
    GPIO_TypeDef *gpio_port = (GPIO_TypeDef *)port;
    HAL_GPIO_TogglePin(gpio_port, pin);
}
