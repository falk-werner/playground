cmake_minimum_required (VERSION 3.10)
project(find-in-files VERSION 1.0.0 DESCRIPTION "find-in-files")

set(CMAKE_C_STANDARD 99)

set(C_WARNINGS -Wall -Wextra)

add_executable(find-in-files
    src/main.c
    src/filewalker.c
    src/path.c
)
