/*
 * Copyright (C) 2026 mtueih
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


/*==============================================================================
 * include/safe_calc.h - 项目主库头文件
 *============================================================================*/
#ifndef SAFE_CALC_H
#define SAFE_CALC_H


/*------------------------------------------------------------------------------
 * 头文件包含
 *----------------------------------------------------------------------------*/
#include <stddef.h>

/**
 * C23 标准已将 bool/true/false 收为内置关键字，
 * 因此按标准仅需在 C23 之前包含 stdbool.h。
 */
#if !defined(__STDC_VERSION__) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
#  include <stdbool.h>
#endif


/*------------------------------------------------------------------------------
 * API 函数原型（声明）
 *----------------------------------------------------------------------------*/

/* size_t 加法。 */

/**
 * @brief
 * 安全地执行【size_t 加法】，防止上溢。
 *
 * @param a
 * 第一个操作数。
 * @param b
 * 第二个操作数。
 * @param result
 * 指向「存储执行结果的 size_t 变量」的指针。
 * 为空指针或会溢出时不写入。
 *
 * @return
 * 如果没溢出则返回 true，否则返回 false。
 */
bool safe_size_t_add(
	size_t a,
	size_t b,
	size_t *result
);

/* size_t 减法。 */

/**
 * @brief
 * 安全地执行【size_t 减法】，防止下溢。
 *
 * @param a
 * 第一个操作数。
 * @param b
 * 第二个操作数。
 * @param result
 * 指向「存储执行结果的 size_t 变量」的指针。
 * 为空指针或会溢出时不写入。
 *
 * @return
 * 如果没溢出则返回 true，否则返回 false。
 */
bool safe_size_t_sub(
	size_t a,
	size_t b,
	size_t *result
);

/* size_t 乘法。 */

/**
 * @brief
 * 安全地执行【size_t 乘法】，防止上溢。
 *
 * @param a
 * 第一个操作数。
 * @param b
 * 第二个操作数。
 * @param result
 * 指向「存储执行结果的 size_t 变量」的指针。
 * 为空指针或会溢出时不写入。
 *
 * @return
 * 如果没溢出则返回 true，否则返回 false。
 */
bool safe_size_t_mul(
	size_t a,
	size_t b,
	size_t *result
);

/* size_t 向上对齐到某个数的整数倍。 */

/**
 * @brief
 * 安全地执行【size_t 向上对齐到某个数的整数倍】，防止上溢。
 *
 * @param x
 * 要对齐的数。
 * @param align
 * 对齐值。
 * 如果为 0，则结果为 x（视 0 为不对齐到任何值）。
 * @param result
 * 指向「存储执行结果的 size_t 变量」的指针。
 * 为空指针或会溢出时不写入。
 *
 * @return
 * 如果没溢出则返回 true，否则返回 false。
 */
bool safe_size_t_align_up(
	size_t x,
	size_t align,
	size_t *result
);


#endif /* SAFE_CALC_H */
