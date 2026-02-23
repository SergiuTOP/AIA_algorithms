cmake_minimum_required(VERSION 3.20)
project(AIA_algorithms C)

set(CMAKE_C_STANDARD 11)

file(GLOB ALGO_SOURCES "algorithms/*.c")

add_executable(sort main.c ${ALGO_SOURCES})

if (WIN32)
    target_link_libraries(sort PRIVATE psapi)
endif()
