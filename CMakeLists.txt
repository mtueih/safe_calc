# =============================================================================
# safe_calc
#
# 顶层构建脚本，用于声明项目元信息并组织全局构建与安装。
#
# 负责：
#   - 定义项目及版本信息。
#   - 配置通用 CMake 模块与安装目录。
#   - 组织源码与测试的构建流程。
#   - 生成并安装供 find_package 使用的包配置文件。
# =============================================================================
cmake_minimum_required(VERSION 3.21)

project(safe_calc
	VERSION 0.1.0
	LANGUAGES C
)


# -----------------------------------------------------------------------------
# 全局配置
# -----------------------------------------------------------------------------
# 引入构建、测试与安装所需的通用 CMake 模块，并统一安装路径变量。
#
# - CTest: 提供测试支持并定义 BUILD_TESTING 选项。
# - GNUInstallDirs: 定义跨平台的标准安装目录变量，如 CMAKE_INSTALL_LIBDIR。
# - CMakePackageConfigHelpers: 提供生成 CMake 包配置文件的辅助函数。
# -----------------------------------------------------------------------------
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)


set(MAIN_PACKAGE_NAME ${PROJECT_NAME})
set(MAIN_LIB_TARGET_NAME ${PROJECT_NAME})

# -----------------------------------------------------------------------------
# 源码构建
# -----------------------------------------------------------------------------
add_subdirectory(src)


# -----------------------------------------------------------------------------
# 测试
# -----------------------------------------------------------------------------
# 控制测试构建行为。仅在启用 BUILD_TESTING 时添加 tests 子目录并注册测试。
#
# 若需跳过测试构建，可在命令行或打包流程中使用 -DBUILD_TESTING=OFF。
# -----------------------------------------------------------------------------
if (BUILD_TESTING)
	add_subdirectory(tests)
endif ()


# -----------------------------------------------------------------------------
# 包配置与安装
# -----------------------------------------------------------------------------
# 生成并安装供 find_package 导入的 CMake 包配置文件，并导出构建目标。
#
# 生成的文件包括：
#
#   1. safe_calcConfig.cmake
#      包入口配置文件，用于在下游项目中定位并导入导出目标。
#
#   2. safe_calcConfigVersion.cmake
#      用于在 find_package 时进行版本兼容性检查（SameMajorVersion）。
# -----------------------------------------------------------------------------

# 安装 EXPORT 目标文件
# 下游通过 find_package(safe_calc) 即可导入 safe_calc::safe_calc 目标
install(EXPORT ${MAIN_PACKAGE_NAME}Targets
	FILE ${MAIN_PACKAGE_NAME}Targets.cmake
	NAMESPACE ${MAIN_PACKAGE_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MAIN_PACKAGE_NAME}
)


# 从模板生成包配置文件。
configure_package_config_file(
	${PROJECT_SOURCE_DIR}/cmake/${MAIN_PACKAGE_NAME}Config.cmake.in
	${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}Config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MAIN_PACKAGE_NAME}
)

# 生成版本兼容性检查文件。
write_basic_package_version_file(
	${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}ConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

# 将生成的包配置文件安装到标准 CMake 包目录。
install(FILES
	${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}Config.cmake
	${PROJECT_BINARY_DIR}/${MAIN_PACKAGE_NAME}ConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MAIN_PACKAGE_NAME}
)