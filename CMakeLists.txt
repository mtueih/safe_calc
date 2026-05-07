cmake_minimum_required(VERSION 4.2)
project(safe_calc VERSION 0.1.0 LANGUAGES C)

# 引入标准 CMake 模块用于安装和包配置
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_C_STANDARD 11)

add_subdirectory(deps/attrs@mtueih)

# 创建库目标（静态库）
add_library(safe_calc STATIC src/safe_calc.c)
add_library(safe_calc::safe_calc ALIAS safe_calc)
target_link_libraries(safe_calc PUBLIC attrs::attrs)

# 配置包含目录，使用生成器表达式：
# - BUILD_INTERFACE：直接构建项目时使用
# - INSTALL_INTERFACE：使用已安装的包时使用
target_include_directories(safe_calc PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# 可选：添加测试可执行文件（仅用于开发测试，不安装）
add_executable(safe_calc_test src/main.c)
target_link_libraries(safe_calc_test PRIVATE safe_calc)

# 安装头文件到标准包含目录
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

# 安装库文件到标准库目录
install(TARGETS safe_calc
	EXPORT safe_calcTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 导出目标定义供下游项目使用
install(EXPORT safe_calcTargets
	FILE safe_calcTargets.cmake
	NAMESPACE safe_calc::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/safe_calc
)

# 从模板生成包配置文件
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/safe_calcConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/safe_calc
)

# 生成版本兼容性文件（支持同主版本兼容策略）
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

# 安装包配置文件和版本文件，以支持 find_package() 功能
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/safe_calcConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/safe_calc
)
