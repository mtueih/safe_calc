#ifndef SAFE_CALC_H
#define SAFE_CALC_H

#include <stddef.h>
#if !defined(__STDC_VERSION__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
	#include <stdbool.h>
#endif

bool safe_size_add(size_t a, size_t b, size_t *result);

#endif
