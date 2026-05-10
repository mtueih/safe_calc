#ifndef SAFE_CALC_H
#define SAFE_CALC_H

#include <attrs.h>
#include <stddef.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L
#include <stdbool.h>
#endif

/**
 * @brief 安全地执行【size_t 加法】，防止溢出。
 * @param a 第一个操作数。
 * @param b 第二个操作数。
 * @param result 指向「存储执行结果的 size_t 变量」的指针（不能为 NULL），如果溢出将不会执行写入操作。
 * @return 如果没有出现溢出，则返回 true，否则返回 false。
 */
bool safe_size_add(
	size_t a,
	size_t b,
	size_t *result
) ATTRS_NONNULL(3);

/**
 * @brief 测试【size_t 加法】能否成功执行，不会出现溢出。
 * @param a 第一个操作数。
 * @param b 第二个操作数。
 * @return 如果不会出现溢出则返回 true，否则返回 false。
 */
bool safe_size_add_test(
	size_t a,
	size_t b
) ATTRS_PURE;

/**
 * @brief 安全地执行【size_t 减法】，防止溢出。
 * @param a 第一个操作数。
 * @param b 第二个操作数。
 * @param result 指向「存储执行结果的 size_t 变量」的指针（不能为 NULL），如果溢出将不会执行写入操作。
 * @return 如果没有出现溢出，则返回 true，否则返回 false。
 */
bool safe_size_sub(
	size_t a,
	size_t b,
	size_t *result
) ATTRS_NONNULL(3);

/**
 * @brief 测试【size_t 减法】能否成功执行，不会出现溢出。
 * @param a 第一个操作数。
 * @param b 第二个操作数。
 * @return 如果不会出现溢出则返回 true，否则返回 false。
 */
bool safe_size_sub_test(
	size_t a,
	size_t b
) ATTRS_PURE;

/**
 * @brief 安全地执行【size_t 乘法】，防止溢出。
 * @param a 第一个操作数。
 * @param b 第二个操作数。
 * @param result 指向「存储执行结果的 size_t 变量」的指针（不能为 NULL），如果溢出将不会执行写入操作。
 * @return 如果没有出现溢出，则返回 true，否则返回 false。
 */
bool safe_size_mul(
	size_t a,
	size_t b,
	size_t *result
) ATTRS_NONNULL(3);

/**
 * @brief 测试【size_t 乘法】能否成功执行，不会出现溢出。
 * @param a 第一个操作数。
 * @param b 第二个操作数。
 * @return 如果不会出现溢出则返回 true，否则返回 false。
 */
bool safe_size_mul_test(
	size_t a,
	size_t b
) ATTRS_PURE;

/**
 * @brief 安全地执行【size_t 向上对齐到某个数的整数倍】，防止溢出。
 * @param x 要对齐的数。
 * @param align 对齐值。
 * @param result 指向「存储执行结果的 size_t 变量」的指针（不能为 NULL），如果溢出将不会执行写入操作。
 * @return 如果没有出现溢出，则返回 true，否则返回 false。
 */
bool safe_size_align_up(
	size_t x,
	size_t align,
	size_t *result
) ATTRS_NONNULL(3);

/**
 * @brief 测试【size_t 向上对齐到某个数的整数倍】能否成功执行，不会出现溢出。
 * @param x 要对齐的数。
 * @param align 对齐值。
 * @return 如果不会出现溢出则返回 true，否则返回 false。
 */
bool safe_size_align_up_test(
	size_t x,
	size_t align
) ATTRS_PURE;

#endif
