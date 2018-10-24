cmake_minimum_required(VERSION 3.9)
project(Mownit_Lab2 C)

set(CMAKE_C_STANDARD 11)
find_package(GSL REQUIRED)
add_executable(Mownit_Lab2 main.c)
target_link_libraries(Mownit_Lab2 GSL::gsl GSL::gslcblas m)

