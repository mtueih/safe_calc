**English** | [简体中文](README.zh-CN.md)

# safe_calc

[![C Standard](https://img.shields.io/badge/C-C99/C11/C17/C23-blue.svg)](https://en.cppreference.com/c)
[![CMake](https://img.shields.io/badge/CMake-3.21+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/safe_calc)](LICENSE)
[![CI](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml/badge.svg)](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml)

A C library for safe calculation, primarily used to prevent various arithmetic overflows.

## Installation

### CMake

Requirements:

- [CMake](https://cmake.org/) 3.21 or later.

```bash
# Clone the repository.
git clone https://github.com/mtueih/safe_calc.git
cd safe_calc

# Configure and install.
cmake . -B build -DSAFE_CALC_INSTALL=ON -DBUILD_TESTING=OFF
cmake --build build
cmake --install build
```

### CPM.cmake

Requirements:

- [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake).

Add the following to your `CMakeLists.txt`:

```cmake
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
	NAME safe_calc
	GITHUB_REPOSITORY mtueih/safe_calc
	GIT_TAG v0.2.2
	OPTIONS "SAFE_CALC_INSTALL OFF" "BUILD_TESTING OFF"
)
```

## Usage

### CMake

Add the following to your `CMakeLists.txt`:

```cmake
find_package(safe_calc REQUIRED)

target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```

## Example

```c
#include <safe_calc.h>
#include <stddef.h>

int main(void) {
	size_t a, b, result;

	a = 1;
	b = 2;

	/* Check only. */
	if (safe_size_t_add(a, b, NULL)) {
		printf("%zu + %zu will not overflow.\n", a, b);
	}

	/* Calculate. */
	if (safe_size_t_add(a, b, &result)) {
		printf("%zu + %zu = %zu\n", a, b, result);
	}

	return 0;
}
```

## Documentation

- [API Reference](docs/api-reference.md).

## License

This project is licensed under the [ISC License](https://www.isc.org/licenses/) — see the [LICENSE](LICENSE) file for details.
