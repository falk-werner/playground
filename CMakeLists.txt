cmake_minimum_required (VERSION 3.10)
project(png-example)

set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

add_executable(png_turn_gray
    src/png_image.c
    src/main.c
)