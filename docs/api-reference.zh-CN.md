<div align="right">

[English](api-reference.md) | **简体中文**

</div>

# API 参考

此库包含针对无符号、有符号整数的加、减、乘、除、求模、取相反数的安全运算函数。每种运算又都针对不同的具体类型提供了不同的版本。

整个库的 API 函数名称遵循一定的规律：`safe_<数值类型名称>_<运算类型名称>`，根据*数值类型*与*运算类型*即可拼凑出对应的 API 函数名称，因此就不一一列出了。

## 运算类型

每种运算类型对应的*用于 API 函数名称的运算类型名称*及所支持的*数值类型类别*情况如下：

| 运算类型 | _运算类型名称_ | 无符号整数 | 有符号整数 |
| -------- | -------------- | ---------- | ---------- |
| 加       | `add`          | ✅         | ✅         |
| 减       | `sub`          | ✅         | ✅         |
| 乘       | `mul`          | ✅         | ✅         |
| 除       | `div`          | ✅         | ✅         |
| 求模     | `mod`          | ✅         | ✅         |
| 取相反数 | `neg`          | ❌         | ✅         |

## 数值类型

每个类别的数值类型，支持的所有**具体类型**，以及它们对应的*用于 API 函数名称的数值类型名称*情况如下：

### 无符号整数

基本类型：

| 具体类型                    | _数值类型名称_ |
| --------------------------- | -------------- |
| `unsigned char`             | `uchar`        |
| `unsigned short`            | `ushort`       |
| `unsigned int`              | `uint`         |
| `unsigned long`             | `ulong`        |
| `unsigned long long`（C99） | `ullong`       |

其他类型：

| 具体类型                              | _数值类型名称_ |
| ------------------------------------- | -------------- |
| `uintmax_t`（`stdint.h`）（C99）      | `uintmax`      |
| `size_t`（`stddef.h`）                | `size`         |
| `uint_least8_t`（`stdint.h`）（C99）  | `uleast8`      |
| `uint_least16_t`（`stdint.h`）（C99） | `uleast16`     |
| `uint_least32_t`（`stdint.h`）（C99） | `uleast32`     |
| `uint_least64_t`（`stdint.h`）（C99） | `uleast64`     |
| `uint_fast8_t`（`stdint.h`）（C99）   | `ufast8`       |
| `uint_fast16_t`（`stdint.h`）（C99）  | `ufast16`      |
| `uint_fast32_t`（`stdint.h`）（C99）  | `ufast32`      |
| `uint_fast64_t`（`stdint.h`）（C99）  | `ufast64`      |

### 有符号整数

基本类型：

| 具体类型           | _数值类型名称_ |
| ------------------ | -------------- |
| `signed char`      | `schar`        |
| `short`            | `short`        |
| `int`              | `int`          |
| `long`             | `long`         |
| `long long`（C99） | `llong`        |

其他类型：

| 具体类型                             | _数值类型名称_ |
| ------------------------------------ | -------------- |
| `intmax_t`（`stdint.h`）（C99）      | `intmax`       |
| `ptrdiff_t`（`stddef.h`）            | `ptrdiff`      |
| `int_least8_t`（`stdint.h`）（C99）  | `ileast8`      |
| `int_least16_t`（`stdint.h`）（C99） | `ileast16`     |
| `int_least32_t`（`stdint.h`）（C99） | `ileast32`     |
| `int_least64_t`（`stdint.h`）（C99） | `ileast64`     |
| `int_fast8_t`（`stdint.h`）（C99）   | `ifast8`       |
| `int_fast16_t`（`stdint.h`）（C99）  | `ifast16`      |
| `int_fast32_t`（`stdint.h`）（C99）  | `ifast32`      |
| `int_fast64_t`（`stdint.h`）（C99）  | `ifast64`      |

## API 函数行为

此库所有 API 函数的名称遵循一定的规律，函数行为也一样。

### 返回值

此库所有 API 函数的返回值类型均为 `safe_calc_error_t`，即错误码，其定义如下：

```c
typedef enum
{
    SAFE_CALC_OK = 0,
    SAFE_CALC_OVERFLOW,
    SAFE_CALC_DIVIDE_BY_ZERO
} safe_calc_error_t;
```

支持的错误码及其含义如下：

| 错误码                     | 含义                     |
| -------------------------- | ------------------------ |
| `SAFE_CALC_OK`             | 表示运算可以成功完成。   |
| `SAFE_CALC_OVERFLOW`       | 表示运算会发生溢出。     |
| `SAFE_CALC_DIVIDE_BY_ZERO` | 表示运算会发生除零错误。 |

### 参数

此库所有 API 函数的参数结构的都是一致的：**_操作数_**（输入参数） + **_接收运算结果的变量的指针_**（输出参数）。

操作数个数与运算类型有关。操作数的语义顺序，遵循从左到右。

### 行为

**输出参数**（接收运算结果的变量的指针）是**可选**的。为*空指针*不会影响函数返回值，通常用于仅测试运算是否可以成功执行。

此库所有 API 函数都遵循“**先判断，再计算**”的结构。因此，
**输出参数**不为*空指针*时，也仅在运算可以成功执行时写入运算结果。
