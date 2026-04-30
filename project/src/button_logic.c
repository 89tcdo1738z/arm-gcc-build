/**
 * @file   button_logic.c
 * @brief  按键业务逻辑实现（纯函数，无硬件依赖）
 */

#include "button_logic.h"

uint8_t button_get_next_speed(uint8_t current_speed)
{
    return (current_speed == SPEED_SLOW) ? SPEED_FAST : SPEED_SLOW;
}
