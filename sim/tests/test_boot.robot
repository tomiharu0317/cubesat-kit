*** Settings ***
Suite Setup       Setup
Suite Teardown    Teardown
Test Setup        Reset Emulation
Test Teardown     Test Teardown
Resource          ${RENODEKEYWORDS}

*** Test Cases ***
Boot Message Should Appear On UART
    [Documentation]    Verify firmware boots and prints expected message on USART2
    Execute Command    mach create "CubeSat"
    Execute Command    machine LoadPlatformDescription @platforms/boards/stm32f4_discovery-kit.repl
    Execute Command    sysbus LoadELF @${CURDIR}/../../build/cubesat-kit.elf
    Create Terminal Tester    sysbus.usart2
    Start Emulation
    Wait For Line On Uart    CUBESAT-KIT BOOT OK    timeout=10
