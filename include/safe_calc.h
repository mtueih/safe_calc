#ifndef SAFE_CALC_H
#define SAFE_CALC_H

#include <stddef.h>
#if !defined(__STDC_VERSION__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
	#include <stdbool.h>
#endif

#include <attrs.h>

/**
 * @brief 安全地执行 size_t 加法，检测溢出
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @param result 输出结果（不能为 NULL）
 * @return true 如果没有溢出，false 如果溢出
 */
bool safe_size_add(
	size_t a,
	size_t b,
	size_t *result
) ATTRS_PURE ATTRS_NONNULL(3);

/**
 * @brief 测试 size_t 加法是否会溢出
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return true 会溢出，不会溢出
 */
bool safe_size_add_test(
	size_t a,
	size_t b
) ATTRS_PURE;

/**
 * @brief 安全地执行 size_t 减法，检测溢出
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @param result 输出结果（不能为 NULL）
 * @return true 如果没有溢出，false 如果溢出
 */
bool safe_size_sub(
	size_t a,
	size_t b,
	size_t *result
) ATTRS_PURE ATTRS_NONNULL(3);

/**
 * @brief 测试 size_t 减法是否会溢出
 * @param a 第一个操作数
 * @param b 第二个操作数
 * @return true 会溢出，不会溢出
 */
bool safe_size_sub_test(
	size_t a,
	size_t b
) ATTRS_PURE;

/**
 * @brief 安全地执行 size_t 向上对齐到某个数的整数倍
 * @param x 要对齐的数
 * @param align 对齐值
 * @param result 输出结果（不能为 NULL）
 * @return true 如果没有溢出，false 如果溢出
 */
bool safe_size_align_up(
	size_t x,
	size_t align,
	size_t *result
) ATTRS_PURE ATTRS_NONNULL(3);

/**
 * @brief 测试 size_t 向上对齐到某个数的整数倍是否会溢出
 * @param x
 * @param align
 * @return true 如果没有溢出，false 如果溢出
 */
bool safe_size_align_up_test(
	size_t x,
	size_t align
) ATTRS_PURE;

#endif
