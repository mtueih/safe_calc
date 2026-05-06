# safe_calc

## 安装

```bash
# 克隆仓库
git clone https://github.com/mtueih/safe_calc.git
cd safe_calc

# 创建构建目录
mkdir build && cd build

# 配置并安装
cmake ..
cmake --build .
cmake --install .
```

## 使用

### CMake 集成

```cmake
find_package(safe_calc REQUIRED)
target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```

### Git 子模块

```bash
# 添加为 git 子模块
git submodule add https://github.com/mtueih/safe_calc.git deps/safe_calc
```

在你的 CMakeLists.txt 中

```cmake
add_subdirectory(deps/safe_calc)
target_link_libraries(your_target PRIVATE safe_calc::safe_calc)
```
