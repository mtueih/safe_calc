#include "safe_calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int main(void) {
	printf("Hello, safe_calc!\n");

	size_t result = 666;
	size_t a, b;

	// 加法测试
	a = (size_t)-1;
	b = 1;

	if (!safe_size_add(a, b, &result)) {
		printf("size_t 加法（%zu + %zu）上溢！result：%zu（不变）\n", a, b, result);
	} else {
		printf("size_t 加法（%zu + %zu）正常！result：%zu\n", a, b, result);
	}


	return EXIT_SUCCESS;
}