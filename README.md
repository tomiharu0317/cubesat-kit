# cubesat-kit

CubeSat flight software CI/CD template for STM32F4 + FreeRTOS.

Fork this repo and get automated build, unit tests, static analysis, and hardware-in-the-loop simulation — out of the box.

[![CubeSat CI](https://github.com/tomiharu0317/cubesat-kit/actions/workflows/ci.yml/badge.svg)](https://github.com/tomiharu0317/cubesat-kit/actions/workflows/ci.yml)

## Why

41% of CubeSats fail in their first year. Most failures are software-related: boot failures, communication loss, unexpected state transitions. This template gives university CubeSat teams production-grade CI/CD from day one.

## What's included

| Layer | Tool | Purpose |
|-------|------|---------|
| Build | CMake + arm-none-eabi-gcc | Cross-compilation for Cortex-M4F |
| Unit Test | Ceedling + CMock + Unity | HAL-mocked subsystem tests |
| Static Analysis | cppcheck | Bug detection without running code |
| Simulation | Renode + Robot Framework | Full firmware execution on simulated STM32F4 |
| CI | GitHub Actions | 4-job parallel pipeline |

## Architecture

```
src/
├── hal/          # HAL abstraction (GPIO, I2C, UART, SPI)
├── subsystems/   # Flight subsystems (EPS, COMM, OBC)
├── app/          # Mission logic
└── main.c        # Boot + FreeRTOS scheduler
```

Subsystems depend only on the HAL abstraction layer, never on STM32 HAL directly. This makes every subsystem unit-testable with auto-generated mocks.

## Quick start

### Prerequisites

- ARM toolchain: `brew install arm-none-eabi-gcc` (macOS) or `apt install gcc-arm-none-eabi libnewlib-arm-none-eabi` (Ubuntu)
- CMake 3.20+
- Ruby + Ceedling: `gem install ceedling`

### Clone

```bash
git clone --recursive https://github.com/tomiharu0317/cubesat-kit.git
cd cubesat-kit
```

### Build

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build
```

Output: `build/cubesat-kit.elf` (ARM binary) + size report.

### Test

```bash
ceedling test:all
```

Runs all unit tests with auto-mocked HAL dependencies.

### Static analysis

```bash
cppcheck --enable=all --error-exitcode=1 \
  --suppress=missingIncludeSystem --suppress=missingInclude \
  --suppress=unusedFunction --suppress=staticFunction \
  --suppress=knownConditionTrueFalse --suppress=checkersReport \
  -I src/hal -I src/subsystems -I src/app -I config \
  -DSTM32F407xx src/
```

## CI pipeline

Every push and PR triggers 4 parallel jobs:

```
[Push/PR] → Build     → ARM cross-compile + size report
          → Test      → Ceedling + gcov coverage
          → Analyze   → cppcheck
          → Simulate  → Renode SITL (boot verification)
```

All jobs must pass for merge.

## Customization

### Adding a new subsystem

1. Create `src/subsystems/your_subsystem.h` and `.c`
2. Use `hal_*.h` functions for hardware access
3. Add `test/test_your_subsystem.c` — Ceedling auto-generates mocks from HAL headers
4. Run `ceedling test:all` to verify

### Changing the target MCU

1. Update `CMakeLists.txt`: change `MCU_DEFINE` and HAL sources
2. Update `STM32F407VGTx_FLASH.ld`: adjust memory regions
3. Update `cmake/arm-none-eabi.cmake`: adjust CPU flags if changing core (e.g., Cortex-M0)
4. Update `config/FreeRTOSConfig.h`: adjust clock and interrupt priorities
5. Update `sim/cubesat.repl`: point to the correct Renode platform

## Project structure

```
cubesat-kit/
├── .github/workflows/ci.yml    # CI pipeline
├── cmake/arm-none-eabi.cmake   # ARM toolchain file
├── config/                     # HAL + FreeRTOS configuration
├── src/                        # Application source
├── drivers/                    # Vendor (git submodules)
│   ├── STM32F4xx_HAL_Driver/
│   ├── CMSIS/
│   └── FreeRTOS/
├── test/                       # Unit tests (Ceedling)
├── sim/                        # Renode simulation
├── CMakeLists.txt              # Build system
├── STM32F407VGTx_FLASH.ld      # Linker script
└── project.yml                 # Ceedling config
```

## License

MIT
