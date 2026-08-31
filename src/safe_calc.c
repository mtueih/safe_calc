/*==============================================================================
 * src/safe_calc.c - 项目主库实现文件
 *============================================================================*/


/*------------------------------------------------------------------------------
 * 头文件包含
 *----------------------------------------------------------------------------*/
#include "safe_calc.h"

#include <limits.h>
#include <stdint.h>


/*------------------------------------------------------------------------------
 * 宏定义
 *----------------------------------------------------------------------------*/

/**
 * C23 标准引入了 nullptr 关键字，因此条件定义一个宏，
 * 在 C23 及以上标准时将宏定义为 nullptr，否则定义为 NULL。
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define SAFE_CALC_NULLPTR nullptr
#else
#  define SAFE_CALC_NULLPTR NULL
#endif


/*------------------------------------------------------------------------------
 * API 函数定义
 *----------------------------------------------------------------------------*/

/* size_t 加法。 */

/* 安全地执行【size_t 加法】，防止上溢。 */
bool safe_size_t_add(
	const size_t a,
	const size_t b,
	size_t *const result
) {
	if (a > SIZE_MAX - b) {
		/* 溢出。 */
		return false;
	}

	if (result != SAFE_CALC_NULLPTR) {
		*result = a + b;
	}

	return true;
}

/* size_t 乘法。 */

/* 安全地执行【size_t 乘法】，防止上溢。 */
bool safe_size_t_mul(
	const size_t a,
	const size_t b,
	size_t *const result
) {
	if (a == 0 || b == 0) {
		if (result != SAFE_CALC_NULLPTR) {
			*result = 0;
		}

		return true;
	}

	if (b > SIZE_MAX / a) {
		/* 溢出。 */
		return false;
	}

	if (result != SAFE_CALC_NULLPTR) {
		*result = a * b;
	}

	return true;
}

/* size_t 向上对齐到某个数的整数倍。 */

/* 安全地执行【size_t 向上对齐到某个数的整数倍】，防止上溢。 */
bool safe_size_t_align_up(
	const size_t x,
	const size_t align,
	size_t *const result
) {
	/* 防止除 0，且将 align 为 0 视为“不对齐到任何值”，则结果为 x。 */
	if (align == 0) {
		if (result != SAFE_CALC_NULLPTR) {
			*result = x;
		}

		return true;
	}

	/* 如果 align 是 2 的幂，则使用更高效的计算方法。 */

	/* 缓存 align - 1（前面已排除 align 为 0 的情况，因此不用做溢出检查）。 */
	const size_t align_mask = align - 1;

	if ((align & align_mask) == 0) {
		/* 如果 x 已是 align 的倍数，则结果为 x，无须再计算。 */
		if ((x & align_mask) == 0) {
			if (result != SAFE_CALC_NULLPTR) {
				*result = x;
			}

			return true;
		}

		/* 安全检查：【size_t 加法】（x + align - 1），即（x + align_mask）是否会溢出。 */
		if (!safe_size_t_add(x, align_mask, SAFE_CALC_NULLPTR)) {
			/* 溢出，无法对齐。 */
			return false;
		}

		if (result != SAFE_CALC_NULLPTR) {
			*result = (x + align_mask) & ~align_mask;
		}

		return true;
	}

	/* 常规计算方法。 */
	/* 如果 x 已是 align 的倍数，则结果为 x，无须再计算。 */
	const size_t remainder = x % align;

	if (remainder == 0) {
		if (result != SAFE_CALC_NULLPTR) {
			*result = x;
		}

		return true;
	}

	/* 计算需要增加的偏移量。 */
	const size_t offset = align - remainder;

	/* 安全检查：【size_t 加法】（x + offset）是否会溢出。 */
	if (!safe_size_t_add(x, offset, SAFE_CALC_NULLPTR)) {
		/* 加法溢出。 */
		return false;
	}

	if (result != SAFE_CALC_NULLPTR) {
		*result = x + offset;
	}

	return true;
}
