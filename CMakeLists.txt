cmake_minimum_required(VERSION 3.20)
project(AIA_algorithms C)

set(CMAKE_C_STANDARD 11)

# This line tells CLion to include main.c AND every .c file in your algorithms folder
file(GLOB ALGO_SOURCES "algorithms/*.c")

add_executable(main main.c ${ALGO_SOURCES})