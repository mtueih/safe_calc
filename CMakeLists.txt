cmake_minimum_required(VERSION 4.2)
project(safe_calc C)

set(CMAKE_C_STANDARD 11)

add_library(safe_calc STATIC src/safe_calc.c)
target_include_directories(safe_calc PUBLIC include)

add_executable(safe_calc_test src/main.c)
target_link_libraries(safe_calc_test PRIVATE safe_calc)
