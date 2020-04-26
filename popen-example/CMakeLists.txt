cmake_minimum_required (VERSION 3.10)
project(popen)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

add_executable(popen
    src/main.c
)