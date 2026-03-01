*** Settings ***
Suite Setup       Setup
Suite Teardown    Teardown
Test Timeout      30 seconds
Resource          ${RENODEKEYWORDS}

*** Keywords ***
Setup
    Execute Command    mach create "CubeSat"
    Execute Command    machine LoadPlatformDescription @${CURDIR}/../cubesat.repl
    Execute Command    sysbus LoadELF @${CURDIR}/../../build/cubesat-kit.elf
    Create Terminal Tester    sysbus.usart2

Teardown
    Execute Command    mach clear

*** Test Cases ***
Boot Message Should Appear On UART
    [Documentation]    Verify firmware boots and prints expected message
    Start Emulation
    Wait For Line On Uart    CUBESAT-KIT BOOT OK    timeout=10
