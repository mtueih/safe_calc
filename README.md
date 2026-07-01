# safe_calc

[![C Standard](https://img.shields.io/badge/C-C99/C11/C17/C23-blue.svg)](https://zh.cppreference.com/c)
[![CMake](https://img.shields.io/badge/CMake-3.21+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/safe_calc)](LICENSE)
[![CMake: Build & Test](https://github.com/mtueih/safe_calc/actions/workflows/cmake-build-and-test.yml/badge.svg)](https://github.com/mtueih/safe_calc/actions/workflows/cmake-build-and-test.yml)

## 安装

### CMake

**环境要求**：

- [CMake](https://cmake.org/) 3.21 或更高版本。

```bash
# 克隆仓库。
git clone https://github.com/mtueih/safe_calc.git
cd safe_calc

# 创建构建目录。
mkdir build && cd build

# 配置并安装。
cmake .. -DSAFE_CALC_INSTALL=ON -DBUILD_TESTING=OFF
cmake --build .
cmake --install .
```

### CPM.cmake

**环境要求**：

- [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)。

在 CMakeLists.txt 中添加以下内容：

```cmake
include(cmake/CPM.cmake)

CPMAddPackage(
	NAME safe_calc
	GITHUB_REPOSITORY mtueih/safe_calc
	GIT_TAG v0.2.0
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

## 许可协议

本项目采用 [GNU 通用公共许可证 v3.0](https://www.gnu.org/licenses/gpl-3.0.html) 授权——详情请参阅 [LICENSE](LICENSE) 文件。
