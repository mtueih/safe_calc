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

/*
 * C23 标准引入了 nullptr 关键字，因此当在 C23 及以上标准时将宏定义为 nullptr，
 * 否则定义为 NULL。
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define SAFE_CALC_NULLPTR nullptr
#else
#  define SAFE_CALC_NULLPTR NULL
#endif

/*
 * 哨兵值：用于验证溢出时 result 未被修改。
 *
 * 每个测试函数使用不同的哨兵值，以降低「函数有 bug 错误地写入了某个值
 * 而该值恰好等于哨兵值」导致假阴性的概率。所有哨兵值均控制在 16 位以内，
 * 不对 size_t 的位宽做任何假设。
 */
#define SENTINEL_ADD   ((size_t)0xBADD)
#define SENTINEL_SUB   ((size_t)0xDEAD)
#define SENTINEL_MUL   ((size_t)0xFACE)
#define SENTINEL_ALIGN ((size_t)0xFEED)


/*-----------------------------------------------------------------------------
 * 测试运行器：将全局状态封装为结构体，便于未来多文件/多线程扩展
 *---------------------------------------------------------------------------*/

typedef struct {
	int assert_total;
	int assert_fail;
	int func_pass;
	int func_fail;
} test_runner_t;

static test_runner_t g_runner = {0, 0, 0, 0};


/*-----------------------------------------------------------------------------
 * 断言宏
 *
 * 注意：ASSERT_EQ_SIZE_T 内部使用 (size_t) 强制转换，调用方应确保传入的
 * 期望值表达式本身为无符号语义。表驱动测试结构体中的 expected 字段已声明为
 * size_t 类型，杜绝了负数被静默转换为 SIZE_MAX 的风险。
 *---------------------------------------------------------------------------*/

#define ASSERT_TRUE(expr) do { \
	g_runner.assert_total++; \
	if (!(expr)) { \
		printf("  FAIL [%s:%d]: %s\n", __FILE__, __LINE__, #expr); \
		g_runner.assert_fail++; \
	} \
} while (0)

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_EQ_SIZE_T(expected, actual) do { \
	g_runner.assert_total++; \
	{ \
		size_t _e = (size_t)(expected); \
		size_t _a = (size_t)(actual); \
		if (_e != _a) { \
			printf("  FAIL [%s:%d]: expected %zu (0x%zX), got %zu (0x%zX)\n", \
			       __FILE__, __LINE__, _e, _e, _a, _a); \
			g_runner.assert_fail++; \
		} \
	} \
} while (0)

#define ASSERT_BOOL(expected, actual) do { \
	g_runner.assert_total++; \
	{ \
		bool _e = (bool)(expected); \
		bool _a = (bool)(actual); \
		if (_e != _a) { \
			printf("  FAIL [%s:%d]: expected %s, got %s\n", \
			       __FILE__, __LINE__, \
			       _e ? "true" : "false", \
			       _a ? "true" : "false"); \
			g_runner.assert_fail++; \
		} \
	} \
} while (0)

/*
 * 表驱动测试专用断言：附带 case 索引，便于快速定位数组中的失败元素。
 * 用法示例：ASSERT_EQ_SIZE_T_IDX(cases[i].expected, result, i);
 */
#define ASSERT_EQ_SIZE_T_IDX(expected, actual, idx) do { \
	g_runner.assert_total++; \
	{ \
		size_t _e = (size_t)(expected); \
		size_t _a = (size_t)(actual); \
		if (_e != _a) { \
			printf("  FAIL [%s:%d] case[%zu]: expected %zu (0x%zX), got %zu (0x%zX)\n", \
			       __FILE__, __LINE__, (size_t)(idx), _e, _e, _a, _a); \
			g_runner.assert_fail++; \
		} \
	} \
} while (0)

#define ASSERT_BOOL_IDX(expected, actual, idx) do { \
	g_runner.assert_total++; \
	{ \
		bool _e = (bool)(expected); \
		bool _a = (bool)(actual); \
		if (_e != _a) { \
			printf("  FAIL [%s:%d] case[%zu]: expected %s, got %s\n", \
			       __FILE__, __LINE__, (size_t)(idx), \
			       _e ? "true" : "false", \
			       _a ? "true" : "false"); \
			g_runner.assert_fail++; \
		} \
	} \
} while (0)

#define RUN_TEST(func) do { \
	int _prev_total = g_runner.assert_total; \
	int _prev_fail  = g_runner.assert_fail; \
	printf("▶ %s\n", #func); \
	func(); \
	int _delta_total = g_runner.assert_total - _prev_total; \
	int _delta_fail  = g_runner.assert_fail  - _prev_fail; \
	if (_delta_fail == 0) { \
		printf("  ✓ PASS (%d assertions)\n", _delta_total); \
		g_runner.func_pass++; \
	} else { \
		printf("  ✗ FAIL (%d/%d assertions failed)\n", \
		       _delta_fail, _delta_total); \
		g_runner.func_fail++; \
	} \
} while (0)


/*-----------------------------------------------------------------------------
 * 辅助结构体：用于表驱动测试
 *
 * expected 字段声明为 size_t，杜绝负数被静默转换为 SIZE_MAX 的风险。
 *---------------------------------------------------------------------------*/

typedef struct {
	size_t a;
	size_t b;
	size_t expected;
	bool should_pass;
} add_sub_case_t;

typedef struct {
	size_t a;
	size_t b;
	size_t expected;
	bool should_pass;
} mul_case_t;

typedef struct {
	size_t x;
	size_t align;
	size_t expected;
	bool should_pass;
} align_case_t;


/*-----------------------------------------------------------------------------
 * safe_size_t_add 测试
 *
 * 覆盖：正常加法、含 0、边界（刚好不溢出）、溢出（多种组合）、
 *       交换律、空指针安全性。
 *---------------------------------------------------------------------------*/
static void test_safe_size_t_add(void) {
	size_t result;
	bool ret;

	/* ---- 正常行为 ---- */

	result = 0;
	ret = safe_size_t_add(32, 64, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(96, result);

	result = 0;
	ret = safe_size_t_add(64, 32, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(96, result);

	/* ---- 含 0 操作数 ---- */

	result = 0;
	ret = safe_size_t_add(SIZE_MAX, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	result = 0;
	ret = safe_size_t_add(0, SIZE_MAX, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	result = 0;
	ret = safe_size_t_add(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	/* ---- 边界：刚好不溢出 ---- */

	result = 0;
	ret = safe_size_t_add(SIZE_MAX - 1, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	result = 0;
	ret = safe_size_t_add(1, SIZE_MAX - 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	result = 0;
	ret = safe_size_t_add(SIZE_MAX / 2, SIZE_MAX / 2, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX - (SIZE_MAX % 2), result);

	result = 0;
	ret = safe_size_t_add(SIZE_MAX / 2, SIZE_MAX - SIZE_MAX / 2, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	/* ---- 溢出行为 ---- */

	result = SENTINEL_ADD;
	ret = safe_size_t_add(SIZE_MAX, 1, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ADD, result);

	result = SENTINEL_ADD;
	ret = safe_size_t_add(1, SIZE_MAX, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ADD, result);

	result = SENTINEL_ADD;
	ret = safe_size_t_add(SIZE_MAX, 2, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ADD, result);

	result = SENTINEL_ADD;
	ret = safe_size_t_add(SIZE_MAX, SIZE_MAX, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ADD, result);

	result = SENTINEL_ADD;
	ret = safe_size_t_add(SIZE_MAX - 1, 2, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ADD, result);

	result = SENTINEL_ADD;
	ret = safe_size_t_add(SIZE_MAX / 2 + 1, SIZE_MAX / 2 + 1, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ADD, result);

	/* ---- 表驱动：正常用例 ---- */

	{
		static const add_sub_case_t cases[] = {
			{100, 200, 300, true},
			{0, 0, 0, true},
			{1, 0, 1, true},
			{0, 1, 1, true},
			{SIZE_MAX, 0, SIZE_MAX, true},
			{0, SIZE_MAX, SIZE_MAX, true},
		};
		for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
			result = 0;
			ret = safe_size_t_add(cases[i].a, cases[i].b, &result);
			ASSERT_BOOL_IDX(cases[i].should_pass, ret, i);
			if (cases[i].should_pass) { ASSERT_EQ_SIZE_T_IDX(cases[i].expected, result, i); }
		}
	}

	/* ---- 表驱动：溢出用例 ---- */

	{
		static const add_sub_case_t over_cases[] = {
			{SIZE_MAX, 1, SENTINEL_ADD, false},
			{1, SIZE_MAX, SENTINEL_ADD, false},
			{SIZE_MAX, SIZE_MAX, SENTINEL_ADD, false},
			{SIZE_MAX - 1, 2, SENTINEL_ADD, false},
			{SIZE_MAX / 2 + 1, SIZE_MAX / 2 + 1, SENTINEL_ADD, false},
		};
		for (size_t i = 0; i < sizeof(over_cases) / sizeof(over_cases[0]); i++) {
			result = SENTINEL_ADD;
			ret = safe_size_t_add(over_cases[i].a, over_cases[i].b, &result);
			ASSERT_BOOL_IDX(over_cases[i].should_pass, ret, i);
			ASSERT_EQ_SIZE_T_IDX(SENTINEL_ADD, result, i);
		}
	}

	/* ---- 空指针安全性 ---- */

	ret = safe_size_t_add(42, 58, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);

	ret = safe_size_t_add(SIZE_MAX, 1, SAFE_CALC_NULLPTR);
	ASSERT_FALSE(ret);

	ret = safe_size_t_add(0, 0, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);
}


/*-----------------------------------------------------------------------------
 * safe_size_t_sub 测试
 *
 * 覆盖：正常减法、结果为 0、减 0、0-0、下溢（多种组合）、
 *       SIZE_MAX 相关边界、空指针安全性。
 *---------------------------------------------------------------------------*/
static void test_safe_size_t_sub(void) {
	size_t result;
	bool ret;

	/* ---- 正常行为 ---- */

	result = 0;
	ret = safe_size_t_sub(64, 16, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(48, result);

	result = 0;
	ret = safe_size_t_sub(100, 100, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	result = 0;
	ret = safe_size_t_sub(SIZE_MAX, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	result = 0;
	ret = safe_size_t_sub(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	/* ---- SIZE_MAX 相关正常边界 ---- */

	result = 0;
	ret = safe_size_t_sub(SIZE_MAX, SIZE_MAX, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	result = 0;
	ret = safe_size_t_sub(SIZE_MAX, SIZE_MAX - 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(1, result);

	result = 0;
	ret = safe_size_t_sub(SIZE_MAX, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX - 1, result);

	/* ---- 下溢行为 ---- */

	result = SENTINEL_SUB;
	ret = safe_size_t_sub(16, 64, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_SUB, result);

	result = SENTINEL_SUB;
	ret = safe_size_t_sub(0, 1, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_SUB, result);

	result = SENTINEL_SUB;
	ret = safe_size_t_sub(1, SIZE_MAX, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_SUB, result);

	result = SENTINEL_SUB;
	ret = safe_size_t_sub(SIZE_MAX - 1, SIZE_MAX, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_SUB, result);

	result = SENTINEL_SUB;
	ret = safe_size_t_sub(0, SIZE_MAX, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_SUB, result);

	/* ---- 表驱动：正常用例 ---- */

	{
		static const add_sub_case_t cases[] = {
			{500, 200, 300, true},
			{1, 1, 0, true},
			{SIZE_MAX, SIZE_MAX, 0, true},
			{SIZE_MAX, 0, SIZE_MAX, true},
			{0, 0, 0, true},
		};
		for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
			result = 0;
			ret = safe_size_t_sub(cases[i].a, cases[i].b, &result);
			ASSERT_BOOL_IDX(cases[i].should_pass, ret, i);
			if (cases[i].should_pass) { ASSERT_EQ_SIZE_T_IDX(cases[i].expected, result, i); }
		}
	}

	/* ---- 表驱动：下溢用例 ---- */

	{
		static const add_sub_case_t over_cases[] = {
			{0, 1, SENTINEL_SUB, false},
			{10, 20, SENTINEL_SUB, false},
			{1, SIZE_MAX, SENTINEL_SUB, false},
			{0, SIZE_MAX, SENTINEL_SUB, false},
		};
		for (size_t i = 0; i < sizeof(over_cases) / sizeof(over_cases[0]); i++) {
			result = SENTINEL_SUB;
			ret = safe_size_t_sub(over_cases[i].a, over_cases[i].b, &result);
			ASSERT_BOOL_IDX(over_cases[i].should_pass, ret, i);
			ASSERT_EQ_SIZE_T_IDX(SENTINEL_SUB, result, i);
		}
	}

	/* ---- 空指针安全性 ---- */

	ret = safe_size_t_sub(100, 30, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);

	ret = safe_size_t_sub(10, 20, SAFE_CALC_NULLPTR);
	ASSERT_FALSE(ret);

	ret = safe_size_t_sub(SIZE_MAX, SIZE_MAX, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);
}


/*-----------------------------------------------------------------------------
 * safe_size_t_mul 测试
 *
 * 覆盖：正常乘法、含 0/含 1、边界（刚好不溢出）、溢出（多种组合）、
 *       交换律、空指针安全性。
 *---------------------------------------------------------------------------*/
static void test_safe_size_t_mul(void) {
	size_t result;
	bool ret;

	/* ---- 正常行为 ---- */

	result = 0;
	ret = safe_size_t_mul(16, 4, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(64, result);

	result = 0;
	ret = safe_size_t_mul(4, 16, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(64, result);

	/* ---- 含 0 操作数 ---- */

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(0, 183, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(171, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	/* ---- 乘 1 边界 ---- */

	result = 0;
	ret = safe_size_t_mul(SIZE_MAX, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	result = 0;
	ret = safe_size_t_mul(1, SIZE_MAX, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	/* ---- 边界：刚好不溢出 ---- */

	result = 0;
	ret = safe_size_t_mul(SIZE_MAX / 2, 2, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T((SIZE_MAX / 2) * 2, result);

	result = 0;
	ret = safe_size_t_mul(2, SIZE_MAX / 2, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(2 * (SIZE_MAX / 2), result);

	result = 0;
	ret = safe_size_t_mul(SIZE_MAX / 3, 3, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T((SIZE_MAX / 3) * 3, result);

	/* ---- 溢出行为 ---- */

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(SIZE_MAX, 2, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_MUL, result);

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(2, SIZE_MAX, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_MUL, result);

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(SIZE_MAX, SIZE_MAX, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_MUL, result);

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(SIZE_MAX / 2 + 1, 2, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_MUL, result);

	result = SENTINEL_MUL;
	ret = safe_size_t_mul(2, SIZE_MAX / 2 + 1, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_MUL, result);

	/* ---- 表驱动：正常用例 ---- */

	{
		static const mul_case_t cases[] = {
			{100, 3, 300, true},
			{0, 0, 0, true},
			{0, 999, 0, true},
			{999, 0, 0, true},
			{SIZE_MAX, 1, SIZE_MAX, true},
			{1, SIZE_MAX, SIZE_MAX, true},
			{SIZE_MAX / 2, 2, (SIZE_MAX / 2) * 2, true},
			{2, SIZE_MAX / 2, 2 * (SIZE_MAX / 2), true},
		};
		for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
			result = 0;
			ret = safe_size_t_mul(cases[i].a, cases[i].b, &result);
			ASSERT_BOOL_IDX(cases[i].should_pass, ret, i);
			if (cases[i].should_pass) { ASSERT_EQ_SIZE_T_IDX(cases[i].expected, result, i); }
		}
	}

	/* ---- 表驱动：溢出用例 ---- */

	{
		static const mul_case_t over_cases[] = {
			{SIZE_MAX, 2, SENTINEL_MUL, false},
			{2, SIZE_MAX, SENTINEL_MUL, false},
			{SIZE_MAX, SIZE_MAX, SENTINEL_MUL, false},
			{SIZE_MAX / 2 + 1, 2, SENTINEL_MUL, false},
			{2, SIZE_MAX / 2 + 1, SENTINEL_MUL, false},
		};
		for (size_t i = 0; i < sizeof(over_cases) / sizeof(over_cases[0]); i++) {
			result = SENTINEL_MUL;
			ret = safe_size_t_mul(over_cases[i].a, over_cases[i].b, &result);
			ASSERT_BOOL_IDX(over_cases[i].should_pass, ret, i);
			ASSERT_EQ_SIZE_T_IDX(SENTINEL_MUL, result, i);
		}
	}

	/* ---- 空指针安全性 ---- */

	ret = safe_size_t_mul(3, 7, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);

	ret = safe_size_t_mul(SIZE_MAX, 2, SAFE_CALC_NULLPTR);
	ASSERT_FALSE(ret);

	ret = safe_size_t_mul(0, 999, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);

	ret = safe_size_t_mul(999, 0, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);
}


/*-----------------------------------------------------------------------------
 * safe_size_t_align_up 测试
 *
 * 覆盖：
 *   align 为 0/1、2 的幂（已对齐/未对齐/x==0）、
 *   非 2 的幂（已对齐/未对齐/x<align/x==0）、
 *   多种溢出边缘（含 2 的幂和非 2 的幂两条代码路径）、
 *   空指针安全性。
 *
 * 注意：所有测试用例均不依赖 size_t 的具体位宽（不假定 16/32/64 位），
 * 仅依赖 SIZE_MAX 为全 1（奇数）且位宽 ≥ 16 这一确定事实。
 *---------------------------------------------------------------------------*/
static void test_safe_size_t_align_up(void) {
	size_t result;
	bool ret;

	/* ---- align 为 0：结果为 x ---- */

	result = 0;
	ret = safe_size_t_align_up(81, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(81, result);

	result = 0;
	ret = safe_size_t_align_up(0, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	result = 0;
	ret = safe_size_t_align_up(SIZE_MAX, 0, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	/* ---- align 为 1：结果为 x ---- */

	result = 0;
	ret = safe_size_t_align_up(93, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(93, result);

	result = 0;
	ret = safe_size_t_align_up(SIZE_MAX, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX, result);

	result = 0;
	ret = safe_size_t_align_up(0, 1, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	/* ---- 2 的幂，x 未对齐 ---- */

	result = 0;
	ret = safe_size_t_align_up(131, 64, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(192, result);

	result = 0;
	ret = safe_size_t_align_up(65, 64, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(128, result);

	result = 0;
	ret = safe_size_t_align_up(1, 64, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(64, result);

	/* ---- 2 的幂，x 已对齐 ---- */

	result = 0;
	ret = safe_size_t_align_up(64, 8, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(64, result);

	result = 0;
	ret = safe_size_t_align_up(0, 16, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	result = 0;
	ret = safe_size_t_align_up(128, 128, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(128, result);

	/* ---- 非 2 的幂，x 未对齐 ---- */

	result = 0;
	ret = safe_size_t_align_up(128, 25, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(150, result);

	result = 0;
	ret = safe_size_t_align_up(7, 10, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(10, result);

	result = 0;
	ret = safe_size_t_align_up(0, 7, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(0, result);

	/* ---- 非 2 的幂，x 已对齐 ---- */

	result = 0;
	ret = safe_size_t_align_up(105, 15, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(105, result);

	result = 0;
	ret = safe_size_t_align_up(21, 7, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(21, result);

	/* ---- 非 2 的幂，x == align ---- */

	result = 0;
	ret = safe_size_t_align_up(15, 15, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(15, result);

	result = 0;
	ret = safe_size_t_align_up(7, 7, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(7, result);

	/* ---- 溢出：2 的幂路径 ---- */

	/*
	 * SIZE_MAX 为全 1，未对齐到任何大于 1 的 2 的幂，
	 * 加上 mask（align-1）必然溢出。
	 */
	result = SENTINEL_ALIGN;
	ret = safe_size_t_align_up(SIZE_MAX, 2, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ALIGN, result);

	result = SENTINEL_ALIGN;
	ret = safe_size_t_align_up(SIZE_MAX, 8, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ALIGN, result);

	/*
	 * SIZE_MAX 为全 1，SIZE_MAX % 4 = 3。
	 * SIZE_MAX - 1 未对齐（末 2 位为 10），加 mask=3 得 SIZE_MAX+2 溢出。
	 */
	result = SENTINEL_ALIGN;
	ret = safe_size_t_align_up(SIZE_MAX - 1, 4, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ALIGN, result);

	/*
	 * SIZE_MAX - 3 的末 2 位为 00，已对齐到 4，走快速路径返回原值。
	 * 验证「已对齐大值」不会触发溢出。
	 */
	result = 0;
	ret = safe_size_t_align_up(SIZE_MAX - 3, 4, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX - 3, result);

	/*
	 * SIZE_MAX % 4 = 3，SIZE_MAX - 4 未对齐（末 2 位为 11）。
	 * (SIZE_MAX - 4) + 3 = SIZE_MAX - 1，不溢出。
	 * 结果为 (SIZE_MAX - 1) & ~3 = SIZE_MAX - 3。
	 * 这是 2 的幂路径上「刚好不溢出」的边界值。
	 */
	result = 0;
	ret = safe_size_t_align_up(SIZE_MAX - 4, 4, &result);
	ASSERT_TRUE(ret);
	ASSERT_EQ_SIZE_T(SIZE_MAX - 3, result);

	/* ---- 溢出：非 2 的幂路径 ---- */

	/*
	 * SIZE_MAX - 1 不是 2 的幂（SIZE_MAX 为全 1，SIZE_MAX-1 为 111...110，
	 * 有 n-1 个 1，不是 2 的幂），因此走非 2 的幂路径。
	 *
	 * SIZE_MAX % (SIZE_MAX - 1) = 1 ≠ 0，offset = SIZE_MAX - 2，
	 * SIZE_MAX + (SIZE_MAX - 2) 必定溢出（对任何位宽 n ≥ 2）。
	 * 这是非 2 的幂路径上「必然溢出」的测试。
	 */
	result = SENTINEL_ALIGN;
	ret = safe_size_t_align_up(SIZE_MAX, SIZE_MAX - 1, &result);
	ASSERT_FALSE(ret);
	ASSERT_EQ_SIZE_T(SENTINEL_ALIGN, result);

	/* ---- 表驱动：正常用例 ---- */

	{
		static const align_case_t cases[] = {
			{0, 0, 0, true},
			{0, 1, 0, true},
			{0, 16, 0, true},
			{0, 7, 0, true},
			{81, 0, 81, true},
			{93, 1, 93, true},
			{131, 64, 192, true},
			{64, 8, 64, true},
			{128, 25, 150, true},
			{7, 10, 10, true},
			{105, 15, 105, true},
			{15, 15, 15, true},
			{7, 7, 7, true},
			{SIZE_MAX, 1, SIZE_MAX, true},
			{SIZE_MAX, 0, SIZE_MAX, true},
		};
		for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
			result = 0;
			ret = safe_size_t_align_up(cases[i].x, cases[i].align, &result);
			ASSERT_BOOL_IDX(cases[i].should_pass, ret, i);
			if (cases[i].should_pass) { ASSERT_EQ_SIZE_T_IDX(cases[i].expected, result, i); }
		}
	}

	/* ---- 表驱动：溢出用例 ---- */

	{
		static const align_case_t over_cases[] = {
			{SIZE_MAX, 2, SENTINEL_ALIGN, false},
			{SIZE_MAX, 8, SENTINEL_ALIGN, false},
			{SIZE_MAX - 1, 4, SENTINEL_ALIGN, false},
			{SIZE_MAX, SIZE_MAX - 1, SENTINEL_ALIGN, false},
		};
		for (size_t i = 0; i < sizeof(over_cases) / sizeof(over_cases[0]); i++) {
			result = SENTINEL_ALIGN;
			ret = safe_size_t_align_up(
				over_cases[i].x, over_cases[i].align, &result);
			ASSERT_BOOL_IDX(over_cases[i].should_pass, ret, i);
			ASSERT_EQ_SIZE_T_IDX(SENTINEL_ALIGN, result, i);
		}
	}

	/* ---- 空指针安全性 ---- */

	ret = safe_size_t_align_up(100, 16, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);

	ret = safe_size_t_align_up(SIZE_MAX, 2, SAFE_CALC_NULLPTR);
	ASSERT_FALSE(ret);

	ret = safe_size_t_align_up(0, 0, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);

	ret = safe_size_t_align_up(0, 16, SAFE_CALC_NULLPTR);
	ASSERT_TRUE(ret);
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

	printf("\n===== 结果：%d 个断言，%d 个失败，"
	       "%d 个函数通过，%d 个函数失败 =====\n",
	       g_runner.assert_total, g_runner.assert_fail,
	       g_runner.func_pass, g_runner.func_fail);

	return g_runner.assert_fail > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
