<div align="right">

**English** | [简体中文](README.zh-CN.md)

</div>

# safe_calc

[![C Standard](https://img.shields.io/badge/C-C99+-blue.svg)](https://en.cppreference.com/w/c)
[![CMake](https://img.shields.io/badge/CMake-3.24+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/safe_calc)](LICENSE)
[![CI](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml/badge.svg)](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml)

A header-only C library for **safe** numerical **calc**ulations, primarily designed to prevent various arithmetic overflows.

## API

This library currently provides calculation functions for the following types:

- Unsigned integers: addition, subtraction, multiplication, division, and modulo.
- Signed integers: addition, subtraction, multiplication, division, modulo, and negation.

Among them,

- Division and modulo calculations support zero-division detection.
- Both unsigned and signed integers cover all unsigned/signed integer types strictly supported by the C99 and later standards.

For details, please refer to the [API Reference](docs/api-reference.md).

## Using in Other Projects

### Adding the Dependency

#### CPM.cmake

Requirements: [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake).

In `CMakeLists.txt`:

```cmake
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:mtueih/safe_calc#v1.0.1")
```

#### CMake find_package (requires installation)

In `CMakeLists.txt`:

```cmake
find_package(safe_calc REQUIRED)
```

### Linking the Library

In `CMakeLists.txt`:

```cmake
target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```

### Using in Code

#### Including the Header

```cpp
#include <safe_calc/safe_calc.h>
```

#### Using Library Functions

```c
#include <safe_calc/safe_calc.h>
#include <stddef.h>

int main(void)
{
    int a, b, result;

    a = 1;
    b = 2;

    /* Check only. */
    if (safe_int_add(a, b, NULL) == SAFE_CALC_OK)
    {
        printf("[%d + %d] will not overflow.\n", a, b);
    }

    /* Perform the calculation. */
    if (safe_int_add(a, b, &result) == SAFE_CALC_OK)
    {
        printf("%d + %d = %d\n", a, b, result);
    }

    return 0;
}
```

## Building from Source

### Requirements

- [CMake](https://cmake.org/) 3.24+.
- A [C compiler](https://en.cppreference.com/w/c/compiler_support) that supports [C99](https://en.cppreference.com/w/c/99)+ (MSVC / MinGW-w64 / Clang).

### Build Steps

#### Clone the Repository

```bash
git clone https://github.com/mtueih/safe_calc.git --depth 1 -b v1.0.1
cd safe_calc
```

#### Configure, Build, and Install

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DSAFE_CALC_INSTALL=ON
cmake --build build --config Release --parallel
cmake --install build --config Release --strip --prefix install
```

Notes on the above commands:

- Installation command: Using `--prefix install` installs the artifacts into the `install` directory instead of a system-wide location, so you can use the installed artifacts in your preferred way. If you want a system-wide installation, simply omit the `--prefix` option.

## License

This project is licensed under the [ISC License](https://www.isc.org/licenses/) — see the [LICENSE](LICENSE) file for details.
