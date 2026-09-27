/*
 * SPDX-FileCopyrightText: 2026 mtueih
 * SPDX-License-Identifier: ISC
 */

/*==============================================================================
 * include/safe_calc.h - 项目主库头文件
 *============================================================================*/
#ifndef SAFE_CALC_H
#define SAFE_CALC_H

/*------------------------------------------------------------------------------
 * 头文件包含
 *----------------------------------------------------------------------------*/
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

/*------------------------------------------------------------------------------
 * 宏定义
 *----------------------------------------------------------------------------*/

/* C++ 兼容包裹宏。 */
#ifdef __cplusplus
#define SAFE_CALC_EXTERN_C_BEGIN                                                                                       \
    extern "C"                                                                                                         \
    {
#define SAFE_CALC_EXTERN_C_END }
#else
#define SAFE_CALC_EXTERN_C_BEGIN
#define SAFE_CALC_EXTERN_C_END
#endif

/* API 函数声明符。 */
#define SAFE_CALC_API static inline

/* C++ 兼容包裹宏-开始。 */
SAFE_CALC_EXTERN_C_BEGIN

/*------------------------------------------------------------------------------
 * 错误码定义
 *----------------------------------------------------------------------------*/
typedef enum
{
    SAFE_CALC_OK = 0,
    SAFE_CALC_OVERFLOW,
    SAFE_CALC_DIVIDE_BY_ZERO
} safe_calc_error_t;

/*------------------------------------------------------------------------------
 * API 函数生成宏
 *----------------------------------------------------------------------------*/

/* 无符号整数系列。 */

/* 无符号整数-安全加法-API 函数生成宏。 */
#define SAFE_CALC_UNSIGNED_INTEGER_ADD_FN_GEN(name, type, max)                                                         \
    SAFE_CALC_API safe_calc_error_t safe_##name##_add(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (a > (max) - b)                                                                                             \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a + b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 无符号整数-安全减法-API 函数生成宏。 */
#define SAFE_CALC_UNSIGNED_INTEGER_SUB_FN_GEN(name, type, max)                                                         \
    SAFE_CALC_API safe_calc_error_t safe_##name##_sub(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (a < b)                                                                                                     \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a - b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 无符号整数-安全乘法-API 函数生成宏。 */
#define SAFE_CALC_UNSIGNED_INTEGER_MUL_FN_GEN(name, type, max)                                                         \
    SAFE_CALC_API safe_calc_error_t safe_##name##_mul(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (a == 0 || b == 0)                                                                                          \
        {                                                                                                              \
            if (result != NULL)                                                                                        \
                *result = 0;                                                                                           \
                                                                                                                       \
            return SAFE_CALC_OK;                                                                                       \
        }                                                                                                              \
                                                                                                                       \
        if (b > (max) / a)                                                                                             \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a * b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 无符号整数-安全除法-API 函数生成宏。 */
#define SAFE_CALC_UNSIGNED_INTEGER_DIV_FN_GEN(name, type, max)                                                         \
    SAFE_CALC_API safe_calc_error_t safe_##name##_div(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (b == 0)                                                                                                    \
            return SAFE_CALC_DIVIDE_BY_ZERO;                                                                           \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a / b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 无符号整数-安全求模-API 函数生成宏。 */
#define SAFE_CALC_UNSIGNED_INTEGER_MOD_FN_GEN(name, type, max)                                                         \
    SAFE_CALC_API safe_calc_error_t safe_##name##_mod(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (b == 0)                                                                                                    \
            return SAFE_CALC_DIVIDE_BY_ZERO;                                                                           \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a % b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 有符号整数系列。 */

/* 有符号整数-安全加法-API 函数生成宏。 */
#define SAFE_CALC_SIGNED_INTEGER_ADD_FN_GEN(name, type, min, max)                                                      \
    SAFE_CALC_API safe_calc_error_t safe_##name##_add(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (b > 0 && a > (max) - b)                                                                                    \
            return SAFE_CALC_OVERFLOW;                                                                                 \
        if (b < 0 && a < (min) - b)                                                                                    \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a + b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 有符号整数-安全减法-API 函数生成宏。 */
#define SAFE_CALC_SIGNED_INTEGER_SUB_FN_GEN(name, type, min, max)                                                      \
    SAFE_CALC_API safe_calc_error_t safe_##name##_sub(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (b > 0 && a < (min) + b)                                                                                    \
            return SAFE_CALC_OVERFLOW;                                                                                 \
        if (b < 0 && a > (max) + b)                                                                                    \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a - b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 有符号整数-安全乘法-API 函数生成宏。 */
#define SAFE_CALC_SIGNED_INTEGER_MUL_FN_GEN(name, type, min, max)                                                      \
    SAFE_CALC_API safe_calc_error_t safe_##name##_mul(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (a == 0 || b == 0)                                                                                          \
        {                                                                                                              \
            if (result != NULL)                                                                                        \
                *result = 0;                                                                                           \
            return SAFE_CALC_OK;                                                                                       \
        }                                                                                                              \
                                                                                                                       \
        if (a > 0)                                                                                                     \
        {                                                                                                              \
            if (b > 0)                                                                                                 \
            {                                                                                                          \
                if (a > (max) / b)                                                                                     \
                    return SAFE_CALC_OVERFLOW;                                                                         \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                if (b < (min) / a)                                                                                     \
                    return SAFE_CALC_OVERFLOW;                                                                         \
            }                                                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            if (b > 0)                                                                                                 \
            {                                                                                                          \
                if (a < (min) / b)                                                                                     \
                    return SAFE_CALC_OVERFLOW;                                                                         \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                if (b < (max) / a)                                                                                     \
                    return SAFE_CALC_OVERFLOW;                                                                         \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a * b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 有符号整数-安全除法-API 函数生成宏。 */
#define SAFE_CALC_SIGNED_INTEGER_DIV_FN_GEN(name, type, min, max)                                                      \
    SAFE_CALC_API safe_calc_error_t safe_##name##_div(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (b == 0)                                                                                                    \
            return SAFE_CALC_DIVIDE_BY_ZERO;                                                                           \
                                                                                                                       \
        if (a == (min) && b == -1)                                                                                     \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a / b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 有符号整数-安全求模-API 函数生成宏。 */
#define SAFE_CALC_SIGNED_INTEGER_MOD_FN_GEN(name, type, min, max)                                                      \
    SAFE_CALC_API safe_calc_error_t safe_##name##_mod(type a, type b, type *result)                                    \
    {                                                                                                                  \
        if (b == 0)                                                                                                    \
            return SAFE_CALC_DIVIDE_BY_ZERO;                                                                           \
                                                                                                                       \
        if (a == (min) && b == -1)                                                                                     \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = a % b;                                                                                           \
        return SAFE_CALC_OK;                                                                                           \
    }

/* 有符号整数-安全取相反数-API 函数生成宏。 */
#define SAFE_CALC_SIGNED_INTEGER_NEG_FN_GEN(name, type, min, max)                                                      \
    SAFE_CALC_API safe_calc_error_t safe_##name##_neg(type a, type *result)                                            \
    {                                                                                                                  \
        if (a == (min))                                                                                                \
            return SAFE_CALC_OVERFLOW;                                                                                 \
                                                                                                                       \
        if (result != NULL)                                                                                            \
            *result = -a;                                                                                              \
        return SAFE_CALC_OK;                                                                                           \
    }

/*------------------------------------------------------------------------------
 * X-Macro 类型列表
 *----------------------------------------------------------------------------*/

/* 无符号整数 X-Macro 类型列表。 */
#define SAFE_CALC_UNSIGNED_INTEGER_TYPES(X)                                                                            \
    X(uchar, unsigned char, UCHAR_MAX)                                                                                 \
    X(ushort, unsigned short, USHRT_MAX)                                                                               \
    X(uint, unsigned int, UINT_MAX)                                                                                    \
    X(ulong, unsigned long, ULONG_MAX)                                                                                 \
    X(ullong, unsigned long long, ULLONG_MAX)                                                                          \
    X(uintmax, uintmax_t, UINTMAX_MAX)                                                                                 \
    X(size, size_t, SIZE_MAX)                                                                                          \
    X(uleast8, uint_least8_t, UINT_LEAST8_MAX)                                                                         \
    X(uleast16, uint_least16_t, UINT_LEAST16_MAX)                                                                      \
    X(uleast32, uint_least32_t, UINT_LEAST32_MAX)                                                                      \
    X(uleast64, uint_least64_t, UINT_LEAST64_MAX)                                                                      \
    X(ufast8, uint_fast8_t, UINT_FAST8_MAX)                                                                            \
    X(ufast16, uint_fast16_t, UINT_FAST16_MAX)                                                                         \
    X(ufast32, uint_fast32_t, UINT_FAST32_MAX)                                                                         \
    X(ufast64, uint_fast64_t, UINT_FAST64_MAX)

/* 有符号整数 X-Macro 类型列表。 */
#define SAFE_CALC_SIGNED_INTEGER_TYPES(X)                                                                              \
    X(schar, signed char, SCHAR_MIN, SCHAR_MAX)                                                                        \
    X(short, short, SHRT_MIN, SHRT_MAX)                                                                                \
    X(int, int, INT_MIN, INT_MAX)                                                                                      \
    X(long, long, LONG_MIN, LONG_MAX)                                                                                  \
    X(llong, long long, LLONG_MIN, LLONG_MAX)                                                                          \
    X(intmax, intmax_t, INTMAX_MIN, INTMAX_MAX)                                                                        \
    X(ptrdiff, ptrdiff_t, PTRDIFF_MIN, PTRDIFF_MAX)                                                                    \
    X(ileast8, int_least8_t, INT_LEAST8_MIN, INT_LEAST8_MAX)                                                           \
    X(ileast16, int_least16_t, INT_LEAST16_MIN, INT_LEAST16_MAX)                                                       \
    X(ileast32, int_least32_t, INT_LEAST32_MIN, INT_LEAST32_MAX)                                                       \
    X(ileast64, int_least64_t, INT_LEAST64_MIN, INT_LEAST64_MAX)                                                       \
    X(ifast8, int_fast8_t, INT_FAST8_MIN, INT_FAST8_MAX)                                                               \
    X(ifast16, int_fast16_t, INT_FAST16_MIN, INT_FAST16_MAX)                                                           \
    X(ifast32, int_fast32_t, INT_FAST32_MIN, INT_FAST32_MAX)                                                           \
    X(ifast64, int_fast64_t, INT_FAST64_MIN, INT_FAST64_MAX)

/*------------------------------------------------------------------------------
 * API 函数生成/定义
 *----------------------------------------------------------------------------*/

/* 无符号整数系列 API 函数生成。 */

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_INTEGER_ADD_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_INTEGER_SUB_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_INTEGER_MUL_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_INTEGER_DIV_FN_GEN)

SAFE_CALC_UNSIGNED_INTEGER_TYPES(SAFE_CALC_UNSIGNED_INTEGER_MOD_FN_GEN)

/* 有符号整数系列 API 函数生成。 */

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_INTEGER_ADD_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_INTEGER_SUB_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_INTEGER_MUL_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_INTEGER_DIV_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_INTEGER_MOD_FN_GEN)

SAFE_CALC_SIGNED_INTEGER_TYPES(SAFE_CALC_SIGNED_INTEGER_NEG_FN_GEN)

/* C++ 兼容包裹宏-结束。 */
SAFE_CALC_EXTERN_C_END

#endif /* SAFE_CALC_H */
