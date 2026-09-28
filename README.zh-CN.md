<div align="right">

[English](README.md) | **简体中文**

</div>

# safe_calc

[![C Standard](https://img.shields.io/badge/C-C99+-blue.svg)](https://zh.cppreference.com/c)
[![CMake](https://img.shields.io/badge/CMake-3.24+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/safe_calc)](LICENSE)
[![CI](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml/badge.svg)](https://github.com/mtueih/safe_calc/actions/workflows/ci.yml)

一个用于安全数值计算的 C 头文件库，主要用于防止各种算数溢出。

## API

此库目前包含针对以下类型的若干种运算函数：

- 无符号整数：加、减、乘、除、求模。
- 有符号整数：加、减、乘、除、求模、取相反数。

其中，

- 除法、求模运算支持除零检测。
- 无符号整数及有符号整数包含所有 C99 及以上标准所严格支持的无符号/有符号整数类型。

具体请参阅：[API 参考](docs/api-reference.zh-CN.md)。

## 在其他项目中使用

### 添加依赖

#### CPM.cmake

环境要求：[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)。

在 `CMakeLists.txt` 中：

```cmake
include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:mtueih/safe_calc#v0.2.2")
```

#### CMake find_package（需已安装）

在 `CMakeLists.txt` 中：

```cmake
find_package(safe_calc REQUIRED)
```

### 链接库

在 `CMakeLists.txt` 中：

```cmake
target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```

### 在代码中使用

#### 引入头文件

```c
#include <safe_calc/safe_calc.h>
```

#### 使用库函数

```c
#include <safe_calc/safe_calc.h>
#include <stddef.h>

int main(void)
{
    int a, b, result;

    a = 1;
    b = 2;

    /* 仅判断。 */
    if (safe_int_add(a, b, NULL) == SAFE_CALC_OK)
    {
        printf("【%d + %d】不会溢出。\n", a, b);
    }

    /* 计算。 */
    if (safe_int_add(a, b, &result) == SAFE_CALC_OK)
    {
        printf("%d + %d = %d\n", a, b, result);
    }

    return 0;
}
```

## 从源码构建

### 环境要求

- [CMake](https://cmake.org/) 3.24+。
- 支持 [C99](https://zh.cppreference.com/c/99)+ 的 [C 编译器](https://zh.cppreference.com/c/compiler_support)（MSVC / MinGW-w64 / Clang）。

### 构建步骤

#### 克隆仓库

```bash
git clone https://github.com/mtueih/safe_calc.git --depth 1 -b v0.2.2
cd safe_calc
```

#### 配置、构建与安装

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DSAFE_CALC_INSTALL=ON
cmake --build build --config Release --parallel
cmake --install build --config Release --strip --prefix install
```

有关上述命令的说明：

- 安装命令。通过 `--prefix install` 将产物安装在了 `install` 目录下，而不是全局安装，以便你按自己的方式使用安装产物。如果你希望全局安装，则删除它即可。

## 许可协议

本项目采用 [ISC 许可证](https://www.isc.org/licenses/) 授权——详情请参阅 [LICENSE](LICENSE) 文件。
