**English** | [简体中文](api-reference.zh-CN.md)

# API Reference

This library provides the following API functions:

| Function                                          | Summary                                                                   |
|---------------------------------------------------|---------------------------------------------------------------------------|
| [`safe_size_t_add()`](#safe_size_t_add)           | Safely perform a `size_t` addition, preventing overflow.                  |
| [`safe_size_t_mul()`](#safe_size_t_mul)           | Safely perform a `size_t` multiplication, preventing overflow.            |
| [`safe_size_t_align_up()`](#safe_size_t_align_up) | Safely align a `size_t` up to a multiple of a value, preventing overflow. |

## `safe_size_t_add()`

Safely perform a `size_t` addition, preventing overflow.

```c
bool safe_size_t_add(
	size_t a,
	size_t b,
	size_t *result
);
```

Parameters

- `a`

  The first operand.

- `b`

  The second operand.

- `result`

  A pointer to the `size_t` variable that stores the result.

  Nothing is written when it is a *null pointer* or *would overflow*.

Return value

(`bool`) Returns *`true`* if there is no overflow, otherwise returns *`false`*.

## `safe_size_t_mul()`

Safely perform a `size_t` multiplication, preventing overflow.

```c
bool safe_size_t_mul(
	size_t a,
	size_t b,
	size_t *result
);
```

Parameters

- `a`

  The first operand.

- `b`

  The second operand.

- `result`

  A pointer to the `size_t` variable that stores the result.

  Nothing is written when it is a *null pointer* or *would overflow*.

Return value

(`bool`) Returns *`true`* if there is no overflow, otherwise returns *`false`*.

## `safe_size_t_align_up()`

Safely align a `size_t` up to a multiple of a value, preventing overflow.

```c
bool safe_size_t_align_up(
	size_t x,
	size_t align,
	size_t *result
);
```

Parameters

- `x`

  The value to align.

- `align`

  The alignment value.

  If it is *`0`*, the result is *`x`* (treat *`0`* as not aligning to any value).

- `result`

  A pointer to the `size_t` variable that stores the result.

  Nothing is written when it is a *null pointer* or *would overflow*.

Return value

(`bool`) Returns *`true`* if there is no overflow, otherwise returns *`false`*.
