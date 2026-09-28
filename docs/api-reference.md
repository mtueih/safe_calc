<div align="right">

**English** | [简体中文](api-reference.zh-CN.md)

</div>

# API Reference

This library provides safe calculation functions for addition, subtraction, multiplication, division, modulo, and negation of unsigned and signed integers. Each calculation also has dedicated versions for different concrete types.

The API function names of the entire library follow a consistent pattern: `safe_<numeric_type_name>_<calculation_type_name>`. The corresponding API function name can be constructed from the _numeric type_ and _calculation type_, so they are not listed individually.

## Calculation Types

The _calculation type name used in API function names_ and the supported _numeric type categories_ for each calculation type are as follows:

| Calculation Type | _Calculation Type Name_ | Unsigned Integer | Signed Integer |
| ---------------- | ----------------------- | ---------------- | -------------- |
| Addition         | `add`                   | ✅               | ✅             |
| Subtraction      | `sub`                   | ✅               | ✅             |
| Multiplication   | `mul`                   | ✅               | ✅             |
| Division         | `div`                   | ✅               | ✅             |
| Modulo           | `mod`                   | ✅               | ✅             |
| Negation         | `neg`                   | ❌               | ✅             |

## Numeric Types

For each category of numeric types, all supported **concrete types** and their corresponding _numeric type names used in API function names_ are as follows:

### Unsigned Integers

Basic types:

| Concrete Type              | _Numeric Type Name_ |
| -------------------------- | ------------------- |
| `unsigned char`            | `uchar`             |
| `unsigned short`           | `ushort`            |
| `unsigned int`             | `uint`              |
| `unsigned long`            | `ulong`             |
| `unsigned long long` (C99) | `ullong`            |

Other types:

| Concrete Type                       | _Numeric Type Name_ |
| ----------------------------------- | ------------------- |
| `uintmax_t` (`stdint.h`) (C99)      | `uintmax`           |
| `size_t` (`stddef.h`)               | `size`              |
| `uint_least8_t` (`stdint.h`) (C99)  | `uleast8`           |
| `uint_least16_t` (`stdint.h`) (C99) | `uleast16`          |
| `uint_least32_t` (`stdint.h`) (C99) | `uleast32`          |
| `uint_least64_t` (`stdint.h`) (C99) | `uleast64`          |
| `uint_fast8_t` (`stdint.h`) (C99)   | `ufast8`            |
| `uint_fast16_t` (`stdint.h`) (C99)  | `ufast16`           |
| `uint_fast32_t` (`stdint.h`) (C99)  | `ufast32`           |
| `uint_fast64_t` (`stdint.h`) (C99)  | `ufast64`           |

### Signed Integers

Basic types:

| Concrete Type     | _Numeric Type Name_ |
| ----------------- | ------------------- |
| `signed char`     | `schar`             |
| `short`           | `short`             |
| `int`             | `int`               |
| `long`            | `long`              |
| `long long` (C99) | `llong`             |

Other types:

| Concrete Type                      | _Numeric Type Name_ |
| ---------------------------------- | ------------------- |
| `intmax_t` (`stdint.h`) (C99)      | `intmax`            |
| `ptrdiff_t` (`stddef.h`)           | `ptrdiff`           |
| `int_least8_t` (`stdint.h`) (C99)  | `ileast8`           |
| `int_least16_t` (`stdint.h`) (C99) | `ileast16`          |
| `int_least32_t` (`stdint.h`) (C99) | `ileast32`          |
| `int_least64_t` (`stdint.h`) (C99) | `ileast64`          |
| `int_fast8_t` (`stdint.h`) (C99)   | `ifast8`            |
| `int_fast16_t` (`stdint.h`) (C99)  | `ifast16`           |
| `int_fast32_t` (`stdint.h`) (C99)  | `ifast32`           |
| `int_fast64_t` (`stdint.h`) (C99)  | `ifast64`           |

## API Function Behavior

All API function names in this library follow a consistent pattern, and so does their behavior.

### Return Value

The return type of all API functions in this library is `safe_calc_error_t`, which is an error code defined as follows:

```c
typedef enum
{
    SAFE_CALC_OK = 0,
    SAFE_CALC_OVERFLOW,
    SAFE_CALC_DIVIDE_BY_ZERO
} safe_calc_error_t;
```

The supported error codes and their meanings are as follows:

| Error Code                 | Meaning                                                            |
| -------------------------- | ------------------------------------------------------------------ |
| `SAFE_CALC_OK`             | Indicates that the calculation can be completed successfully.      |
| `SAFE_CALC_OVERFLOW`       | Indicates that the calculation would overflow.                     |
| `SAFE_CALC_DIVIDE_BY_ZERO` | Indicates that the calculation would cause a divide-by-zero error. |

### Parameters

The parameter structure of all API functions in this library is consistent: **_operands_** (input parameters) + **_pointer to the variable that receives the calculation result_** (output parameter).

The number of operands depends on the calculation type. The semantic order of the operands follows left-to-right order.

### Behavior

The **output parameter** (pointer to the variable that receives the calculation result) is **optional**. Passing a _null pointer_ does not affect the function’s return value and is typically used to test only whether the calculation can be performed successfully.

All API functions in this library follow a “**check first, then calculate**” structure. Therefore, when the **output parameter** is not a _null pointer_, the calculation result is written only if the calculation can be performed successfully.
