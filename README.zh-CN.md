[English](README.md) | **简体中文**

# safe_calc

[![C Standard](https://img.shields.io/badge/C-C99/C11/C17/C23-blue.svg)](https://zh.cppreference.com/c)
[![CMake](https://img.shields.io/badge/CMake-3.21+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/safe_calc)](LICENSE)
[![CI](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml/badge.svg)](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml)

一个 C 语言安全计算库，主要用于防止各种算数溢出。

## 安装

### CMake

环境要求：

- [CMake](https://cmake.org/) 3.21 或更高版本。

```bash
# 克隆仓库。
git clone https://github.com/mtueih/safe_calc.git
cd safe_calc

# 配置并安装。
cmake . -B build -DSAFE_CALC_INSTALL=ON -DBUILD_TESTING=OFF
cmake --build build
cmake --install build
```

### CPM.cmake

环境要求：

- [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)。

在 `CMakeLists.txt` 中添加以下内容：

```cmake
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
	NAME safe_calc
	GITHUB_REPOSITORY mtueih/safe_calc
	GIT_TAG v0.2.2
	OPTIONS "SAFE_CALC_INSTALL OFF" "BUILD_TESTING OFF"
)
```

## 使用

### CMake

在 `CMakeLists.txt` 中添加以下内容：

```cmake
find_package(safe_calc REQUIRED)

target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```

## 示例

```c
#include <safe_calc.h>
#include <stddef.h>

int main(void) {
	size_t a, b, result;

	a = 1;
	b = 2;

	/* 仅判断。 */
	if (safe_size_t_add(a, b, NULL)) {
		printf("【%zu + %zu】不会溢出。\n", a, b);
	}

	/* 计算。 */
	if (safe_size_t_add(a, b, &result)) {
		printf("%zu + %zu = %zu\n", a, b, result);
	}

	return 0;
}
```

## 文档

- [API 参考](docs/api-reference.zh-CN.md)。

## 许可协议

本项目采用 [ISC许可证](https://www.isc.org/licenses/) 授权——详情请参阅 [LICENSE](LICENSE) 文件。
