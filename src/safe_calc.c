#include "safe_calc.h"

#include <limits.h>

// 安全地执行 size_t 加法，检测溢出
bool safe_size_add(const size_t a, const size_t b, size_t *const result) {
	if (a > SIZE_MAX - b) {
		return false; // 溢出
	}
	*result = a + b;
	return true;
}

// 测试 size_t 加法是否会溢出
bool safe_size_add_test(const size_t a, const size_t b) {
	if (a > SIZE_MAX - b) {
		return false; // 溢出
	}
	return true;
}

// 安全地执行 size_t 减法，检测溢出
bool safe_size_sub(const size_t a, const size_t b, size_t *const result) {
	if (a < b) {
		return false; // 溢出
	}
	*result = a - b;
	return true;
}

// 测试 size_t 减法是否会溢出
bool safe_size_sub_test(const size_t a, const size_t b) {
	if (a < b) {
		return false; // 溢出
	}
	return true;
}

// 安全地执行 size_t 对齐
bool safe_size_align_up(const size_t x, const size_t align, size_t *const result) {
	// 防止除 0，且将 align 为 0 视为不对齐，则结果为 x
	if (align == 0) {
		*result = x;
		return true;
	}

	// 如果 align 是 2 的幂，则使用更高效的计算方法
	// 缓存 align - 1（前面已排除 align 为 0 的情况，因此不用做溢出检查）
	const size_t align_mask = align - 1;
	if ((align & align_mask) == 0) {
		// 如果 x 已是 align 的倍数，则结果为 x，无须再计算
		if ((x & align_mask) == 0) {
			*result = x;
			return true;
		}

		// 安全检查：size_t 加法（x + align - 1)，即 (x + align_mask)是否会溢出
		if (!safe_size_add_test(x, align_mask)) {
			return false;  // 溢出，无法对齐
		}

		*result = (x + align_mask) & ~align_mask;
		return true;
	}

	// 常规计算方法
	// 如果 x 已是 align 的倍数，则结果为 x，无须再计算
	const size_t remainder = x % align;
	if (remainder == 0) {
		*result = x;
		return true;
	}

	// 计算需要增加的偏移量
	const size_t offset = align - remainder;

	// 安全检查：size_t 加法（x + offset）是否会溢出
	if (!safe_size_add_test(x, offset)) {
		return false;	// 加法溢出
	}

	*result = x + offset;
	return true;
}

// 测试 size_t 对齐是否会溢出
bool safe_size_align_up_test(const size_t x, const size_t align) {
	// 防止除 0，且将 align 为 0 视为不对齐，则结果为 x
	if (align == 0) {
		return true;
	}

	// 如果 align 是 2 的幂，则使用更高效的计算方法
	// 缓存 align - 1（前面已排除 align 为 0 的情况，因此不用做溢出检查）
	const size_t align_mask = align - 1;
	if ((align & align_mask) == 0) {
		// 如果 x 已是 align 的倍数，则结果为 x，无须再计算
		if ((x & align_mask) == 0) {
			return true;
		}

		// 安全检查：size_t 加法（x + align - 1)，即 (x + align_mask)是否会溢出
		if (!safe_size_add_test(x, align_mask)) {
			return false;  // 溢出，无法对齐
		}

		return true;
	}

	// 常规计算方法
	// 如果 x 已是 align 的倍数，则结果为 x，无须再计算
	const size_t remainder = x % align;
	if (remainder == 0) {
		return true;
	}

	// 计算需要增加的偏移量
	const size_t offset = align - remainder;

	// 安全检查：size_t 加法（x + offset）是否会溢出
	if (!safe_size_add_test(x, offset)) {
		return false;	// 加法溢出
	}

	return true;
}
