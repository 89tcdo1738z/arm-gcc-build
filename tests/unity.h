/**
 * @file   unity.h
 * @brief  极简 C 单元测试框架（约 80 行，无外部依赖）
 *
 * 使用示例：
 *   #include "unity.h"
 *
 *   void test_something(void) {
 *       TEST_ASSERT_EQUAL_INT(42, my_func());
 *   }
 *
 *   int main(void) {
 *       RUN_TEST(test_something);
 *       UNITY_END();
 *       return 0;
 *   }
 *
 * 编译：gcc -std=c11 -I. test_xxx.c ../project/src/xxx.c -o run_test
 * 运行：./run_test （非零退出码表示有失败，CI 会自动捕获）
 */

#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── 内部计数器（static 避免多文件重定义）───────────────── */
static int _unity_pass  = 0;
static int _unity_fail  = 0;
static int _unity_total = 0;

/* ─── 断言：布尔条件为真 ──────────────────────────────────── */
#define TEST_ASSERT_TRUE(cond)                                          \
    do {                                                                \
        _unity_total++;                                                 \
        if (cond) {                                                     \
            _unity_pass++;                                              \
            printf("  [PASS] " #cond "\n");                            \
        } else {                                                        \
            _unity_fail++;                                              \
            printf("  [FAIL] " #cond "  → 期望 true，实际 false"      \
                   "  (%s:%d)\n", __FILE__, __LINE__);                 \
        }                                                               \
    } while (0)

/* ─── 断言：两个整数相等 ─────────────────────────────────── */
#define TEST_ASSERT_EQUAL_INT(expected, actual)                         \
    do {                                                                \
        _unity_total++;                                                 \
        int _e = (int)(expected), _a = (int)(actual);                  \
        if (_e == _a) {                                                 \
            _unity_pass++;                                              \
            printf("  [PASS] %d == %d  (" #actual ")\n", _e, _a);     \
        } else {                                                        \
            _unity_fail++;                                              \
            printf("  [FAIL] 期望 %d，实际 %d  (" #actual ")"         \
                   "  (%s:%d)\n", _e, _a, __FILE__, __LINE__);        \
        }                                                               \
    } while (0)

/* ─── 断言：两个整数不等 ─────────────────────────────────── */
#define TEST_ASSERT_NOT_EQUAL_INT(unexpected, actual)                   \
    do {                                                                \
        _unity_total++;                                                 \
        int _u = (int)(unexpected), _a = (int)(actual);                \
        if (_u != _a) {                                                 \
            _unity_pass++;                                              \
            printf("  [PASS] %d != %d  (" #actual ")\n", _u, _a);     \
        } else {                                                        \
            _unity_fail++;                                              \
            printf("  [FAIL] 不期望 %d，但实际得到 %d  (" #actual ")" \
                   "  (%s:%d)\n", _u, _a, __FILE__, __LINE__);        \
        }                                                               \
    } while (0)

/* ─── 运行一个测试函数，打印函数名 ──────────────────────── */
#define RUN_TEST(func)                                                  \
    do {                                                                \
        printf("\n[TEST] %s\n", #func);                                \
        func();                                                         \
    } while (0)

/* ─── 打印汇总，失败时以非零退出码退出（CI 判断依据）─────── */
#define UNITY_END()                                                     \
    do {                                                                \
        printf("\n=== 测试结果：%d 通过 / %d 失败 / %d 总计 ===\n",   \
               _unity_pass, _unity_fail, _unity_total);                \
        if (_unity_fail > 0) {                                         \
            printf("!!! 有测试失败，CI 将标记为 FAILED !!!\n");        \
            exit(EXIT_FAILURE);                                         \
        }                                                               \
        printf("所有测试通过 ✓\n");                                     \
    } while (0)

#endif /* UNITY_H */
