# ==============================================================================
# safe_calc 库定义
# ==============================================================================
# 仅负责：库目标创建 + 头文件搜索路径配置。
# 安装规则、CMake 包配置等全部位于顶层 CMakeLists.txt。

# 静态库目标
add_library(safe_calc STATIC safe_calc.c)
add_library(safe_calc::safe_calc ALIAS safe_calc)

# 头文件搜索路径
#   BUILD_INTERFACE   — 构建期内，指向项目源码树中的 include/
#   INSTALL_INTERFACE — 安装后，指向 ${CMAKE_INSTALL_INCLUDEDIR}
target_include_directories(safe_calc PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
