/*
 * SPDX-FileCopyrightText: 2026 mtueih
 * SPDX-License-Identifier: ISC
 */

/*==============================================================================
 * tests/test_safe_calc.c - 项目主库单元测试文件
 *============================================================================*/

/*------------------------------------------------------------------------------
 * 头文件包含
 *----------------------------------------------------------------------------*/
#include "safe_calc/safe_calc.h"

#include <unity.h>

/*------------------------------------------------------------------------------
 * Unity 测试框架需要的函数定义
 *----------------------------------------------------------------------------*/
void setUp(void)
{
}

void tearDown(void)
{
}

/*------------------------------------------------------------------------------
 * 单元测试函数生成宏
 *----------------------------------------------------------------------------*/

/* 无符号整数系列。 */

/* 无符号整数-安全加法-测试函数生成宏。 */
#define SAFE_CALC_UNSIGNED_ADD_TEST_FN_GEN(name, type, max)                                                            \
    void test_safe_##name##_add(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Normal case. */                                                                                             \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)15);                                                                          \
                                                                                                                       \
        /* Lower boundary. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)0, (type)0, &result));                             \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Upper boundary: (MAX - 1) + 1 == MAX. */                                                                    \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)((max) - 1), (type)1, &result));                   \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* Overflow: MAX + 1. */                                                                                       \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_add((type)(max), (type)1, &result));                   \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)10, (type)5, NULL));                               \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_add((type)(max), (type)1, NULL));                      \
    }

/* 无符号整数-安全减法-测试函数生成宏。 */
#define SAFE_CALC_UNSIGNED_SUB_TEST_FN_GEN(name, type, max)                                                            \
    void test_safe_##name##_sub(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Normal case. */                                                                                             \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)5);                                                                           \
                                                                                                                       \
        /* Lower boundary: 0 - 0 == 0. */                                                                              \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)0, (type)0, &result));                             \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Upper boundary: MAX - 0 == MAX. */                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)(max), (type)0, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* Boundary: 1 - 1 == 0. */                                                                                    \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)1, (type)1, &result));                             \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Underflow: 0 - 1. */                                                                                        \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_sub((type)0, (type)1, &result));                       \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)10, (type)5, NULL));                               \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_sub((type)0, (type)1, NULL));                          \
    }

/* 无符号整数-安全乘法-测试函数生成宏。 */
#define SAFE_CALC_UNSIGNED_MUL_TEST_FN_GEN(name, type, max)                                                            \
    void test_safe_##name##_mul(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Normal case. */                                                                                             \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)50);                                                                          \
                                                                                                                       \
        /* Upper boundary: MAX * 1 == MAX. */                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)(max), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* Zero branch: 0 * x == 0. */                                                                                 \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)0, (type)(max), &result));                         \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Zero branch: x * 0 == 0. */                                                                                 \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)(max), (type)0, &result));                         \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Overflow: MAX * 2. */                                                                                       \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mul((type)(max), (type)2, &result));                   \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)10, (type)5, NULL));                               \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mul((type)(max), (type)2, NULL));                      \
    }

/* 无符号整数-安全除法-测试函数生成宏。 */
#define SAFE_CALC_UNSIGNED_DIV_TEST_FN_GEN(name, type, max)                                                            \
    void test_safe_##name##_div(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Normal case. */                                                                                             \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)2);                                                                           \
                                                                                                                       \
        /* Zero numerator. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)0, (type)5, &result));                             \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Upper boundary: MAX / 1 == MAX. */                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)(max), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* Integer truncation. */                                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)10, (type)3, &result));                            \
        TEST_ASSERT_TRUE(result == (type)3);                                                                           \
                                                                                                                       \
        /* Divide by zero. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_div((type)1, (type)0, &result));                 \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)10, (type)5, NULL));                               \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_div((type)1, (type)0, NULL));                    \
    }

/* 无符号整数-安全求模-测试函数生成宏。 */
#define SAFE_CALC_UNSIGNED_MOD_TEST_FN_GEN(name, type, max)                                                            \
    void test_safe_##name##_mod(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Normal case. */                                                                                             \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)10, (type)3, &result));                            \
        TEST_ASSERT_TRUE(result == (type)1);                                                                           \
                                                                                                                       \
        /* Zero numerator. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)0, (type)5, &result));                             \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* MAX % 1 == 0. */                                                                                            \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)(max), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Modulo by zero. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_mod((type)1, (type)0, &result));                 \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)10, (type)3, NULL));                               \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_mod((type)1, (type)0, NULL));                    \
    }

/* 有符号整数系列。 */

/* 有符号整数-安全加法-测试函数生成宏。 */
#define SAFE_CALC_SIGNED_ADD_TEST_FN_GEN(name, type, min, max)                                                         \
    void test_safe_##name##_add(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Positive + positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)15);                                                                          \
                                                                                                                       \
        /* Positive + negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)10, (type) - 5, &result));                         \
        TEST_ASSERT_TRUE(result == (type)5);                                                                           \
                                                                                                                       \
        /* Negative + positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type) - 10, (type)5, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 5);                                                                        \
                                                                                                                       \
        /* Negative + negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type) - 10, (type) - 5, &result));                      \
        TEST_ASSERT_TRUE(result == (type) - 15);                                                                       \
                                                                                                                       \
        /* MAX boundary. */                                                                                            \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)((max) - 1), (type)1, &result));                   \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* MIN boundary. */                                                                                            \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)((min) + 1), (type) - 1, &result));                \
        TEST_ASSERT_TRUE(result == (type)(min));                                                                       \
                                                                                                                       \
        /* Positive overflow: MAX + 1. */                                                                              \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_add((type)(max), (type)1, &result));                   \
                                                                                                                       \
        /* Negative overflow: MIN - 1. */                                                                              \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_add((type)(min), (type) - 1, &result));                \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_add((type)10, (type) - 5, NULL));                            \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_add((type)(max), (type)1, NULL));                      \
    }

/* 有符号整数-安全减法-测试函数生成宏。 */
#define SAFE_CALC_SIGNED_SUB_TEST_FN_GEN(name, type, min, max)                                                         \
    void test_safe_##name##_sub(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Positive - positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)5);                                                                           \
                                                                                                                       \
        /* Positive - negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)10, (type) - 5, &result));                         \
        TEST_ASSERT_TRUE(result == (type)15);                                                                          \
                                                                                                                       \
        /* Negative - positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type) - 10, (type)5, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 15);                                                                       \
                                                                                                                       \
        /* Negative - negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type) - 10, (type) - 5, &result));                      \
        TEST_ASSERT_TRUE(result == (type) - 5);                                                                        \
                                                                                                                       \
        /* MAX boundary: MAX - 0 == MAX. */                                                                            \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)(max), (type)0, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* MIN boundary: MIN - 0 == MIN. */                                                                            \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)(min), (type)0, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(min));                                                                       \
                                                                                                                       \
        /* Positive overflow: MIN - 1. */                                                                              \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_sub((type)(min), (type)1, &result));                   \
                                                                                                                       \
        /* Positive overflow direction: MAX - (-1). */                                                                 \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_sub((type)(max), (type) - 1, &result));                \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_sub((type)10, (type) - 5, NULL));                            \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_sub((type)(min), (type)1, NULL));                      \
    }

/* 有符号整数-安全乘法-测试函数生成宏。 */
#define SAFE_CALC_SIGNED_MUL_TEST_FN_GEN(name, type, min, max)                                                         \
    void test_safe_##name##_mul(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Positive * positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)50);                                                                          \
                                                                                                                       \
        /* Positive * negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)10, (type) - 5, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 50);                                                                       \
                                                                                                                       \
        /* Negative * positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type) - 10, (type)5, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 50);                                                                       \
                                                                                                                       \
        /* Negative * negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type) - 10, (type) - 5, &result));                      \
        TEST_ASSERT_TRUE(result == (type)50);                                                                          \
                                                                                                                       \
        /* Zero paths. */                                                                                              \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)0, (type)(max), &result));                         \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)(min), (type)0, &result));                         \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* MAX * 1 == MAX. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)(max), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* MIN * 1 == MIN. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)(min), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(min));                                                                       \
                                                                                                                       \
        /* Positive * positive overflow: MAX * 2. */                                                                   \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mul((type)(max), (type)2, &result));                   \
                                                                                                                       \
        /* Negative * positive overflow: MIN * 2. */                                                                   \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mul((type)(min), (type)2, &result));                   \
                                                                                                                       \
        /* Positive * negative overflow: 2 * MIN. */                                                                   \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mul((type)2, (type)(min), &result));                   \
                                                                                                                       \
        /* Negative * negative overflow: MIN * (-1). */                                                                \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mul((type)(min), (type) - 1, &result));                \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mul((type)10, (type) - 5, NULL));                            \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mul((type)(min), (type) - 1, NULL));                   \
    }

/* 有符号整数-安全除法-测试函数生成宏。 */
#define SAFE_CALC_SIGNED_DIV_TEST_FN_GEN(name, type, min, max)                                                         \
    void test_safe_##name##_div(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Positive / positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)10, (type)5, &result));                            \
        TEST_ASSERT_TRUE(result == (type)2);                                                                           \
                                                                                                                       \
        /* Negative / positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type) - 10, (type)5, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 2);                                                                        \
                                                                                                                       \
        /* Positive / negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)10, (type) - 5, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 2);                                                                        \
                                                                                                                       \
        /* Negative / negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type) - 10, (type) - 5, &result));                      \
        TEST_ASSERT_TRUE(result == (type)2);                                                                           \
                                                                                                                       \
        /* MAX / 1 == MAX. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)(max), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(max));                                                                       \
                                                                                                                       \
        /* MIN / 1 == MIN. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)(min), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)(min));                                                                       \
                                                                                                                       \
        /* Integer truncation toward zero. */                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type)10, (type)3, &result));                            \
        TEST_ASSERT_TRUE(result == (type)3);                                                                           \
                                                                                                                       \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type) - 10, (type)3, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 3);                                                                        \
                                                                                                                       \
        /* Divide by zero. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_div((type)1, (type)0, &result));                 \
                                                                                                                       \
        /* MIN / -1 is not representable. */                                                                           \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_div((type)(min), (type) - 1, &result));                \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_div((type) - 10, (type)5, NULL));                            \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_div((type)1, (type)0, NULL));                    \
    }

/* 有符号整数-安全求模-测试函数生成宏。 */
#define SAFE_CALC_SIGNED_MOD_TEST_FN_GEN(name, type, min, max)                                                         \
    void test_safe_##name##_mod(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Positive % positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)10, (type)3, &result));                            \
        TEST_ASSERT_TRUE(result == (type)1);                                                                           \
                                                                                                                       \
        /* Negative % positive. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type) - 10, (type)3, &result));                         \
        TEST_ASSERT_TRUE(result == (type) - 1);                                                                        \
                                                                                                                       \
        /* Positive % negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)10, (type) - 3, &result));                         \
        TEST_ASSERT_TRUE(result == (type)1);                                                                           \
                                                                                                                       \
        /* Negative % negative. */                                                                                     \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type) - 10, (type) - 3, &result));                      \
        TEST_ASSERT_TRUE(result == (type) - 1);                                                                        \
                                                                                                                       \
        /* MAX % 1 == 0. */                                                                                            \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)(max), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* MIN % 1 == 0. */                                                                                            \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type)(min), (type)1, &result));                         \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Modulo by zero. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_mod((type)1, (type)0, &result));                 \
                                                                                                                       \
        /* MIN % -1 is the same exceptional case as MIN / -1. */                                                       \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_mod((type)(min), (type) - 1, &result));                \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_mod((type) - 10, (type)3, NULL));                            \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_DIVIDE_BY_ZERO, safe_##name##_mod((type)1, (type)0, NULL));                    \
    }

/* 有符号整数-安全取相反数-测试函数生成宏。 */
#define SAFE_CALC_SIGNED_NEG_TEST_FN_GEN(name, type, min, max)                                                         \
    void test_safe_##name##_neg(void)                                                                                  \
    {                                                                                                                  \
        type result;                                                                                                   \
                                                                                                                       \
        /* Zero. */                                                                                                    \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_neg((type)0, &result));                                      \
        TEST_ASSERT_TRUE(result == (type)0);                                                                           \
                                                                                                                       \
        /* Positive value. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_neg((type)5, &result));                                      \
        TEST_ASSERT_TRUE(result == (type) - 5);                                                                        \
                                                                                                                       \
        /* Negative value. */                                                                                          \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_neg((type) - 5, &result));                                   \
        TEST_ASSERT_TRUE(result == (type)5);                                                                           \
                                                                                                                       \
        /* MAX -> -MAX. */                                                                                             \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_neg((type)(max), &result));                                  \
        TEST_ASSERT_TRUE(result == (type)(-(max)));                                                                    \
                                                                                                                       \
        /* MIN cannot be negated. */                                                                                   \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_neg((type)(min), &result));                            \
                                                                                                                       \
        /* NULL result is valid on success. */                                                                         \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OK, safe_##name##_neg((type)5, NULL));                                         \
                                                                                                                       \
        /* NULL result is also valid on error. */                                                                      \
        TEST_ASSERT_EQUAL_INT(SAFE_CALC_OVERFLOW, safe_##name##_neg((type)(min), NULL));                               \
    }

/*------------------------------------------------------------------------------
 * 测试函数生成/定义
 *----------------------------------------------------------------------------*/

/* 无符号整数系列测试函数生成。 */

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_ADD_TEST_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_SUB_TEST_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_MUL_TEST_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_DIV_TEST_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_MOD_TEST_FN_GEN)

/* 有符号整数系列测试函数生成。 */

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_ADD_TEST_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_SUB_TEST_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_MUL_TEST_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_DIV_TEST_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_MOD_TEST_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_NEG_TEST_FN_GEN)

/*------------------------------------------------------------------------------
 * Unity 测试框架-测试函数运行宏
 *----------------------------------------------------------------------------*/

/* 无符号整数系列。 */

#define SAFE_CALC_UNSIGNED_ADD_TEST_RUN(name, type, max) RUN_TEST(test_safe_##name##_add);

#define SAFE_CALC_UNSIGNED_SUB_TEST_RUN(name, type, max) RUN_TEST(test_safe_##name##_sub);

#define SAFE_CALC_UNSIGNED_MUL_TEST_RUN(name, type, max) RUN_TEST(test_safe_##name##_mul);

#define SAFE_CALC_UNSIGNED_DIV_TEST_RUN(name, type, max) RUN_TEST(test_safe_##name##_div);

#define SAFE_CALC_UNSIGNED_MOD_TEST_RUN(name, type, max) RUN_TEST(test_safe_##name##_mod);

/* 有符号整数系列。 */

#define SAFE_CALC_SIGNED_ADD_TEST_RUN(name, type, min, max) RUN_TEST(test_safe_##name##_add);

#define SAFE_CALC_SIGNED_SUB_TEST_RUN(name, type, min, max) RUN_TEST(test_safe_##name##_sub);

#define SAFE_CALC_SIGNED_MUL_TEST_RUN(name, type, min, max) RUN_TEST(test_safe_##name##_mul);

#define SAFE_CALC_SIGNED_DIV_TEST_RUN(name, type, min, max) RUN_TEST(test_safe_##name##_div);

#define SAFE_CALC_SIGNED_MOD_TEST_RUN(name, type, min, max) RUN_TEST(test_safe_##name##_mod);

#define SAFE_CALC_SIGNED_NEG_TEST_RUN(name, type, min, max) RUN_TEST(test_safe_##name##_neg);

/*------------------------------------------------------------------------------
 * main() 函数定义
 *----------------------------------------------------------------------------*/

int main(void)
{
    UNITY_BEGIN();

    /* Unity 测试框架-运行所有无符号整数测试函数。 */

    SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_ADD_TEST_RUN)

    SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_SUB_TEST_RUN)

    SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_MUL_TEST_RUN)

    SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_DIV_TEST_RUN)

    SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_MOD_TEST_RUN)

    /* Unity 测试框架-运行所有有符号整数测试函数。 */

    SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_ADD_TEST_RUN)

    SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_SUB_TEST_RUN)

    SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_MUL_TEST_RUN)

    SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_DIV_TEST_RUN)

    SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_MOD_TEST_RUN)

    SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_NEG_TEST_RUN)

    return UNITY_END();
}
