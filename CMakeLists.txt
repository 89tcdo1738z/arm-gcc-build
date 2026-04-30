cmake_minimum_required(VERSION 3.22)

# 工具链必须在 project() 之前设置
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-none-eabi.cmake)

project(at32f403a_template C CXX ASM)

# ── MCU 标志 ──────────────────────────────────────────────────────────────────
set(MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=softfp
)

# ── 编译选项 ──────────────────────────────────────────────────────────────────
add_compile_options(
    ${MCU_FLAGS}
    -Wall
    -Og
    -g
    -fdata-sections
    -ffunction-sections
)

add_compile_options($<$<COMPILE_LANGUAGE:C>:-std=c11>)
add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++11>)

# ── 预处理宏 ──────────────────────────────────────────────────────────────────
add_compile_definitions(
    _DEBUG
    USE_STDPERIPH_DRIVER
    AT_START_F403A_V1
    AT32F403AVGT7
)

# ── 头文件路径 ────────────────────────────────────────────────────────────────
include_directories(
    libraries/cmsis/cm4/core_support
    libraries/cmsis/cm4/device_support
    libraries/drivers/inc
    project/inc
    project/board
)

# ── 源文件 ────────────────────────────────────────────────────────────────────
set(SOURCES
    # 启动与系统初始化
    libraries/cmsis/cm4/device_support/startup/gcc/startup_at32f403a_407.s
    libraries/cmsis/cm4/device_support/system_at32f403a_407.c

    # 外设驱动库
    libraries/drivers/src/at32f403a_407_acc.c
    libraries/drivers/src/at32f403a_407_adc.c
    libraries/drivers/src/at32f403a_407_bpr.c
    libraries/drivers/src/at32f403a_407_can.c
    libraries/drivers/src/at32f403a_407_crc.c
    libraries/drivers/src/at32f403a_407_crm.c
    libraries/drivers/src/at32f403a_407_dac.c
    libraries/drivers/src/at32f403a_407_debug.c
    libraries/drivers/src/at32f403a_407_dma.c
    libraries/drivers/src/at32f403a_407_exint.c
    libraries/drivers/src/at32f403a_407_flash.c
    libraries/drivers/src/at32f403a_407_gpio.c
    libraries/drivers/src/at32f403a_407_i2c.c
    libraries/drivers/src/at32f403a_407_misc.c
    libraries/drivers/src/at32f403a_407_pwc.c
    libraries/drivers/src/at32f403a_407_rtc.c
    libraries/drivers/src/at32f403a_407_sdio.c
    libraries/drivers/src/at32f403a_407_spi.c
    libraries/drivers/src/at32f403a_407_tmr.c
    libraries/drivers/src/at32f403a_407_usart.c
    libraries/drivers/src/at32f403a_407_usb.c
    libraries/drivers/src/at32f403a_407_wdt.c
    libraries/drivers/src/at32f403a_407_wwdt.c
    libraries/drivers/src/at32f403a_407_xmc.c

    # 板级与应用
    project/board/at32f403a_407_board.c
    project/src/at32f403a_407_clock.c
    project/src/at32f403a_407_int.c
    project/src/main.c
)

# ── 链接器脚本 ────────────────────────────────────────────────────────────────
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/project/misc/AT32F403AxG_FLASH.ld)

# ── 链接选项 ──────────────────────────────────────────────────────────────────
add_link_options(
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    --specs=nosys.specs
    --specs=nano.specs
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -lm
)

# ── 生成目标 ──────────────────────────────────────────────────────────────────
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# 链接数学库
target_link_libraries(${PROJECT_NAME}.elf m)

# ── 生成 bin / hex 并打印大小 ─────────────────────────────────────────────────
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
    COMMENT "Generating ${PROJECT_NAME}.hex and ${PROJECT_NAME}.bin"
)
