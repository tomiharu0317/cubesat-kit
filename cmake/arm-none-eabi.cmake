# CMake toolchain file for ARM Cortex-M4F (STM32F407VG)
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake ..

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain prefix - set ARM_TOOLCHAIN_PATH to override
if(DEFINED ENV{ARM_TOOLCHAIN_PATH})
    set(ARM_TOOLCHAIN_PREFIX "$ENV{ARM_TOOLCHAIN_PATH}/bin/arm-none-eabi-")
else()
    set(ARM_TOOLCHAIN_PREFIX "arm-none-eabi-")
endif()

# Toolchain binaries
set(CMAKE_C_COMPILER   "${ARM_TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${ARM_TOOLCHAIN_PREFIX}g++")
set(CMAKE_ASM_COMPILER "${ARM_TOOLCHAIN_PREFIX}gcc")
set(CMAKE_OBJCOPY      "${ARM_TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_OBJDUMP      "${ARM_TOOLCHAIN_PREFIX}objdump")
set(CMAKE_SIZE         "${ARM_TOOLCHAIN_PREFIX}size")

# CPU-specific flags for Cortex-M4F
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Common compile flags
set(COMMON_FLAGS "${CPU_FLAGS} -fdata-sections -ffunction-sections -Wall -Wextra")

set(CMAKE_C_FLAGS_INIT   "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT "${CPU_FLAGS} -x assembler-with-cpp")

set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS}")

# Debug / Release defaults
set(CMAKE_C_FLAGS_DEBUG   "-Og -g3 -DDEBUG" CACHE STRING "")
set(CMAKE_C_FLAGS_RELEASE "-Os -DNDEBUG"    CACHE STRING "")

# Prevent CMake from testing the compiler against the host
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Search paths: never search host programs, only search target libs/includes
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
