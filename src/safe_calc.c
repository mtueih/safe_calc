#include "safe_calc.h"

#include <limits.h>

bool safe_size_add(const size_t a, const size_t b, size_t *const result) {
	if (a > SIZE_MAX - b) {
		return false; // 溢出
	}
	*result = a + b;
	return true;
}