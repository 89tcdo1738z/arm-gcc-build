/**
 * @file   test_button_logic.c
 * @brief  button_logic 模块单元测试
 *
 * 编译方式（宿主机 GCC，无需 ARM 工具链）：
 *   cd tests
 *   make test
 *
 * 手动编译（了解原理）：
 *   gcc -std=c11 -I. -I../project/inc \
 *       test_button_logic.c ../project/src/button_logic.c \
 *       -o out/test_button_logic
 *   ./out/test_button_logic
 */

#include "unity.h"
#include "../project/inc/button_logic.h"

/* ──────────────────────────────────────────────────────────────────
 * 测试用例 1：当前速度 FAST → 期望切换为 SLOW
 * ────────────────────────────────────────────────────────────────── */
void test_fast_switches_to_slow(void)
{
    uint8_t result = button_get_next_speed(SPEED_FAST);
    TEST_ASSERT_EQUAL_INT(SPEED_SLOW, result);
}

/* ──────────────────────────────────────────────────────────────────
 * 测试用例 2：当前速度 SLOW → 期望切换为 FAST
 * ────────────────────────────────────────────────────────────────── */
void test_slow_switches_to_fast(void)
{
    uint8_t result = button_get_next_speed(SPEED_SLOW);
    TEST_ASSERT_EQUAL_INT(SPEED_FAST, result);
}

/* ──────────────────────────────────────────────────────────────────
 * 测试用例 3：连续切换两次应恢复原值（幂等性验证）
 * ────────────────────────────────────────────────────────────────── */
void test_double_toggle_returns_original(void)
{
    uint8_t after_first  = button_get_next_speed(SPEED_FAST);
    uint8_t after_second = button_get_next_speed(after_first);
    TEST_ASSERT_EQUAL_INT(SPEED_FAST, after_second);
}

/* ──────────────────────────────────────────────────────────────────
 * 测试用例 4：返回值始终是合法速度（边界值验证）
 * ────────────────────────────────────────────────────────────────── */
void test_result_is_always_valid(void)
{
    uint8_t r1 = button_get_next_speed(SPEED_FAST);
    uint8_t r2 = button_get_next_speed(SPEED_SLOW);

    TEST_ASSERT_TRUE(r1 == SPEED_FAST || r1 == SPEED_SLOW);
    TEST_ASSERT_TRUE(r2 == SPEED_FAST || r2 == SPEED_SLOW);
}

/* ──────────────────────────────────────────────────────────────────
 * 测试入口
 * ────────────────────────────────────────────────────────────────── */
int main(void)
{
    printf("=== button_logic 单元测试 ===\n");

    RUN_TEST(test_fast_switches_to_slow);
    RUN_TEST(test_slow_switches_to_fast);
    RUN_TEST(test_double_toggle_returns_original);
    RUN_TEST(test_result_is_always_valid);

    UNITY_END();
    return 0;
}
