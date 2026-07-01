# =============================================================================
# safe_calc - 项目源码目录主 CMake 脚本
# =============================================================================
# 职责：
#	1. 项目源码的构建。
#	2. 源码构建产物的安装（如果有）。
# =============================================================================


# -----------------------------------------------------------------------------
# 源码构建
# -----------------------------------------------------------------------------

# 构建项目主库（静态）。
add_library(${MAIN_LIB_TARGET_NAME} STATIC ${MAIN_LIB_TARGET_NAME}.c)

add_library(${MAIN_PACKAGE_NAME}::${MAIN_LIB_TARGET_NAME} ALIAS ${MAIN_LIB_TARGET_NAME})

# 配置项目主库的头文件搜索路径。
if (NOT SAFE_CALC_INSTALL)
	target_include_directories(${MAIN_LIB_TARGET_NAME}
		PUBLIC ${PROJECT_SOURCE_DIR}/include
	)
else ()
	# 指定构建期和安装期的头文件导出接口，以便在源码树内构建与在下游项目中使用时都能正确找到头文件。
	target_include_directories(${MAIN_LIB_TARGET_NAME} PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)
endif ()


# ------------------------------------------------------------------------
# 源码构建产物的安装
# ------------------------------------------------------------------------
if (SAFE_CALC_INSTALL)
	# 安装项目的公共头文件。
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		FILES_MATCHING PATTERN "*.h"
	)

	# 安装项目主库的库文件并导出目标到项目主包的目标集。
	install(TARGETS ${MAIN_LIB_TARGET_NAME}
		EXPORT ${MAIN_PACKAGE_NAME}Targets
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
endif ()
