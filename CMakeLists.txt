# ==============================================================================
# safe_calc — 顶层 CMake 构建脚本
# ==============================================================================
# 职责：
#   1. 项目声明与核心模块引入
#   2. 子目录构建目标编排
#   3. 安装规则（头文件 / 库 / CMake 包配置）
#   4. 测试入口（CTest）
# ==============================================================================

cmake_minimum_required(VERSION 3.21)
project(safe_calc VERSION 0.1.0 LANGUAGES C)

# ------------------------------------------------------------------------
# 核心模块
# ------------------------------------------------------------------------
# CTest                     — 测试框架支持
# GNUInstallDirs            — 平台标准安装路径变量（lib / bin / include）
# CMakePackageConfigHelpers — configure_package_config_file 等包配置宏
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ------------------------------------------------------------------------
# 构建目标
# ------------------------------------------------------------------------
add_subdirectory(src)

# ------------------------------------------------------------------------
# 安装规则
# ------------------------------------------------------------------------

# 公共头文件
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

# 库文件及 EXPORT 定义（safe_calcTargets）
# 同时指定 INCLUDES DESTINATION，使下游 find_package 后自动获得头文件路径
install(TARGETS safe_calc
	EXPORT safe_calcTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 安装 EXPORT 目标文件
# 下游通过 find_package(safe_calc) 即可导入 safe_calc::safe_calc 目标
install(EXPORT safe_calcTargets
	FILE safe_calcTargets.cmake
	NAMESPACE safe_calc::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/safe_calc
)

# ------------------------------------------------------------------------
# CMake 包配置（find_package 支持）
# ------------------------------------------------------------------------

# 从 .in 模板生成 safe_calcConfig.cmake
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/safe_calcConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/safe_calc
)

# 生成版本兼容文件，策略：SameMajorVersion
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

# 安装 Config + Version 文件至包搜索路径
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/safe_calc
)

# ------------------------------------------------------------------------
# 测试
# ------------------------------------------------------------------------
if(BUILD_TESTING)
	add_subdirectory(tests)
endif()
