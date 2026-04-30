/**
 * @file   button_logic.h
 * @brief  按键业务逻辑 —— 纯 C 函数，零硬件依赖
 *
 * 设计原则（嵌入式单元测试黄金法则）：
 *   ┌──────────────────────────────────────────────────┐
 *   │  硬件层（GPIO/EXINT）  ←  只在 main.c / _int.c  │
 *   │                                                  │
 *   │  业务逻辑层（本文件）  ←  纯函数，可在 PC 测试  │
 *   └──────────────────────────────────────────────────┘
 *
 * 这样 CI 里只需要宿主机 GCC，不需要 ARM 工具链就能跑单元测试。
 */

#ifndef BUTTON_LOGIC_H
#define BUTTON_LOGIC_H

#include <stdint.h>

/* 与 main.c 中的 #define 保持一致 */
#define SPEED_FAST  1u
#define SPEED_SLOW  4u

/**
 * @brief  纯逻辑函数：根据当前速度返回切换后的速度
 *
 *   按键每次触发：FAST → SLOW → FAST → ...
 *
 * @param  current_speed  当前速度值（SPEED_FAST 或 SPEED_SLOW）
 * @return 切换后的速度值
 *
 * @note   此函数不碰任何寄存器，可在宿主机 GCC 上直接编译、测试。
 */
uint8_t button_get_next_speed(uint8_t current_speed);

#endif /* BUTTON_LOGIC_H */
