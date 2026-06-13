#include <safe_calc.h>
#include <stdio.h>
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

#include <stdlib.h>

int main(void) {
	printf("Hello, safe_calc!\n");

	size_t result = 666;
	size_t a, b;

	// 加法测试。
	a = (size_t) -1;
	b = 1;

	if (!safe_size_add(a, b, &result)) {
		printf("【溢出】size_t 加法：%zu + %zu = ?（结果不变，仍为 %zu）\n", a, b, result);
	} else {
		printf("【成功】size_t 加法：%zu + %zu = %zu\n", a, b, result);
	}

	// 减法测试。
	a = 0;
	b = 1;

	if (!safe_size_sub(a, b, &result)) {
		printf("【溢出】size_t 减法：%zu - %zu = ?（结果不变，仍为 %zu）\n", a, b, result);
	} else {
		printf("【成功】size_t 减法：%zu - %zu = %zu\n", a, b, result);
	}

	// 乘法测试。
	a = (size_t) -1;
	b = 2;

	if (!safe_size_mul(a, b, &result)) {
		printf("【溢出】size_t 乘法：%zu * %zu = ?（结果不变，仍为 %zu）\n", a, b, result);
	} else {
		printf("【成功】size_t 乘法：%zu * %zu = %zu\n", a, b, result);
	}

	// 对齐测试。
	b = ((size_t) -1 >> 2) * 3;
	a = b + 1;

	if (!safe_size_align_up(a, b, &result)) {
		printf("【溢出】size_t 向上对齐：%zu ⬆ %zu = ?（结果不变，仍为 %zu）\n", a, b, result);
	} else {
		printf("【成功】size_t 向上对齐：%zu ⬆ %zu = %zu\n", a, b, result);
	}


	return EXIT_SUCCESS;
}
