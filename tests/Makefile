# =============================================================================
# tests/Makefile — 使用宿主机 GCC 编译并运行所有单元测试
# =============================================================================
#
# 使用：
#   make test     → 编译 + 运行全部测试
#   make clean    → 删除编译产物
#
# 本地 Windows 运行方式：
#   1. 安装 WSL2（推荐）→ wsl 后进入此目录 → make test
#   2. 或安装 MSYS2，在 MSYS2 MinGW64 终端中运行
#
# CI 中运行方式（ubuntu-latest runner，自带 gcc）：
#   cd tests && make test
#
# =============================================================================

CC       = gcc
CFLAGS   = -std=c11 -Wall -Wextra
CFLAGS  += -I.               # 找到 tests/unity.h
CFLAGS  += -I../project/inc  # 找到 button_logic.h 等业务头文件

OUT_DIR  = out

# --- 测试可执行文件列表（每增加一个测试文件就在这里加一行）---
TESTS    = $(OUT_DIR)/test_button_logic

# =============================================================================
.PHONY: test clean

# 编译所有测试，再依次运行
test: $(TESTS)
	@echo ""
	@echo "--- 开始运行测试 ---"
	@FAIL=0; \
	for t in $(TESTS); do \
	    echo ""; \
	    echo ">> $$t"; \
	    ./$$t || FAIL=1; \
	done; \
	echo ""; \
	if [ $$FAIL -eq 0 ]; then \
	    echo "=== 全部测试通过 ==="; \
	else \
	    echo "!!! 有测试失败 !!!"; exit 1; \
	fi

# --- 为每个测试文件指定其依赖的源文件 ---

# test_button_logic.c 依赖 project/src/button_logic.c
$(OUT_DIR)/test_button_logic: test_button_logic.c ../project/src/button_logic.c unity.h | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ test_button_logic.c ../project/src/button_logic.c

# --- 创建输出目录 ---
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
	@echo "测试产物已清理"
