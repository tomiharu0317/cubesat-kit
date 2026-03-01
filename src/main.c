#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mission.h"
#include "hal_uart.h"

static void SystemClock_Config(void);

/* FreeRTOS required hooks */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    for (;;) { }
}

void vApplicationMallocFailedHook(void)
{
    for (;;) { }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Enable GPIOD clock and configure pin 12 (green LED) as output */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin   = GPIO_PIN_12;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &gpio);

    /* Initialize UART for boot message */
    hal_uart_init(115200);

    const uint8_t boot_msg[] = "CUBESAT-KIT BOOT OK\r\n";
    hal_uart_transmit(boot_msg, sizeof(boot_msg) - 1);

    /* Create mission task and start scheduler */
    xTaskCreate(mission_task, "MISSION", 512, NULL, 3, NULL);
    vTaskStartScheduler();

    /* Should never reach here */
    for (;;) {
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef rcc_osc = {0};
    RCC_ClkInitTypeDef rcc_clk = {0};

    /* Enable PWR clock and set voltage scaling for 168 MHz */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* HSE oscillator + PLL: 8 MHz HSE -> 168 MHz SYSCLK */
    rcc_osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    rcc_osc.HSEState       = RCC_HSE_ON;
    rcc_osc.PLL.PLLState   = RCC_PLL_ON;
    rcc_osc.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    rcc_osc.PLL.PLLM       = 8;
    rcc_osc.PLL.PLLN       = 336;
    rcc_osc.PLL.PLLP       = RCC_PLLP_DIV2;
    rcc_osc.PLL.PLLQ       = 7;

    if (HAL_RCC_OscConfig(&rcc_osc) != HAL_OK) {
        for (;;) { }
    }

    /* HCLK=168MHz, APB1=42MHz, APB2=84MHz */
    rcc_clk.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                             RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    rcc_clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    rcc_clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    rcc_clk.APB1CLKDivider = RCC_HCLK_DIV4;
    rcc_clk.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&rcc_clk, FLASH_LATENCY_5) != HAL_OK) {
        for (;;) { }
    }
}
