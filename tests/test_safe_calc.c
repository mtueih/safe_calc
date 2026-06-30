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


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "safe_calc.h"

/***************************************************************
 ********************    简易测试框架。    *********************
 **************************************************************/

static int g_test_count = 0;
static int g_fail_count = 0;

#define ASSERT_TRUE(expr) do { \
	g_test_count++; \
	if (!(expr)) { \
		printf("  FAIL [%s:%d]: %s\n", __FILE__, __LINE__, #expr); \
		g_fail_count++; \
	} \
} while (0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ(a, b) do { \
	g_test_count++; \
	if ((a) != (b)) { \
		printf("  FAIL [%s:%d]: %s == %s（%zu != %zu）\n", \
			__FILE__, __LINE__, #a, #b, (size_t)(a), (size_t)(b)); \
		g_fail_count++; \
	} \
} while (0)

#define RUN_TEST(func) do { \
	printf("▶ %s\n", #func); \
	func(); \
} while (0)


/***************************************************************
 ******************    safe_size_t_add 测试。    ***************
 **************************************************************/

static void test_safe_size_t_add(void) {
	size_t result;
	size_t sentinel;
	bool ret;

	/* 正常值。 */
	ret = safe_size_t_add(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_add(1, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 2);

	ret = safe_size_t_add(3, 7, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 10);

	ret = safe_size_t_add(100, 200, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 300);

	/* 边界值。 */
	ret = safe_size_t_add(SIZE_MAX, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	ret = safe_size_t_add(0, SIZE_MAX, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	ret = safe_size_t_add(1, SIZE_MAX - 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	ret = safe_size_t_add(SIZE_MAX - 1, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	ret = safe_size_t_add(SIZE_MAX / 2, SIZE_MAX / 2, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX - 1);

	/* 溢出。 */
	sentinel = 666;
	ret = safe_size_t_add(SIZE_MAX, 1, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666); /* 不写入。 */

	sentinel = 666;
	ret = safe_size_t_add(1, SIZE_MAX, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_add(SIZE_MAX, SIZE_MAX, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_add(SIZE_MAX / 2 + 1, SIZE_MAX / 2 + 1, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	/* 空指针 + 不溢出。 */
	ret = safe_size_t_add(3, 7, NULL);
	ASSERT_TRUE(ret);

	ret = safe_size_t_add(SIZE_MAX, 0, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + 溢出。 */
	ret = safe_size_t_add(SIZE_MAX, 1, NULL);
	ASSERT_FALSE(ret);

	ret = safe_size_t_add(SIZE_MAX, SIZE_MAX, NULL);
	ASSERT_FALSE(ret);
}


/***************************************************************
 ******************    safe_size_t_sub 测试。    ***************
 **************************************************************/

static void test_safe_size_t_sub(void) {
	size_t result;
	size_t sentinel;
	bool ret;

	/* 正常值。 */
	ret = safe_size_t_sub(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_sub(1, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 1);

	ret = safe_size_t_sub(1, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_sub(10, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 7);

	ret = safe_size_t_sub(200, 100, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 100);

	/* 边界值。 */
	ret = safe_size_t_sub(SIZE_MAX, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	ret = safe_size_t_sub(SIZE_MAX, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX - 1);

	ret = safe_size_t_sub(SIZE_MAX, SIZE_MAX, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_sub(SIZE_MAX - 1, SIZE_MAX - 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	/* 溢出（下溢）。 */
	sentinel = 666;
	ret = safe_size_t_sub(0, 1, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_sub(1, 2, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_sub(100, 200, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_sub(SIZE_MAX - 1, SIZE_MAX, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	/* 空指针 + 不溢出。 */
	ret = safe_size_t_sub(10, 3, NULL);
	ASSERT_TRUE(ret);

	ret = safe_size_t_sub(SIZE_MAX, SIZE_MAX, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + 溢出。 */
	ret = safe_size_t_sub(0, 1, NULL);
	ASSERT_FALSE(ret);

	ret = safe_size_t_sub(100, 200, NULL);
	ASSERT_FALSE(ret);
}


/***************************************************************
 ******************    safe_size_t_mul 测试。    ***************
 **************************************************************/

static void test_safe_size_t_mul(void) {
	size_t result;
	size_t sentinel;
	bool ret;

	/* 零分支：a == 0 或 b == 0。 */
	ret = safe_size_t_mul(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_mul(0, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_mul(1, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_mul(0, SIZE_MAX, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_mul(SIZE_MAX, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_mul(0, 100, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	/* 正常值。 */
	ret = safe_size_t_mul(1, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 1);

	ret = safe_size_t_mul(2, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 6);

	ret = safe_size_t_mul(7, 100, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 700);

	ret = safe_size_t_mul(100, 100, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 10000);

	/* 边界值。 */
	ret = safe_size_t_mul(1, SIZE_MAX, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	ret = safe_size_t_mul(SIZE_MAX, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	ret = safe_size_t_mul(2, SIZE_MAX / 2, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX - 1);

	ret = safe_size_t_mul(SIZE_MAX / 2, 2, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX - 1);

	/* 溢出。 */
	sentinel = 666;
	ret = safe_size_t_mul(2, SIZE_MAX, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_mul(SIZE_MAX, 2, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_mul(SIZE_MAX, SIZE_MAX, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	sentinel = 666;
	ret = safe_size_t_mul(SIZE_MAX / 2 + 1, 3, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	/* 空指针 + 零分支。 */
	ret = safe_size_t_mul(0, SIZE_MAX, NULL);
	ASSERT_TRUE(ret);

	ret = safe_size_t_mul(SIZE_MAX, 0, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + 不溢出。 */
	ret = safe_size_t_mul(2, 3, NULL);
	ASSERT_TRUE(ret);

	ret = safe_size_t_mul(1, SIZE_MAX, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + 溢出。 */
	ret = safe_size_t_mul(2, SIZE_MAX, NULL);
	ASSERT_FALSE(ret);

	ret = safe_size_t_mul(SIZE_MAX, SIZE_MAX, NULL);
	ASSERT_FALSE(ret);
}


/***************************************************************
 ***************    safe_size_t_align_up 测试。    *************
 **************************************************************/

static void test_safe_size_t_align_up(void) {
	size_t result;
	size_t sentinel;
	bool ret;

	/* align == 0 分支：结果为 x。 */
	ret = safe_size_t_align_up(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_align_up(1, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 1);

	ret = safe_size_t_align_up(100, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 100);

	ret = safe_size_t_align_up(SIZE_MAX, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	/* align 是 2 的幂，x 已是 align 的倍数。 */
	ret = safe_size_t_align_up(0, 4, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_align_up(8, 4, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 8);

	ret = safe_size_t_align_up(16, 8, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 16);

	ret = safe_size_t_align_up(256, 256, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 256);

	ret = safe_size_t_align_up(SIZE_MAX - 255, 256, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX - 255);

	/* align 是 2 的幂，x 不是 align 的倍数。 */
	ret = safe_size_t_align_up(1, 4, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 4);

	ret = safe_size_t_align_up(5, 4, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 8);

	ret = safe_size_t_align_up(7, 8, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 8);

	ret = safe_size_t_align_up(1, 256, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 256);

	ret = safe_size_t_align_up(255, 256, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 256);

	ret = safe_size_t_align_up(257, 256, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 512);

	/* align 不是 2 的幂，x 已是 align 的倍数。 */
	ret = safe_size_t_align_up(0, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 0);

	ret = safe_size_t_align_up(9, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 9);

	ret = safe_size_t_align_up(15, 5, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 15);

	ret = safe_size_t_align_up(70, 7, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 70);

	ret = safe_size_t_align_up(100, 10, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 100);

	/* align 不是 2 的幂，x 不是 align 的倍数。 */
	ret = safe_size_t_align_up(1, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 3);

	ret = safe_size_t_align_up(10, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 12);

	ret = safe_size_t_align_up(1, 5, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 5);

	ret = safe_size_t_align_up(16, 5, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 20);

	ret = safe_size_t_align_up(1, 7, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 7);

	ret = safe_size_t_align_up(8, 7, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 14);

	ret = safe_size_t_align_up(1, 10, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 10);

	ret = safe_size_t_align_up(99, 10, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, 100);

	/* 溢出：align 是 2 的幂。 */
	/* x = SIZE_MAX - 1, align = 4：x 不是 4 的倍数，且 x + offset 超过 SIZE_MAX → 溢出。 */
	sentinel = 666;
	ret = safe_size_t_align_up(SIZE_MAX - 1, 4, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	/* x = SIZE_MAX - 3, align = 8：x 不是 8 的倍数，且 x + (align - 1) 超过 SIZE_MAX → 溢出。 */
	sentinel = 666;
	ret = safe_size_t_align_up(SIZE_MAX - 3, 8, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	/* 溢出：align 不是 2 的幂。 */
	/* x = SIZE_MAX - 2, align = 6：x 不是 6 的倍数，且 x + offset 超过 SIZE_MAX → 溢出。 */
	sentinel = 666;
	ret = safe_size_t_align_up(SIZE_MAX - 2, 6, &sentinel);
	ASSERT_FALSE(ret);
	ASSERT_EQ(sentinel, 666);

	/* 最大有效对齐值：align 不是 2 的幂且 x 已是其倍数，x = SIZE_MAX 时结果仍为 SIZE_MAX，不溢出。 */
	ret = safe_size_t_align_up(SIZE_MAX, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ(result, SIZE_MAX);

	/* 空指针 + align == 0。 */
	ret = safe_size_t_align_up(42, 0, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + align 是 2 的幂，x 是倍数。 */
	ret = safe_size_t_align_up(8, 4, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + align 是 2 的幂，x 不是倍数。 */
	ret = safe_size_t_align_up(5, 4, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + align 不是 2 的幂，x 是倍数。 */
	ret = safe_size_t_align_up(9, 3, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + align 不是 2 的幂，x 不是倍数。 */
	ret = safe_size_t_align_up(10, 3, NULL);
	ASSERT_TRUE(ret);

	/* 空指针 + 溢出。 */
	ret = safe_size_t_align_up(SIZE_MAX - 1, 4, NULL);
	ASSERT_FALSE(ret);

	/* 空指针 + align 不是 2 的幂，SIZE_MAX 是 3 的倍数（所有偶数位宽）。 */
	ret = safe_size_t_align_up(SIZE_MAX, 3, NULL);
	ASSERT_TRUE(ret);
}


/***************************************************************
 ***********************    主函数。    ************************
 **************************************************************/

int main(void) {
	printf("===== safe_calc 测试 =====\n\n");

	RUN_TEST(test_safe_size_t_add);
	RUN_TEST(test_safe_size_t_sub);
	RUN_TEST(test_safe_size_t_mul);
	RUN_TEST(test_safe_size_t_align_up);

	printf("\n===== 结果：%d 个断言，%d 个失败 =====\n", g_test_count, g_fail_count);

	return g_fail_count > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
