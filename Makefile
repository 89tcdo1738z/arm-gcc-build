# =============================================================================
# AT32F403A ARM Cortex-M4 交叉编译 Makefile
# =============================================================================
#
# 用途：
#   供 CI/CD 流水线（GitHub Actions / GitLab CI）或本地 Linux/WSL 环境
#   通过命令行编译固件，无需 EIDE 图形界面。
#
# 常用命令：
#   make all      → 编译，生成 .elf
#   make clean    → 删除 build/ci/ 目录
#   make size     → 查看各段大小
#
# 前提条件（CI 中由 Docker / apt 提供）：
#   apt-get install -y gcc-arm-none-eabi binutils-arm-none-eabi make
#
# =============================================================================

# --- 工具链前缀（可通过环境变量覆盖）---
PREFIX   ?= arm-none-eabi-
CC        = $(PREFIX)gcc
AS        = $(PREFIX)gcc
LD        = $(PREFIX)gcc
OBJCOPY   = $(PREFIX)objcopy
SIZE      = $(PREFIX)size

# --- 构建输出目录（区别于 EIDE 的 build/Debug/，避免冲突）---
TARGET    = at32f403a_template
BUILD_DIR = build/ci

# --- CPU 参数（来自 build/Debug/compile_commands.json 实测值）---
# Cortex-M4, Thumb2, 软件浮点（无 FPU 硬件指令）
CPU_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=soft

# --- C 编译选项 ---
CFLAGS    = $(CPU_FLAGS)
CFLAGS   += -std=c11
CFLAGS   += -Og              # 调试优化（与 EIDE Debug 配置一致）
CFLAGS   += -Wall -Wextra    # 开启全部警告
CFLAGS   += -g               # 生成调试符号
CFLAGS   += -ffunction-sections -fdata-sections   # 允许链接器删除未用代码
CFLAGS   += -funsigned-char
CFLAGS   += --specs=nosys.specs --specs=nano.specs

# --- 汇编选项 ---
ASFLAGS   = $(CPU_FLAGS) -c -x assembler-with-cpp

# --- 预处理宏（从 .eide/eide.yml targets.Debug.cppPreprocessAttrs.defineList 同步）---
DEFS      = -D_DEBUG
DEFS     += -DUSE_STDPERIPH_DRIVER
DEFS     += -DAT_START_F403A_V1
DEFS     += -DAT32F403AVGT7

# --- 头文件搜索路径（从 .eide/eide.yml incList 同步）---
INCS      = -Ilibraries/cmsis/cm4/core_support
INCS     += -Ilibraries/cmsis/cm4/device_support
INCS     += -Ilibraries/drivers/inc
INCS     += -Iproject/inc
INCS     += -Iproject/board

# --- C 源文件 ---
# ① AT32 外设驱动库（全部 .c）
SRCS_C    = $(wildcard libraries/drivers/src/*.c)
# ② CMSIS 设备支持（系统初始化）
SRCS_C   += libraries/cmsis/cm4/device_support/system_at32f403a_407.c
# ③ 板级支持
SRCS_C   += project/board/at32f403a_407_board.c
# ④ 项目源文件
SRCS_C   += project/src/at32f403a_407_clock.c
SRCS_C   += project/src/at32f403a_407_int.c
SRCS_C   += project/src/button_logic.c
SRCS_C   += project/src/main.c

# --- 汇编启动文件（使用 GCC 版本，排除 MDK/IAR 版本）---
SRCS_S    = libraries/cmsis/cm4/device_support/startup/gcc/startup_at32f403a_407.s

# --- 链接脚本 ---
LD_SCRIPT = project/misc/AT32F403AxG_FLASH.ld

# --- 目标文件路径（自动镜像源文件目录结构）---
OBJS_C    = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS_C))
OBJS_S    = $(patsubst %.s, $(BUILD_DIR)/%.o, $(SRCS_S))
OBJS      = $(OBJS_C) $(OBJS_S)

# --- 链接选项 ---
LDFLAGS   = $(CPU_FLAGS)
LDFLAGS  += -T$(LD_SCRIPT)
LDFLAGS  += -Wl,--gc-sections                       # 删除未引用的段
LDFLAGS  += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map     # 生成 map 文件
LDFLAGS  += --specs=nosys.specs --specs=nano.specs
LDFLAGS  += -lm

# =============================================================================
# 构建规则
# =============================================================================
.PHONY: all clean size

all: $(BUILD_DIR)/$(TARGET).elf

# --- 链接：所有 .o → .elf ---
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo ""
	$(SIZE) $@
	@echo "=== 固件编译成功：$@ ==="

# --- 编译 C 文件，-MMD 自动生成 .d 依赖文件 ---
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -MMD -c -o $@ $<

# --- 编译汇编文件 ---
$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $(INCS) -MMD -o $@ $<

# 引入依赖文件，使头文件修改也能触发重新编译
-include $(OBJS:.o=.d)

size: $(BUILD_DIR)/$(TARGET).elf
	$(SIZE) $<

clean:
	rm -rf $(BUILD_DIR)
	@echo "=== 清理完成 ==="
