cmake_minimum_required(VERSION 3.9)
project(mownit_matrix_multiplication C)

set(CMAKE_C_STANDARD 11)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
find_package(GSL REQUIRED)
add_executable(mownit_matrix_multiplication main.c)
target_link_libraries(mownit_matrix_multiplication GSL::gsl GSL::gslcblas m)
