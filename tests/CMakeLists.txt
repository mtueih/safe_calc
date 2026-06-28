# ==============================================================================
# safe_calc 单元测试
# ==============================================================================
# 仅当 BUILD_TESTING=ON 时由顶层 CMakeLists.txt 引入。

# 测试可执行文件，链接 safe_calc
add_executable(test_safe_calc test_safe_calc.c)
target_link_libraries(test_safe_calc PRIVATE safe_calc)

# 注册到 CTest
add_test(NAME test_safe_calc COMMAND test_safe_calc)
