cmake_minimum_required(VERSION 3.10)
project(Sweep)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -ggdb3 -fno-strict-aliasing)

include_directories(include runtime)
file(GLOB SOURCES "src/*.c")

add_executable(sweep src/main.c ${SOURCES})

target_link_libraries(sweep m pthread)