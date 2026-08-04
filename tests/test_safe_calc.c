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
 * tests/test_safe_calc.c - 项目主库单元测试文件
 *============================================================================*/


/*------------------------------------------------------------------------------
 * 头文件包含
 *----------------------------------------------------------------------------*/
#include "safe_calc.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * C23 标准已将 bool/true/false 收为内置关键字，
 * 因此按标准仅需在 C23 之前包含 stdbool.h。
 */
#if !defined(__STDC_VERSION__) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
#  include <stdbool.h>
#endif


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

/* 测试函数数量。 */
#define TEST_COUNT 3


/*------------------------------------------------------------------------------
 * 测试静态函数声明
 *----------------------------------------------------------------------------*/

/* size_t 加法（safe_size_t_add）测试。 */
static bool test_safe_size_t_add(void);

/* size_t 乘法（safe_size_t_mul）测试。 */
static bool test_safe_size_t_mul(void);

/* size_t 向上对齐（safe_size_t_align_up）测试。 */
static bool test_safe_size_t_align_up(void);


/*------------------------------------------------------------------------------
 * 主函数
 *----------------------------------------------------------------------------*/
int main(void) {
	unsigned fail_count = 0;

	printf("======== safe_calc 单元测试 ========\n");

	fail_count += test_safe_size_t_add() ? 0 : 1;
	fail_count += test_safe_size_t_mul() ? 0 : 1;
	fail_count += test_safe_size_t_align_up() ? 0 : 1;

	printf(
		"\n---- safe_calc 单元测试结束 ----\n"
		"- 共 %d 个测试函数  ✔ %u 个成功  ✘ %u 个失败\n",
		TEST_COUNT, TEST_COUNT - fail_count, fail_count
	);

	return (fail_count > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

/*------------------------------------------------------------------------------
 * 测试静态函数定义
 *----------------------------------------------------------------------------*/

/* size_t 加法（safe_size_t_add）测试。 */
static bool test_safe_size_t_add(void) {
	size_t res = 0;

	printf("\n➤【size_t 加法】测试\n");

	/* 正常加法测试。 */
	if (!safe_size_t_add(1, 1, &res)) {
		fprintf(
			stderr, "\n\t正常加法（1 + 1）测试失败：\n"
			"\t返回值（false）不符合预期（true）！\n"
		);
		return false;
	}

	if (res != 2) {
		fprintf(
			stderr, "\n\t正常加法（1 + 1）测试失败：\n"
			"\t计算结果（%zu）不符合预期（2）！\n", res
		);
		return false;
	}

	/* 异常加法（会溢出）测试。 */
	if (safe_size_t_add(SIZE_MAX, 1, &res)) {
		fprintf(
			stderr, "\n\t异常加法（SIZE_MAX + 1）测试失败：\n"
			"\t返回值（true）不符合预期（false）！\n"
		);
		return false;
	}

	if (res != 2) {
		fprintf(
			stderr, "\n\t异常加法（SIZE_MAX + 1）测试失败：\n"
			"\t计算结果（%zu）不符合预期（不更改）！\n", res
		);
		return false;
	}

	return true;
}

/* size_t 乘法（safe_size_t_mul）测试。 */
static bool test_safe_size_t_mul(void) {
	size_t res = 0;

	printf("\n➤【size_t 乘法】测试\n");

	/* 正常乘法测试。 */
	if (!safe_size_t_mul(6, 6, &res)) {
		fprintf(
			stderr, "\n\t正常乘法（6 * 6）测试失败：\n"
			"\t返回值（false）不符合预期（true）！\n"
		);
		return false;
	}

	if (res != 36) {
		fprintf(
			stderr, "\n\t正常乘法（6 * 6）测试失败：\n"
			"\t计算结果（%zu）不符合预期（36）！\n", res
		);
		return false;
	}

	/* 异常乘法（会溢出）测试。 */
	if (safe_size_t_mul(SIZE_MAX >> 1, 3, &res)) {
		fprintf(
			stderr, "\n\t异常乘法（SIZE_MAX / 2 * 3）测试失败：\n"
			"\t返回值（true）不符合预期（false）！\n"
		);
		return false;
	}

	if (res != 36) {
		fprintf(
			stderr, "\n\t异常乘法（SIZE_MAX / 2 * 3）测试失败：\n"
			"\t计算结果（%zu）不符合预期（不更改）！\n", res
		);
		return false;
	}

	return true;
}

/* size_t 向上对齐（safe_size_t_align_up）测试。 */
static bool test_safe_size_t_align_up(void) {
	size_t res = 0;

	printf("\n➤【size_t 向上对齐】测试\n");

	/* 正常向上对齐测试。 */
	if (!safe_size_t_align_up(27, 16, &res)) {
		fprintf(
			stderr, "\n\t正常向上对齐（27, 16）测试失败：\n"
			"\t返回值（false）不符合预期（true）！\n"
		);
		return false;
	}

	if (res != 32) {
		fprintf(
			stderr, "\n\t正常向上对齐（27, 16）测试失败：\n"
			"\t计算结果（%zu）不符合预期（32）！\n", res
		);
		return false;
	}

	/* 异常向上对齐（会溢出）测试。 */
	if (safe_size_t_align_up(SIZE_MAX, 2, &res)) {
		fprintf(
			stderr, "\n\t异常向上对齐（SIZE_MAX, 2）测试失败：\n"
			"\t返回值（true）不符合预期（false）！\n"
		);
		return false;
	}

	if (res != 32) {
		fprintf(
			stderr, "\n\t异常向上对齐（SIZE_MAX, 2）测试失败：\n"
			"\t计算结果（%zu）不符合预期（不更改）！\n", res
		);
		return false;
	}

	return true;
}
