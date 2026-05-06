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

#endif
