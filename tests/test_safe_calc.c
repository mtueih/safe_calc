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


/*=============================================================================
 * safe_calc - 主库单元测试
 *===========================================================================*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "safe_calc.h"


/*-----------------------------------------------------------------------------
 * 宏定义
 *---------------------------------------------------------------------------*/

/* C23 标准引入了 nullptr 关键字，因此当在 C23 以下标准时将宏定义为 nullptr，否则定义为 NULL。 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define SAFE_CALC_NULLPTR nullptr
#else
#  define SAFE_CALC_NULLPTR NULL
#endif


/*-----------------------------------------------------------------------------
 * 简易测试框架
 *---------------------------------------------------------------------------*/
static int g_test_count = 0;
static int g_fail_count = 0;

#define ASSERT_TRUE(expr) do { \
	g_test_count++; \
	if (!(expr)) { \
		printf("  FAIL [%s:%d]: %s\n", __FILE__, __LINE__, #expr); \
		g_fail_count++; \
	} \
} while (0)

#define RUN_TEST(func) do { \
	printf("▶ %s\n", #func); \
	func(); \
} while (0)


/*-----------------------------------------------------------------------------
 * 测试函数定义
 *---------------------------------------------------------------------------*/

/**
 * @brief safe_size_t_add 测试。
 *
 * 测试：
 *	1、正常行为（不溢出时，是否能获得正确的计算结果及返回值）。
 *	2、溢出行为（溢出时，是否能获得正确的返回值，且不会写入数据）。
 */
static void test_safe_size_t_add(void) {
	size_t result;
	bool ret;

	/* 1、正常行为。 */
	result = 0;
	ret = safe_size_t_add(32, 64, &result);
	ASSERT_TRUE(ret && result == 96);

	/* 2、溢出行为。 */
	result = 66;
	ret = safe_size_t_add(SIZE_MAX, 1, &result);
	ASSERT_TRUE(!ret && result == 66);
}

/**
 * @brief safe_size_t_sub 测试。
 *
 * 测试：
 *	1、正常行为（不溢出时，是否能获得正确的计算结果及返回值）。
 *	2、溢出行为（溢出时，是否能获得正确的返回值，且不会写入数据）。
 */
static void test_safe_size_t_sub(void) {
	size_t result;
	bool ret;

	/* 1、正常行为。 */
	result = 0;
	ret = safe_size_t_sub(64, 16, &result);
	ASSERT_TRUE(ret && result == 48);

	/* 2、溢出行为。 */
	result = 666;
	ret = safe_size_t_sub(16, 64, &result);
	ASSERT_TRUE(!ret && result == 666);
}

/**
 * @brief safe_size_t_mul 测试。
 *
 * 测试：
 *	1、正常行为（不溢出时，是否能获得正确的计算结果及返回值）。
 *		1.1、操作数含 0 的情况。
 *	2、溢出行为（溢出时，是否能获得正确的返回值，且不会写入数据）。
 */
static void test_safe_size_t_mul(void) {
	size_t result;
	bool ret;

	/* 1、正常行为。 */
	result = 0;
	ret = safe_size_t_mul(16, 4, &result);
	ASSERT_TRUE(ret && result == 64);

	/* 1.1、特殊情况，操作数含 0。 */
	result = 64;
	ret = safe_size_t_mul(0, 183, &result);
	ASSERT_TRUE(ret && result == 0);

	result = 64;
	ret = safe_size_t_mul(171, 0, &result);
	ASSERT_TRUE(ret && result == 0);

	/* 2、溢出行为。 */
	result = 666;
	ret = safe_size_t_mul(SIZE_MAX, 2, &result);
	ASSERT_TRUE(!ret && result == 666);
}

/**
 * @brief safe_size_t_align_up 测试。
 *
 * 测试：
 *	1、正常行为（不溢出时，是否能获得正确的计算结果及返回值）。
 *		1.1、align 为 0、1。
 *		1.2、align 为 2 的幂/非 2 的幂。
 *		1.3、x 未对齐/已对齐。
 *	2、溢出行为（溢出时，是否能获得正确的返回值，且不会写入数据）。
 */
static void test_safe_size_t_align_up(void) {
	size_t result;
	bool ret;

	/* 1、正常行为。 */
	/* 1.1.1、align 为 0，结果为 x。 */
	result = 0;
	ret = safe_size_t_align_up(81, 0, &result);
	ASSERT_TRUE(ret && result == 81);

	/* 1.1.2、align 为 1，结果为 x。 */
	result = 0;
	ret = safe_size_t_align_up(93, 1, &result);
	ASSERT_TRUE(ret && result == 93);

	/* 【1.2.1 & 1.3.1】、align 为 2 的幂。未对齐。 */
	result = 0;
	ret = safe_size_t_align_up(131, 64, &result);
	ASSERT_TRUE(ret && result == 192);

	/* 【1.2.1 & 1.3.2】、align 为 2 的幂。已对齐。 */
	result = 0;
	ret = safe_size_t_align_up(64, 8, &result);
	ASSERT_TRUE(ret && result == 64);

	/* 【1.2.2 & 1.3.1】、align 为非 2 的幂。未对齐。 */
	result = 0;
	ret = safe_size_t_align_up(128, 25, &result);
	ASSERT_TRUE(ret && result == 150);

	/* 【1.2.2 & 1.3.2】、align 为非 2 的幂。已对齐。 */
	result = 0;
	ret = safe_size_t_align_up(105, 15, &result);
	ASSERT_TRUE(ret && result == 105);

	/* 2、溢出行为。 */
	result = 666;
	ret = safe_size_t_align_up(SIZE_MAX, 2, &result);
	ASSERT_TRUE(!ret && result == 666);
}


/*-----------------------------------------------------------------------------
 * 主函数
 *---------------------------------------------------------------------------*/
int main(void) {
	printf("===== safe_calc - 主库单元测试 =====\n\n");

	RUN_TEST(test_safe_size_t_add);
	RUN_TEST(test_safe_size_t_sub);
	RUN_TEST(test_safe_size_t_mul);
	RUN_TEST(test_safe_size_t_align_up);

	printf("\n===== 结果：%d 个断言，%d 个失败 =====\n", g_test_count, g_fail_count);

	return g_fail_count > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
