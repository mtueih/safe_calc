# safe_calc

![C99](https://img.shields.io/badge/standard-C99-blue.svg)

## 安装

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

```cmake
find_package(safe_calc REQUIRED)
target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```
