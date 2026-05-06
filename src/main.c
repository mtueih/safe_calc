#include "safe_calc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int main(void) {
	printf("Hello, safe_calc!\n");

	size_t result = 666;
	size_t a, b;

	// 加法测试
	a = 0;
	a -= 1;

	if (!safe_size_add(a, 1, &result)) {
		printf("加法上溢！result：%zu（不变）\n", result);
	} else {
		printf("加法正常！result：%zu\n", result);
	}


	return EXIT_SUCCESS;
}