cmake_minimum_required(VERSION 3.20)

project(cubesat-kit LANGUAGES C ASM)

# ---------------------------------------------------------------------------
# Target MCU
# ---------------------------------------------------------------------------
set(MCU_DEFINE STM32F407xx)

# ---------------------------------------------------------------------------
# Include paths
# ---------------------------------------------------------------------------
set(APP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/hal
    ${CMAKE_SOURCE_DIR}/src/subsystems
    ${CMAKE_SOURCE_DIR}/src/app
    ${CMAKE_SOURCE_DIR}/config
)

set(HAL_DRIVER_DIR ${CMAKE_SOURCE_DIR}/drivers/STM32F4xx_HAL_Driver)
set(CMSIS_DEVICE_DIR ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Device/ST/STM32F4xx)
set(CMSIS_CORE_DIR ${CMAKE_SOURCE_DIR}/drivers/CMSIS/CMSIS_5/CMSIS/Core)
set(FREERTOS_DIR ${CMAKE_SOURCE_DIR}/drivers/FreeRTOS)

set(DRIVER_INCLUDE_DIRS
    ${HAL_DRIVER_DIR}/Inc
    ${CMSIS_DEVICE_DIR}/Include
    ${CMSIS_CORE_DIR}/Include
    ${FREERTOS_DIR}/include
    ${FREERTOS_DIR}/portable/GCC/ARM_CM4F
)

# ---------------------------------------------------------------------------
# Application sources
# ---------------------------------------------------------------------------
file(GLOB APP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/hal/*.c
    ${CMAKE_SOURCE_DIR}/src/subsystems/*.c
    ${CMAKE_SOURCE_DIR}/src/app/*.c
)

# ---------------------------------------------------------------------------
# STM32 HAL driver sources (only what we need)
# ---------------------------------------------------------------------------
set(HAL_SOURCES
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_cortex.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_gpio.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_i2c.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_uart.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_spi.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_rcc.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_rcc_ex.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_pwr.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_pwr_ex.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_dma.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_flash.c
    ${HAL_DRIVER_DIR}/Src/stm32f4xx_hal_flash_ex.c
)

# ---------------------------------------------------------------------------
# CMSIS sources
# ---------------------------------------------------------------------------
set(CMSIS_SOURCES
    ${CMSIS_DEVICE_DIR}/Source/Templates/gcc/startup_stm32f407xx.s
    ${CMSIS_DEVICE_DIR}/Source/Templates/system_stm32f4xx.c
)

# ---------------------------------------------------------------------------
# FreeRTOS sources
# ---------------------------------------------------------------------------
set(FREERTOS_SOURCES
    ${FREERTOS_DIR}/tasks.c
    ${FREERTOS_DIR}/list.c
    ${FREERTOS_DIR}/queue.c
    ${FREERTOS_DIR}/timers.c
    ${FREERTOS_DIR}/event_groups.c
    ${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS_DIR}/portable/MemMang/heap_4.c
)

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------
add_executable(cubesat-kit.elf
    ${APP_SOURCES}
    ${HAL_SOURCES}
    ${CMSIS_SOURCES}
    ${FREERTOS_SOURCES}
)

target_include_directories(cubesat-kit.elf PRIVATE
    ${APP_INCLUDE_DIRS}
    ${DRIVER_INCLUDE_DIRS}
)

target_compile_definitions(cubesat-kit.elf PRIVATE
    ${MCU_DEFINE}
    USE_HAL_DRIVER
)

# ---------------------------------------------------------------------------
# Linker configuration
# ---------------------------------------------------------------------------
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VGTx_FLASH.ld)

target_link_options(cubesat-kit.elf PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,--gc-sections
    -specs=nosys.specs
    -specs=nano.specs
    -Wl,-Map=${CMAKE_BINARY_DIR}/cubesat-kit.map,--cref
)

set_target_properties(cubesat-kit.elf PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT}
)

# ---------------------------------------------------------------------------
# Post-build: generate .bin and print size
# ---------------------------------------------------------------------------
add_custom_command(TARGET cubesat-kit.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:cubesat-kit.elf>
            ${CMAKE_BINARY_DIR}/cubesat-kit.bin
    COMMENT "Generating cubesat-kit.bin"
)

add_custom_target(size ALL
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:cubesat-kit.elf>
    DEPENDS cubesat-kit.elf
    COMMENT "Firmware size report"
)
