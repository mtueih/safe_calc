# safe_calc

[![C Standard](https://img.shields.io/badge/C-C99/C11/C17/C23-blue.svg)](https://cppreference.cn/w/c)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)
[![GitHub License](https://img.shields.io/github/license/mtueih/safe_calc)](LICENSE)
[![CMake on multiple platforms](https://github.com/mtueih/safe_calc/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/mtueih/safe_calc/actions/workflows/cmake-multi-platform.yml)

## 安装

**环境要求**：

- CMake 3.14 或更高版本。

**依赖**：

- [`attrs`](https://github.com/mtueih/attrs)。

```bash
# 克隆仓库。
git clone https://github.com/mtueih/safe_calc.git
cd safe_calc

# 创建构建目录。
mkdir build && cd build

# 配置并安装。
cmake ..
cmake --build .
cmake --install .
```

## 使用

### CMake

在 `CMakeLists.txt` 中添加以下内容：

```cmake
find_package(safe_calc REQUIRED)

target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```

## 许可协议

本项目采用 [GNU 通用公共许可证 v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html) 授权——详情请参阅 [LICENSE](LICENSE) 文件。
