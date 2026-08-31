[English](api-reference.md) | **简体中文**

# API 参考

此库包含以下 API 函数：

| 函数名                                            | 简述                                                        |
|---------------------------------------------------|-------------------------------------------------------------|
| [`safe_size_t_add()`](#safe_size_t_add)           | 安全地执行【`size_t` 加法】，防止上溢。                     |
| [`safe_size_t_mul()`](#safe_size_t_mul)           | 安全地执行【`size_t` 乘法】，防止上溢。                     |
| [`safe_size_t_align_up()`](#safe_size_t_align_up) | 安全地执行【`size_t` 向上对齐到某个数的整数倍】，防止上溢。 |

## `safe_size_t_add()`

安全地执行【`size_t` 加法】，防止上溢。

```c
bool safe_size_t_add(
	size_t a,
	size_t b,
	size_t *result
);
```

参数

- `a`

  第一个操作数。

- `b`

  第二个操作数。

- `result`

  指向「存储执行结果的 `size_t` 变量」的指针。

  为*空指针*或*会溢出*时不写入。

返回值

（`bool`）如果没溢出则返回 *`true`*，否则返回 *`false`*。

## `safe_size_t_mul()`

安全地执行【`size_t` 乘法】，防止上溢。

```c
bool safe_size_t_mul(
	size_t a,
	size_t b,
	size_t *result
);
```

参数

- `a`

  第一个操作数。

- `b`

  第二个操作数。

- `result`

  指向「存储执行结果的 `size_t` 变量」的指针。

  为*空指针*或*会溢出*时不写入。

返回值

（`bool`）如果没溢出则返回 *`true`*，否则返回 *`false`*。

## `safe_size_t_align_up()`

安全地执行【`size_t` 向上对齐到某个数的整数倍】，防止上溢。

```c
bool safe_size_t_align_up(
	size_t x,
	size_t align,
	size_t *result
);
```

参数

- `x`

  要对齐的数。

- `align`

  对齐值。

  如果为 *`0`*，则结果为 *`x`*（视 *`0`* 为不对齐到任何值）。

- `result`

  指向「存储执行结果的 size_t 变量」的指针。

  为*空指针*或*会溢出*时不写入。

返回值

（`bool`）如果没溢出则返回 *`true`*，否则返回 *`false`*。
