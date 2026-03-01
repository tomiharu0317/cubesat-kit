/**
  * FreeRTOS configuration for STM32F407VG (Cortex-M4F @ 168 MHz)
  * cubesat-kit flight software template
  */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* ----------------------- Core settings ----------------------------------- */
#define configUSE_PREEMPTION                1
#define configCPU_CLOCK_HZ                  ((uint32_t)168000000)
#define configTICK_RATE_HZ                  ((TickType_t)1000)
#define configMAX_PRIORITIES                7
#define configMINIMAL_STACK_SIZE            ((uint16_t)128)
#define configTOTAL_HEAP_SIZE               ((size_t)(32 * 1024))
#define configMAX_TASK_NAME_LEN             16
#define configUSE_16_BIT_TICKS              0
#define configIDLE_SHOULD_YIELD             1

/* ----------------------- Synchronisation --------------------------------- */
#define configUSE_MUTEXES                   1
#define configUSE_RECURSIVE_MUTEXES         1
#define configUSE_COUNTING_SEMAPHORES       1
#define configQUEUE_REGISTRY_SIZE           8

/* ----------------------- Software timers --------------------------------- */
#define configUSE_TIMERS                    1
#define configTIMER_TASK_PRIORITY           2
#define configTIMER_QUEUE_LENGTH            10
#define configTIMER_TASK_STACK_DEPTH        (configMINIMAL_STACK_SIZE * 2)

/* ----------------------- Hook functions ---------------------------------- */
#define configUSE_IDLE_HOOK                 0
#define configUSE_TICK_HOOK                 0
#define configUSE_MALLOC_FAILED_HOOK        1
#define configCHECK_FOR_STACK_OVERFLOW      2

/* ----------------------- Run-time stats ---------------------------------- */
#define configGENERATE_RUN_TIME_STATS       0
#define configUSE_TRACE_FACILITY            1
#define configUSE_STATS_FORMATTING_FUNCTIONS 0

/* ----------------------- Co-routines (unused) ---------------------------- */
#define configUSE_CO_ROUTINES               0
#define configMAX_CO_ROUTINE_PRIORITIES     2

/* ----------------------- API inclusion ----------------------------------- */
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources        0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_eTaskGetState               1

/* --------------- Cortex-M4 interrupt priority configuration --------------- */
/*
 * The STM32F4 uses 4 priority bits (0-15). FreeRTOS needs to know the raw
 * (shifted) values for kernel-managed interrupts.
 *
 * configLIBRARY_* values are in the 0-15 "logical" range.
 * configKERNEL_INTERRUPT_PRIORITY and configMAX_SYSCALL_INTERRUPT_PRIORITY
 * are the same values left-shifted into the upper 4 bits of the 8-bit
 * priority register.
 */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY      15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY  5

#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_KERNEL_INTERRUPT_PRIORITY << (8 - 4))          /* 255 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - 4))     /* 80  */

/* Use CMSIS-compatible names so the vector table picks up FreeRTOS handlers */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
